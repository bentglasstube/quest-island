#include "map_screen.h"

#include <iostream>
#include <sstream>

#include "island.h"

void MapScreen::init() {
  island_.reset(new Island());
  island_->generate(23982374);

  map_ = island_.get();
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
        text  << "Hello, I am the " << npc->name();
        dialog_.reset(new Dialog());
        dialog_->add_page(text.str());

      } else {
        const Map::Tile t = map_->get_tile(p.first, p.second);
        switch (t) {
          case Map::Tile::CHEST:
            dialog_.reset(new Dialog());
            dialog_->add_page("You don't have any way to open that.");
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

    auto pos = map_->player->position();
    const Map::Tile t = map_->get_tile(pos.first, pos.second);
    if (t == Map::Tile::CAVE) {
      map_ = map_->get_cave(pos.first, pos.second);

      if (map_) {
        bump_player();
      } else {
        std::cerr << "Couldn't get cave map, staying on island\n";
        map_ = island_.get();
      }
    } else if (t == Map::Tile::ENTRANCE) {
      map_ = island_.get();
      bump_player();
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
}

Screen* MapScreen::next_screen() {
  return NULL;
}

void MapScreen::bump_player() {
  auto pos = map_->player->position();
  map_->player->set_position(pos.first, pos.second + 1);
}
