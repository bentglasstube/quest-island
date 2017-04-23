#include "island.h"

#define STB_PERLIN_IMPLEMENTATION 1
#include "stb_perlin.h"

Map::Tile Island::get_tile(int x, int y) const {
  auto overlay = overlays_.find(std::make_pair(x, y));
  if (overlay != overlays_.end()) {
    return overlay->second;
  }

  const float e = elevation(x, y);
  const float m = moisture(x, y);

  if (e > 0.030f) {
    if (m > 0.1) return Tile::TREES;
    return Tile::MOUNTAINS;
  } else if (e > 0.005f) {
    if (m > 0.3) return Tile::SWAMP;
    if (m > 0.1) return Tile::TREES;
    if (m > -0.2) return Tile::GRASS;
    return Tile::SAND;
  } else if (e > 0.0f) {
    return Tile::SAND;
  } else {
    return Tile::WATER;
  }
}

void Island::generate(unsigned int seed) {
  rand_.seed(seed);

  player_.reset(new Character(Character::Role::PEASANT, 0, 0));

  std::uniform_real_distribution<float> u(-100, 100);
  elevation_z_ = u(rand_);
  moisture_z_ = u(rand_);

  std::normal_distribution<float> n(2, 1);

  const int caves = n(rand_);
  for (int i = 0; i < caves; ++i) {
    place_cave();
  }

  const int towns = n(rand_);
  for (int i = 0; i < towns; ++i) {
    place_town();
  }

  place_npc(Character::Role::WIZARD);
  place_npc(Character::Role::KNIGHT);
  place_npc(Character::Role::PRINCESS);
}

float Island::elevation(int x, int y) const {
  const float d = x * x + y * y;
  const float e = noise( 1 * x,  1 * y, elevation_z_) / 2.0f
    + noise( 2 * x,  2 * y, elevation_z_) / 4.0f
    + noise( 4 * x,  4 * y, elevation_z_) / 8.0f;
  return powf(e, 3.0f) + 0.03f - NOISE_ZOOM * 0.0001 * d;
}

float Island::moisture(int x, int y) const {
  const float m = noise( 1 * x,  1 * y, moisture_z_) / 2.0f
    + noise( 2 * x,  2 * y, moisture_z_) / 4.0f
    + noise( 4 * x,  4 * y, moisture_z_) / 8.0f;
  return m;
}

float Island::noise(int x, int y, int z) const {
  return stb_perlin_noise3(NOISE_ZOOM * x, NOISE_ZOOM * y, z);
}

void Island::place_cave() {
  std::normal_distribution<float> normal(0, 100);

  while (true) {
    int cx = normal(rand_);
    int cy = normal(rand_);

    if (get_tile(cx, cy) == Tile::MOUNTAINS) {
      while (get_tile(cx, cy) == Tile::MOUNTAINS) {
        ++cy;
      }
      add_overlay(cx, cy - 1, Tile::CAVE);
      return;
    }
  }
}

void Island::place_town() {
  std::normal_distribution<float> normal(0, 100);

  while (true) {
    int cx = normal(rand_);
    int cy = normal(rand_);

    if (get_tile(cx, cy) == Tile::GRASS) {
      add_overlay(cx, cy, Tile::TOWN);
      return;
    }
  }
}

void Island::place_npc(Character::Role role) {
  std::normal_distribution<float> normal(0, 100);

  while (true) {
    int cx = normal(rand_);
    int cy = normal(rand_);

    if (walkable(cx, cy)) {
      add_npc(role, cx, cy);
      return;
    }
  }
}

