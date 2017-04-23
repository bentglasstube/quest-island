#include "map.h"

#include <sstream>

Map::Map() {
  text_.reset(new Text("text.png"));
  sprites_.reset(new SpriteMap("map.png", 8, TILE_SIZE, TILE_SIZE));
}

void Map::update(unsigned int elapsed) {
  player_->update(elapsed);
  for (auto i = npcs_.begin(); i < npcs_.end(); ++i) {
    (*i).update(elapsed);
  }
}

void Map::draw(Graphics& graphics, int x, int y, int w, int h, int radius) const {
  int tw = (w / TILE_SIZE) + 1;
  int th = (h / TILE_SIZE) + 1;

  auto pos = player_->position();
  const int sx = pos.first / TILE_SIZE - tw / 2;
  const int sy = pos.second / TILE_SIZE - th / 2;

  for (int iy = 0; iy < th; ++iy) {
    for (int ix = 0; ix < tw; ++ix) {
      const int t = static_cast<int>(get_tile(ix + sx, iy + sy));
      sprites_->draw(graphics, t, x + TILE_SIZE * ix, y + TILE_SIZE * iy);
    }
  }

  player_->draw(graphics, x + w / 2, y + h / 2);

  std::ostringstream info;
  info << "(" << pos.first << ", " << pos.second << ")";
  text_->draw(graphics, info.str(), 0, 0);
}

void Map::move_player(Character::Facing direction) {
  auto pos = player_->position();
  int px = pos.first / TILE_SIZE;
  int py = pos.second / TILE_SIZE;

  switch (direction) {
    case Character::Facing::UP:
      py--;
      break;

    case Character::Facing::DOWN:
      py++;
      break;

    case Character::Facing::LEFT:
      px--;
      break;

    case Character::Facing::RIGHT:
      px++;
      break;
  }

  player_->move(direction);
  if (!walkable(px, py)) {
    player_->stop();
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
      // TODO check for player and npcs

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
