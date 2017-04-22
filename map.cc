#include "map.h"

#define STB_PERLIN_IMPLEMENTATION 1
#include "stb_perlin.h"

Map::Map(int seed) : seed_(seed / 12.324f + 38.238f), water_level_(0.0f) {
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

Map::Tile Map::get_tile(int x, int y) const {
  const float e = elevation(x, y);
  const float m = moisture(x, y);

  if (e < water_level_) return Tile::WATER;

  if (e > 0.030f) {
    if (m > 0.1) return Tile::TREES;
    return Tile::MOUNTAINS;
  } else if (e > 0.005f) {
    if (m > 0.3) return Tile::SWAMP;
    if (m > 0.1) return Tile::TREES;
    if (m > -0.2) return Tile::GRASS;
    return Tile::SAND;
  } else {
    return Tile::SAND;
  }
}

bool Map::walkable(int x, int y) const {
  const Tile t = get_tile(x, y);
  return !(t == Tile::WATER || t == Tile::MOUNTAINS);
}

void Map::increase_water_level() {
  water_level_ += 0.002;
}

void Map::decrease_water_level() {
  water_level_ -= 0.002;
}

float Map::elevation(int x, int y) const {
  const float d = x * x + y * y;
  const float e = noise( 1 * x,  1 * y, seed_) / 2.0f
                + noise( 2 * x,  2 * y, seed_) / 4.0f
                + noise( 4 * x,  4 * y, seed_) / 8.0f;
  return powf(e, 3.0f) + 0.03f - NOISE_ZOOM * 0.0001 * d;
}

float Map::moisture(int x, int y) const {
  const float m = noise( 1 * x,  1 * y, seed_ + 15) / 2.0f
                + noise( 2 * x,  2 * y, seed_ + 15) / 4.0f
                + noise( 4 * x,  4 * y, seed_ + 15) / 8.0f;
  return m;
}

float Map::noise(int x, int y, int z) const {
  return stb_perlin_noise3(NOISE_ZOOM * x, NOISE_ZOOM * y, z);
}
