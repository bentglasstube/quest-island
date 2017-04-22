#include "map.h"

Map::Map() {
  sprites_.reset(new SpriteMap("map.png", 8, TILE_SIZE, TILE_SIZE));
}

void Map::draw(Graphics& graphics, int x, int y, int mx, int my, int mw, int mh, int radius) const {
  const int cx = mw / 2;
  const int cy = mh / 2;

  for (int iy = 0; iy < mh; ++iy) {
    for (int ix = 0; ix < mw; ++ix) {
      const float d = (ix - cx) * (ix - cx) + (iy - cy) * (iy - cy);

      if (d < radius * radius) {
        const int t = static_cast<int>(get_tile(ix + mx, iy + my));
        sprites_->draw(graphics, t, x + TILE_SIZE * ix, y + TILE_SIZE * iy);
      }
    }
  }
}

bool Map::walkable(int x, int y) const {
  switch (get_tile(x, y)) {
    case Tile::WATER:
    case Tile::MOUNTAINS:
    case Tile::WALL:
    case Tile::TOWN:
    case Tile::CHEST:
      return false;

    default:
      return true;
  }
}

float Map::visibility() const {
  return 1.0f;
}

void Map::add_overlay(int x, int y, Tile tile) {
  overlays_.insert({ std::make_pair(x, y), tile });
}

void Map::generate() {
  std::random_device r;
  generate(r());
}
