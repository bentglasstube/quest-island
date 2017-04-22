#include "character.h"

Character::Character(Role role) : role_(role), facing_(Facing::DOWN), wait_(0) {
  sprites_.reset(new SpriteMap("character.png", 4, 8, 8));
}

void Character::update(unsigned int elapsed) {
  wait_ -= elapsed;
  if (wait_ < 0) wait_ = 0;
}

void Character::draw(Graphics& graphics, int x, int y) const {
  const int n = static_cast<int>(role_) * 4 + static_cast<int>(facing_);
  sprites_->draw(graphics, n, x, y);
}

bool Character::waiting() const {
  return wait_ > 0;
}

void Character::set_facing(Facing facing) {
  facing_ = facing;
}

void Character::add_wait(int wait) {
  wait_ += wait;
}

