#include "map_screen.h"

#include <cstdlib>

void MapScreen::init() {
  map_.reset(new Map(std::rand()));
  player_.reset(new Character(Character::Role::WIZARD));
  px = py = 0;
}

bool MapScreen::update(const Input& input, Audio&, unsigned int) {
  if (input.key_held(SDL_SCANCODE_W)) {
    player_->set_facing(Character::Facing::UP);
    if (map_->walkable(px, py - 1)) --py;
  } else if (input.key_held(SDL_SCANCODE_A)) {
    player_->set_facing(Character::Facing::LEFT);
    if (map_->walkable(px - 1, py)) --px;
  } else if (input.key_held(SDL_SCANCODE_S)) {
    player_->set_facing(Character::Facing::DOWN);
    if (map_->walkable(px, py + 1)) ++py;
  } else if (input.key_held(SDL_SCANCODE_D)) {
    player_->set_facing(Character::Facing::RIGHT);
    if (map_->walkable(px + 1, py)) ++px;
  }

  if (input.key_pressed(SDL_SCANCODE_SPACE)) {
    map_.reset(new Map(std::rand()));
    px = py = 0;
  }

  return true;
}

void MapScreen::draw(Graphics& graphics) const {
  const int nx = graphics.width() / 8;
  const int ny = graphics.height() / 8;

  map_->draw(graphics, 0, 0, px - nx / 2, py - ny / 2, nx, ny);
  player_->draw(graphics, graphics.width() / 2, graphics.height() / 2);
}

Screen* MapScreen::next_screen() {
  return NULL;
}
