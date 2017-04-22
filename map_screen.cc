#include "map_screen.h"

#include <cstdlib>

void MapScreen::init() {
  map_.reset(new Map(std::rand()));
  ox = oy = 0;
}

bool MapScreen::update(const Input& input, Audio&, unsigned int) {
  if (input.key_held(SDL_SCANCODE_W)) --oy;
  if (input.key_held(SDL_SCANCODE_A)) --ox;
  if (input.key_held(SDL_SCANCODE_S)) ++oy;
  if (input.key_held(SDL_SCANCODE_D)) ++ox;

  if (input.key_pressed(SDL_SCANCODE_SPACE)) map_.reset(new Map(std::rand()));

  return true;
}

void MapScreen::draw(Graphics& graphics) const {
  const int nx = graphics.width() / 8;
  const int ny = graphics.height() / 8;

  map_->draw(graphics, 0, 0, ox - nx / 2, oy - ny / 2, nx, ny);
}

Screen* MapScreen::next_screen() {
  return NULL;
}
