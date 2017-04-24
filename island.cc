#include "island.h"

#include <iostream>

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

  player.reset(new Character(Character::Role::PEASANT, 0, 0));

  std::uniform_real_distribution<float> u(-100, 100);
  elevation_z_ = u(rand_);
  moisture_z_ = u(rand_);

  std::uniform_int_distribution<int> n(1, 3);

  std::set<Item::Type> treasures = {
    Item::Type::AXE,
    Item::Type::ROD,
    Item::Type::TORCH,
    Item::Type::KEY,
    Item::Type::BOOK,
    Item::Type::SKULL,
    Item::Type::CROWN,
    Item::Type::FRESH,
    Item::Type::CHICKEN,
    Item::Type::POTION,
    Item::Type::RING,
    Item::Type::BAG,
    Item::Type::SWORD,
    Item::Type::SHIELD,
  };

  std::set<Item::Type> needs;

  const int caves = n(rand_);
  for (int i = 0; i < caves; ++i) {
    place_cave();
  }

  const int towns = n(rand_);
  for (int i = 0; i < towns; ++i) {
    place_town();
  }

  std::set<Item::Type> wizard_wants = {
    Item::Type::LOG,
    Item::Type::FISH,
    Item::Type::KEY,
    Item::Type::BOOK,
    Item::Type::SKULL,
    Item::Type::CHICKEN,
    Item::Type::POTION,
    Item::Type::BAG,
  };

  std::set<Item::Type> knight_wants = {
    Item::Type::LOG,
    Item::Type::FISH,
    Item::Type::KEY,
    Item::Type::CHICKEN,
    Item::Type::BAG,
    Item::Type::SWORD,
    Item::Type::SHIELD,
  };

  std::set<Item::Type> princess_wants = {
    Item::Type::LOG,
    Item::Type::FISH,
    Item::Type::KEY,
    Item::Type::CROWN,
    Item::Type::CHICKEN,
    Item::Type::RING,
    Item::Type::BAG,
  };

  Character* wizard   = place_npc(Character::Role::WIZARD);
  Character* knight   = place_npc(Character::Role::KNIGHT);
  Character* princess = place_npc(Character::Role::PRINCESS);

  set_npc_want(wizard, wizard_wants);

  knight_wants.erase(wizard->wants);
  princess_wants.erase(wizard->wants);

  set_npc_want(knight, knight_wants);

  princess_wants.erase(knight->wants);

  set_npc_want(princess, princess_wants);

  needs.insert(wizard->wants);
  needs.insert(knight->wants);
  needs.insert(princess->wants);

  auto fish = needs.find(Item::Type::FISH);
  if (fish != needs.end()) {
    needs.erase(fish);
    needs.insert(Item::Type::ROD);
  }

  auto wood = needs.find(Item::Type::LOG);
  if (wood != needs.end()) {
    needs.erase(wood);
    needs.insert(Item::Type::AXE);
  }

  std::cerr << "Generated island #" << seed << "\n";
  std::cerr << caves << " caves\n";
  std::cerr << towns << " towns\n";

  auto cave = caves_.begin();
  for (auto i = needs.begin(); i != needs.end(); ++i) {
    cave->second.place_chest(*i);

    cave++;
    if (cave == caves_.end()) cave = caves_.begin();
  }
}

std::string Island::music_type() const {
  return "island.ogg";
}

Map* Island::get_cave(int x, int y) {
  auto cave = caves_.find(std::make_pair(x, y));
  if (cave == caves_.end()) {
    return NULL;
  } else {
    return &(cave->second);
  }
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
      auto r = caves_.emplace(std::make_pair(cx, cy - 1), Cave{});
      if (r.second) r.first->second.generate(rand_());

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

Character* Island::place_npc(Character::Role role) {
  std::normal_distribution<float> normal(0, 100);

  while (true) {
    int cx = normal(rand_);
    int cy = normal(rand_);

    if (walkable(cx, cy)) {
      return add_npc(role, cx, cy);
    }
  }
}

Item::Type Island::set_npc_want(Character* npc, std::set<Item::Type> wants) {
  std::uniform_int_distribution<int> uni(0, wants.size() - 1);
  auto i = wants.begin();
  std::advance(i, uni(rand_));

  return npc->wants = *i;
}
