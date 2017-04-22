#include "map.h"

Map::Map() {
  sprites_.reset(new SpriteMap("map.png", 8, TILE_SIZE, TILE_SIZE));
}

void Map::draw(Graphics& graphics, int x, int y, int mx, int my, int mw, int mh) const {
  for (int iy = 0; iy < mh; ++iy) {
    for (int ix = 0; ix < mw; ++ix) {
      const int t = static_cast<int>(get_tile(ix + mx, iy + my));
      sprites_->draw(graphics, t, x + TILE_SIZE * ix, y + TILE_SIZE * iy);
    }
  }
}

bool Map::walkable(int x, int y) const {
  const Tile t = get_tile(x, y);
  return !(t == Tile::WATER || t == Tile::MOUNTAINS || t == Tile::WALL);
}

void Map::add_overlay(int x, int y, Tile tile) {
  overlays_.insert({ std::make_pair(x, y), tile });
}

void Map::generate() {
  std::random_device r;
  generate(r());
}
