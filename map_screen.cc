#include "map_screen.h"

#include <iostream>
#include <sstream>

#include "island.h"

void MapScreen::init() {
  island_.reset(new Island());
  island_->generate(23982374);

  map_ = island_.get();
  state_ = DisplayState::FADEIN;
  fade_time_ = FADE_DURATION;
}

bool MapScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (dialog_) {
    dialog_->update(audio, elapsed);
    if (input.any_pressed()) dialog_->advance();
    if (dialog_->all_done()) dialog_.reset();
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
        std::ostringstream text;
        text  << "Hello, I am the " << npc->name() << ".";
        dialog_.reset(new Dialog());
        dialog_->add_page(text.str());

      } else {
        const Map::Tile t = map_->get_tile(p.first, p.second);
        switch (t) {
          case Map::Tile::CHEST:
            map_->open_chest(p.first, p.second);

            dialog_.reset(new Dialog());
            dialog_->add_page("You found an inflatable\ndigshake inside!");
            break;

          case Map::Tile::EMPTY:
            dialog_.reset(new Dialog());
            dialog_->add_page("It's empty!");
            break;

          case Map::Tile::TOWN:
            dialog_.reset(new Dialog());
            dialog_->add_page("Looks like nobody is home.");
            break;

          default:
            dialog_.reset(new Dialog());
            dialog_->add_page("You looked all around, but\nyou didn't find anything.");
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
  const int visibility = 25 * map_->visibility();
  map_->draw(graphics, 0, 0, graphics.width(), graphics.height(), visibility);

  if (dialog_) {
    dialog_->draw(graphics, 8, 8, graphics.width() - 16, 64);
  }

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

void MapScreen::switch_maps(Map* next_map) {
  if (next_map) {
    next_map_ = next_map;

    /* auto pos = next_map_->player->position(); */
    /* next_map_->player->set_position(pos.first, pos.second + 1); */

    state_ = DisplayState::FADEOUT;
    fade_time_ = FADE_DURATION;
  }
}
