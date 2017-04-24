#include "cave.h"

#include <iostream>

Map::Tile Cave::get_tile(int x, int y) const {
  auto overlay = overlays_.find(std::make_pair(x, y));
  if (overlay != overlays_.end()) {
    return overlay->second;
  }

  if (x < 0 || x >= SIZE) return Tile::WALL;
  if (y < 0 || y >= SIZE) return Tile::WALL;
  return data_[y][x];
}

void Cave::generate(unsigned int seed) {
  rand_.seed(seed);
  player.reset(new Character(Character::Role::PEASANT, 0, 0));

  std::uniform_real_distribution<float> u(0.0, 1.0f);

  for (int y = 0; y < SIZE; ++y) {
    for (int x = 0; x < SIZE; ++x) {
      data_[y][x] = u(rand_) < 0.4f ? Tile::WALL : Tile::DIRT;
    }
  }

  for (int i = 0; i < 4; ++i) {
    iterate([this](int x, int y){ return walls_within(x, y, 1) >= 5 || walls_within(x, y, 2) <= 2; });
  }
  for (int i = 0; i < 3; ++i) {
    iterate([this](int x, int y){ return walls_within(x, y, 1) >= 5; });
  }

  place_entrance();

  std::cerr << "Generated cave #" << seed << "\n";
}

std::string Cave::music_type() const {
  return "cave.wav";
}

float Cave::visibility() const {
  return 0.20f;
}

Item* Cave::open_chest(int x, int y) {
  if (get_tile(x, y) == Tile::CHEST) {
    data_[y][x] = Tile::EMPTY;

    auto i = chests_.find(std::make_pair(x, y));
    return &((*i).second);
  }

  return NULL;
}

void Cave::iterate(std::function<bool(int, int)> selector) {
  Tile d[SIZE][SIZE];
  for (int y = 0; y < SIZE; ++y) {
    for (int x = 0; x < SIZE; ++x) {
      d[y][x] = selector(x, y) ? Tile::WALL : Tile::DIRT;
    }
  }

  for (int y = 0; y < SIZE; ++y) {
    for (int x = 0; x < SIZE; ++x) {
      data_[y][x] = d[y][x];
    }
  }
}

int Cave::walls_within(int x, int y, int r) const {
  int count = 0;
  for (int iy = y - r; iy <= y + r; ++iy) {
    for (int ix  = x - r; ix <= x + r; ++ix) {
      if (get_tile(ix, iy) == Tile::WALL) ++count;
    }
  }

  return count;
}

void Cave::place_chest(Item::Type treasure) {
  std::normal_distribution<float> normal(0, 100);

  for (int i = 0; i < 100; ++i) {
    int cx = normal(rand_);
    int cy = normal(rand_);

    if (get_tile(cx, cy) == Tile::DIRT) {
      data_[cy][cx] = Tile::CHEST;

      auto r = chests_.emplace(std::make_pair(cx, cy), treasure);
      std::cerr << "Placed " << r.first->second.name() << " in chest at " << cx << ", " << cy << "\n";

      return;
    }
  }
}

void Cave::place_entrance() {
  std::uniform_int_distribution<int> u(0, SIZE);

  int cx = u(rand_);
  int cy = u(rand_);

  if (get_tile(cx, cy) == Tile::DIRT) {
    while (get_tile(cx, cy) == Tile::DIRT) {
      --cy;
    }
    add_overlay(cx, cy + 1, Tile::ENTRANCE);
    player->set_position(cx, cy + 1);
  } else {
    while (get_tile(cx, cy) == Tile::WALL) {
      ++cy;
      if (cy > SIZE) {
        place_entrance();
        return;
      }
    }
    add_overlay(cx, cy - 1, Tile::ENTRANCE);
    player->set_position(cx, cy - 1);
  }
}
