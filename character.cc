#include "character.h"

Character::Character(Role role) : role_(role), facing_(Facing::DOWN) {
  sprites_.reset(new SpriteMap("character.png", 4, 8, 8));
}

void Character::draw(Graphics& graphics, int x, int y) const {
  const int n = static_cast<int>(role_) * 4 + static_cast<int>(facing_);
  sprites_->draw(graphics, n, x, y);
}

void Character::set_facing(Facing facing) {
  facing_ = facing;
}

