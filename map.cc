#include "map.h"

Map::Map() {
  text_.reset(new Text("text.png"));
  sprites_.reset(new SpriteMap("map.png", 8, TILE_SIZE, TILE_SIZE));
}

void Map::update(unsigned int elapsed) {
  player->update(elapsed);

  std::uniform_int_distribution<int> r(0, 3);

  for (auto i = npcs_.begin(); i < npcs_.end(); ++i) {
    Character& npc = (*i);

    auto pos = npc.position();
    if (!npc.waiting()) {
      Character::Facing dir = static_cast<Character::Facing>(r(rand_));
      npc.add_wait(NPC_SLOW_FACTOR * tile_delay(pos.first, pos.second));
      npc.move(dir);
    }

    if (!walkable(pos.first, pos.second, npc.facing())) {
      npc.stop();
    }

    npc.update(elapsed);
  }
}

void Map::draw(Graphics& graphics, int x, int y, int w, int h, int radius) const {
  int tw = (w / TILE_SIZE) + 1;
  int th = (h / TILE_SIZE) + 1;

  auto pos = player->position();
  const int sx = pos.first - tw / 2;
  const int sy = pos.second - th / 2;

  for (int iy = 0; iy < th; ++iy) {
    const int dy = iy + sy - pos.second;
    for (int ix = 0; ix < tw; ++ix) {
      const int dx = ix + sx - pos.first;
      const int d = dx * dx + dy * dy;
      if (d < radius * radius) {
        const int t = static_cast<int>(get_tile(ix + sx, iy + sy));
        sprites_->draw(graphics, t, x + TILE_SIZE * ix, y + TILE_SIZE * iy);
      }
    }
  }

  player->draw(graphics, x + w / 2, y + h / 2);

  for (auto i = npcs_.begin(); i < npcs_.end(); ++i) {
    auto pos = (*i).position();
    const int nx = (pos.first - sx) * TILE_SIZE;
    const int ny = (pos.second - sy) * TILE_SIZE;

    if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
      (*i).draw(graphics, x + nx, y + ny);
    }
  }
}

void Map::move_player(Character::Facing direction) {
  auto pos = player->position();

  if (!player->waiting()) {
    player->move(direction);
    if (walkable(pos.first, pos.second, direction)) {
      player->add_wait(tile_delay(pos.first, pos.second));
    } else {
      player->stop();
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
    case Tile::EMPTY:
      return false;

    default:
      auto p = player->position();
      if (x == p.first && y == p.second) return false;
      if (get_npc(x, y) != NULL) return false;
      return true;
  }
}

bool Map::walkable(int x, int y, Character::Facing direction) const {
  switch (direction) {
    case Character::Facing::UP:    return walkable(x, y - 1);
    case Character::Facing::DOWN:  return walkable(x, y + 1);
    case Character::Facing::LEFT:  return walkable(x - 1, y);
    case Character::Facing::RIGHT: return walkable(x + 1, y);
  }
  return false;
}

int Map::tile_delay(int x, int y) const {
  const Tile t = get_tile(x, y);
  switch (t) {
    case Tile::SWAMP: return 300;
    case Tile::TREES: return 200;
    default:          return 100;
  }
}

float Map::visibility() const {
  return 1.0f;
}

void Map::add_overlay(int x, int y, Tile tile) {
  overlays_.insert({ std::make_pair(x, y), tile });
}

Character* Map::add_npc(Character::Role role, int x, int y) {
  npcs_.emplace_back(role, x, y);
  return &(npcs_.back());
}

void Map::generate() {
  std::random_device r;
  generate(r());
}

const Character* Map::get_npc(int x, int y) const {
  for (auto i = npcs_.begin(); i < npcs_.end(); ++i) {
    auto p = (*i).position();
    if (x == p.first && y == p.second) return &*i;
  }
  return NULL;
}

Character* Map::get_npc(int x, int y) {
  return const_cast<Character*>(static_cast<const Map*>(this)->get_npc(x, y));
}

Item* Map::open_chest(int, int) {
  return NULL;
}

Map* Map::get_cave(int, int) {
  return NULL;
}
