#include "npc.h"

#include <random>

NPC::NPC(Role role, int x, int y) : Character(role), x_(x), y_(y) {}

void NPC::update(const Map& map, unsigned int elapsed) {
  Character::update(map, elapsed);

  if (!waiting()) {
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> u(0, 3);

    int tx = x_, ty = y_;

    const int dir = u(e);
    switch (dir) {
      case 0:
        facing_ = Facing::UP;
        ty = y_ - 1;
        break;
      case 1:
        facing_ = Facing::LEFT;
        tx = x_ - 1;
        break;
      case 2:
        facing_ = Facing::DOWN;
        ty = y_ + 1;
        break;
      case 3:
        facing_ = Facing::RIGHT;
        tx = x_ + 1;
        break;
    }

    if (map.walkable(tx, ty)) {
      x_ = tx;
      y_ = ty;
    }

    add_wait(1000);
  }
}
