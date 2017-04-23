#include "map.h"

Map::Map() {
  text_.reset(new Text("text.png"));
  sprites_.reset(new SpriteMap("map.png", 8, TILE_SIZE, TILE_SIZE));
}

void Map::update(unsigned int elapsed) {
  player_->update(elapsed);

  std::uniform_int_distribution<int> r(0, 3);

  for (auto i = npcs_.begin(); i < npcs_.end(); ++i) {
    Character& npc = (*i);

    auto pos = npc.position();
    if (!npc.waiting()) {
      Character::Facing dir = static_cast<Character::Facing>(r(rand_));
      npc.add_wait(5 * tile_delay(pos.first, pos.second));
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

  auto pos = player_->position();
  const int sx = pos.first - tw / 2;
  const int sy = pos.second - th / 2;

  for (int iy = 0; iy < th; ++iy) {
    const int dy = iy + sy - pos.second;
    for (int ix = 0; ix < tw; ++ix) {
      const int dx = ix + sx - pos.first;
      const int d = dx * dx + dy * dy;
      if (d <= radius * radius) {
        const int t = static_cast<int>(get_tile(ix + sx, iy + sy));
        sprites_->draw(graphics, t, x + TILE_SIZE * ix, y + TILE_SIZE * iy);
      }
    }
  }

  player_->draw(graphics, x + w / 2, y + h / 2);

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
  auto pos = player_->position();

  if (!player_->waiting()) {
    player_->move(direction);
    if (walkable(pos.first, pos.second, direction)) {
      player_->add_wait(tile_delay(pos.first, pos.second));
    } else {
      player_->stop();
    }
  }
}

void Map::stop_player() {
  player_->stop();
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
      auto pp = player_->position();
      if (x == pp.first && y == pp.second) return false;

      for (auto i = npcs_.begin(); i < npcs_.end(); ++i) {
        auto np = (*i).position();
        if (x == np.first && y == np.second) return false;
      }

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
  }
  return 100;
}

float Map::visibility() const {
  return 1.0f;
}

void Map::add_overlay(int x, int y, Tile tile) {
  overlays_.insert({ std::make_pair(x, y), tile });
}

void Map::add_npc(Character::Role role, int x, int y) {
  npcs_.emplace_back(role, x, y);
}

void Map::generate() {
  std::random_device r;
  generate(r());
}
