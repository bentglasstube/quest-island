#include "map_screen.h"

#include "cave.h"
#include "island.h"

void MapScreen::init() {
  map_.reset(new Island());
  map_->generate();
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
    map_->stop_player();
  }

  // TODO handle more inputs

  return true;
}

void MapScreen::draw(Graphics& graphics) const {
  const int visibility = 25 * map_->visibility();
  map_->draw(graphics, 0, 0, graphics.width(), graphics.height(), visibility);
}

Screen* MapScreen::next_screen() {
  return NULL;
}
