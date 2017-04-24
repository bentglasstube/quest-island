#include "map_screen.h"

#include <iostream>
#include <sstream>

#include "island.h"

void MapScreen::init() {
  island_.reset(new Island());
  island_->generate();

  map_ = island_.get();
  state_ = DisplayState::FADEIN;
  fade_time_ = FADE_DURATION;
}

bool MapScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {

  if (input.key_pressed(SDL_SCANCODE_RETURN)) {
    if (paused_) {
      paused_.reset();
    } else {
      paused_.reset(new PauseMenu(inventory_));
    }
  }

  if (dialog_) {

    dialog_->update(audio, elapsed);
    if (input.any_pressed() && dialog_->done()) dialog_.reset();

  } else if (paused_) {

    paused_->update(input, audio);

  } else {

    map_->update(elapsed);

    if (input.key_held(SDL_SCANCODE_W)) {
      map_->move_player(Character::Facing::UP);
    } else if (input.key_held(SDL_SCANCODE_A)) {
      map_->move_player(Character::Facing::LEFT);
    } else if (input.key_held(SDL_SCANCODE_S)) {
      map_->move_player(Character::Facing::DOWN);
    } else if (input.key_held(SDL_SCANCODE_D)) {
      map_->move_player(Character::Facing::RIGHT);
    } else {
      map_->player->stop();
    }

    if (input.key_pressed(SDL_SCANCODE_SPACE)) {
      auto p = map_->player->position();
      switch (map_->player->facing()) {
        case Character::Facing::UP:    p.second--; break;
        case Character::Facing::DOWN:  p.second++; break;
        case Character::Facing::LEFT:  p.first--;  break;
        case Character::Facing::RIGHT: p.first++;  break;
      }

      const Character* npc = map_->get_npc(p.first, p.second);
      if (npc) {

        dialog_.reset(new Dialog(npc->quest_hint()));

      } else {
        const Map::Tile t = map_->get_tile(p.first, p.second);
        switch (t) {

          case Map::Tile::CHEST:

            handle_chest(audio, p.first, p.second);
            break;

          case Map::Tile::EMPTY:
            dialog_.reset(new Dialog("It's empty."));
            break;

          case Map::Tile::TOWN:
            dialog_.reset(new Dialog("Looks like nobody is home."));
            break;

          case Map::Tile::TREES:
            if (player_has(Item::Type::AXE)) {
              map_->add_overlay(p.first, p.second, Map::Tile::STUMP);
              audio.play_sample("chop.wav");

              if (!player_has(Item::Type::LOG)) {
                inventory_.insert(Item::Type::LOG);
                audio.play_sample("fanfare.wav");
                dialog_.reset(new Dialog("You cut down the tree and\ngot a log!"));
              }
            } else {
              dialog_.reset(new Dialog("These are some very lovely\npine trees."));
            }
            break;

          case Map::Tile::WATER:
            if (player_has(Item::Type::ROD)) {
              if (!player_has(Item::Type::FISH)) {
                inventory_.insert(Item::Type::FISH);
                audio.play_sample("fanfare.wav");

                dialog_.reset(new Dialog("You caught a fresh fish!"));
              }
            } else {
              dialog_.reset(new Dialog("You can see plenty of fish\nswimming in the clear water."));
            }
            break;

          default:
            dialog_.reset(new Dialog("You looked all around, but\nyou didn't find anything."));
            break;
        }
      }
    }
  }

  if (state_ == DisplayState::FADEOUT) {
    map_->player->stop();
    fade_time_ -= elapsed;

    if (fade_time_ < 0) {
      map_ = next_map_;
      next_map_ = NULL;
      state_ = DisplayState::FADEIN;
      fade_time_ = FADE_DURATION;

      audio.stop_music();
    }

  } else if (state_ == DisplayState::FADEIN) {
    fade_time_ -= elapsed;
    if (fade_time_ < 0) {
      state_ = DisplayState::PLAYING;
    }

  } else {
    auto pos = map_->player->position();
    const Map::Tile t = map_->get_tile(pos.first, pos.second);
    if (t == Map::Tile::CAVE) {
      switch_maps(map_->get_cave(pos.first, pos.second));
    } else if (t == Map::Tile::ENTRANCE) {
      switch_maps(island_.get());
    }
  }

  return true;
}

void MapScreen::draw(Graphics& graphics) const {
  const int visibility = 25 * map_->visibility() * (player_has(Item::Type::TORCH) ? 3 : 1);
  map_->draw(graphics, 0, 0, graphics.width(), graphics.height(), visibility);

  if (dialog_) dialog_->draw(graphics, 8, 8);
  if (paused_) paused_->draw(graphics, 16, 16);

  int width = 0;
  switch (state_) {
    case DisplayState::FADEIN:
      width = graphics.width() / 2 * fade_time_ / FADE_DURATION;
      break;

    case DisplayState::FADEOUT:
      width = graphics.width() / 2 * (FADE_DURATION - fade_time_) / FADE_DURATION;
      break;

    default:
      width = 0;
      break;
  }

  if (width > 0) {
    SDL_Rect curtain = {0, 0, width, graphics.height()};
    graphics.draw_rect(&curtain, 0x000000ff, true);
    curtain.x = graphics.width() - width;
    graphics.draw_rect(&curtain, 0x000000ff, true);
  }
}

Screen* MapScreen::next_screen() {
  return NULL;
}

std::string MapScreen::get_music_track() const {
  if (map_) return map_->music_type();
  else return "island.wav";
}

void MapScreen::switch_maps(Map* next_map) {
  if (next_map) {
    next_map_ = next_map;

    // TODO investigate this
    auto pos = next_map_->player->position();
    next_map_->player->set_position(pos.first, pos.second + 1);

    state_ = DisplayState::FADEOUT;
    fade_time_ = FADE_DURATION;
  }
}

void MapScreen::handle_chest(Audio& audio, int x, int y) {
  Item* item = map_->open_chest(x, y);
  std::ostringstream msg;

  if (item) {
    audio.play_sample("fanfare.wav");
    msg << "You found the " << item->name() << "!";
    inventory_.insert(item->type());
  } else {
    msg << "Huh, there was nothing inside.";
  }

  dialog_.reset(new Dialog(msg.str()));
}

bool MapScreen::player_has(Item::Type item) const {
  return inventory_.find(item) != inventory_.end();
}
