#include "map_screen.h"

#include "cave.h"
#include "island.h"

void MapScreen::init() {
  player_.reset(new Character(Character::Role::PEASANT));
  map_.reset(new Cave());

  map_->generate();

  auto start = map_->spawn();
  px_ = start.first;
  py_ = start.second;
}

bool MapScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  player_->update(elapsed);

  if (!player_->waiting()) {
    bool moved = false;

    if (input.key_held(SDL_SCANCODE_W)) {
      player_->set_facing(Character::Facing::UP);
      if (map_->walkable(px_, py_ - 1)) {
        --py_;
        moved = true;
      } else {

      }
    } else if (input.key_held(SDL_SCANCODE_A)) {
      player_->set_facing(Character::Facing::LEFT);
      if (map_->walkable(px_ - 1, py_)) {
        --px_;
        moved = true;
      }
    } else if (input.key_held(SDL_SCANCODE_S)) {
      player_->set_facing(Character::Facing::DOWN);
      if (map_->walkable(px_, py_ + 1)) {
        ++py_;
        moved = true;
      }
    } else if (input.key_held(SDL_SCANCODE_D)) {
      player_->set_facing(Character::Facing::RIGHT);
      if (map_->walkable(px_ + 1, py_)) {
        ++px_;
        moved = true;
      }
    }

    if (moved) {
      switch (map_->get_tile(px_, py_)) {
        case Map::Tile::SWAMP:
          player_->add_wait(300);
          break;
        case Map::Tile::TREES:
          player_->add_wait(200);
          break;
        default:
          player_->add_wait(100);
      }
    }
  }

  return true;
}

void MapScreen::draw(Graphics& graphics) const {
  const int nx = graphics.width() / 8;
  const int ny = graphics.height() / 8;

  // TODO determine visibility
  const int visibility = 25 * map_->visibility();

  map_->draw(graphics, 0, 0, px_ - nx / 2, py_ - ny / 2, nx, ny, visibility);
  player_->draw(graphics, graphics.width() / 2, graphics.height() / 2);
}

Screen* MapScreen::next_screen() {
  return NULL;
}
