#include "map.h"

#define STB_PERLIN_IMPLEMENTATION 1
#include "stb_perlin.h"

Map::Map(int seed) : seed_(seed / 12.324f + 38.238f) {
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

  if (e > 0.030f) {
    if (m > 0.0f) return Tile::TREES;
    return Tile::MOUNTAINS;
  }

  if (e > 0.005f) {
    if (m > 0.5f) return Tile::SWAMP;
    if (m > 0.2f) return Tile::TREES;
    if (m > -0.2f) return Tile::GRASS;
    return Tile::SAND;
  }

  if (e > 0.000f) return Tile::SAND;
  return Tile::WATER;
}

float Map::elevation(int x, int y) const {
  const float d = x * x + y * y;
  const float e = noise( 1 * x,  1 * y, seed_) / 2.0f
                + noise( 2 * x,  2 * y, seed_) / 4.0f
                + noise( 4 * x,  4 * y, seed_) / 8.0f;
  return powf(e, 3.0f) + 0.03f - NOISE_ZOOM * 0.0005 * d;
}

float Map::moisture(int x, int y) const {
  const float m = noise( 1 * x,  1 * y, seed_ + 15) / 2.0f
                + noise( 2 * x,  2 * y, seed_ + 15) / 4.0f
                + noise( 4 * x,  4 * y, seed_ + 15) / 8.0f;
  return m;
}

float Map::noise(int x, int y, int z) const {
  // TODO move these
  const float wrap = 16;

  return stb_perlin_noise3(NOISE_ZOOM * x, NOISE_ZOOM * y, z, wrap, wrap);
}
