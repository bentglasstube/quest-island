#include "map_screen.h"

#include <iostream>

#include "island.h"

void MapScreen::init() {
  island_.reset(new Island());
  island_->generate();

  map_ = island_.get();
}

bool MapScreen::update(const Input& input, Audio&, unsigned int elapsed) {
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

  auto pos = map_->player->position();
  const Map::Tile t = map_->get_tile(pos.first, pos.second);
  if (t == Map::Tile::CAVE) {
    Island* i = dynamic_cast<Island*>(island_.get());
    map_ = i->get_cave(pos.first, pos.second);

    if (!map_) {
      std::cerr << "Couldn't get cave map, staying on island\n";
      map_ = island_.get();
    }
  }

  return true;
}

void MapScreen::draw(Graphics& graphics) const {
  const int visibility = 25 * map_->visibility();
  map_->draw(graphics, 0, 0, graphics.width(), graphics.height(), visibility);
}

Screen* MapScreen::next_screen() {
  return NULL;
}
