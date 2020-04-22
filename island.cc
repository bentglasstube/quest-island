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

  // FIXME terrible design
  npcs_.reserve(3); // pre-allocate vector so pointers remain valid
  Character* wizard   = place_npc(Character::Role::WIZARD);
  Character* knight   = place_npc(Character::Role::KNIGHT);
  Character* princess = place_npc(Character::Role::PRINCESS);

  wizard->wants = random_from_set(wizard_wants);

  knight_wants.erase(wizard->wants);
  princess_wants.erase(wizard->wants);

  knight->wants = random_from_set(knight_wants);

  princess_wants.erase(knight->wants);

  princess->wants = random_from_set(princess_wants);

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

  needs.insert(Item::Type::TORCH);

  for (auto i = needs.begin(); i != needs.end(); ++i) {
    treasures.erase(*i);
  }

  for (int i = 0; i < 3; ++i) {
    const Item::Type item = random_from_set(treasures);
    needs.insert(item);
    treasures.erase(item);
  }

  needs.erase(wizard->wants);
  wizard->gift = random_from_set(needs);
  needs.erase(wizard->gift);

  needs.erase(knight->wants);
  knight->gift = random_from_set(needs);
  needs.erase(knight->gift);

  needs.erase(princess->wants);
  princess->gift = random_from_set(needs);
  needs.erase(princess->gift);

  needs.insert(wizard->wants);
  needs.insert(knight->wants);
  needs.insert(princess->wants);

  auto fish2 = needs.find(Item::Type::FISH);
  if (fish2 != needs.end()) {
    needs.erase(fish2);
    needs.insert(Item::Type::ROD);
  }

  auto wood2 = needs.find(Item::Type::LOG);
  if (wood2 != needs.end()) {
    needs.erase(wood2);
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
  std::normal_distribution<float> normal(0, PLACEMENT_DIST);

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
  std::normal_distribution<float> normal(0, PLACEMENT_DIST);

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
  std::normal_distribution<float> normal(0, PLACEMENT_DIST);

  while (true) {
    int cx = normal(rand_);
    int cy = normal(rand_);

    if (walkable(cx, cy)) {
      return add_npc(role, cx, cy);
    }
  }
}

template <class T> T Island::random_from_set(std::set<T> items) {
  std::uniform_int_distribution<int> uni(0, items.size() - 1);
  auto i = items.begin();
  std::advance(i, uni(rand_));
  return *i;
}
