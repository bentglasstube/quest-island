#include "character.h"

Character::Character(Role role, int x, int y) : role_(role), facing_(Facing::DOWN), moving_(false), x_(x), y_(y), wait_(0) {
  sprites_.reset(new SpriteMap("character.png", 4, 8, 8));
}

void Character::update(unsigned int elapsed) {
  wait_ -= elapsed;
  if (wait_ <= 0) {
    wait_ = 0;
    if (moving_) {
      // TODO smooth movement
      switch (facing_) {
        case Facing::UP:    --y_; break;
        case Facing::DOWN:  ++y_; break;
        case Facing::LEFT:  --x_; break;
        case Facing::RIGHT: ++x_; break;
      }
    }
  }
}

void Character::draw(Graphics& graphics, int x, int y) const {
  const int n = static_cast<int>(role_) * 4 + static_cast<int>(facing_);
  sprites_->draw(graphics, n, x, y);
}

bool Character::waiting() const {
  return wait_ > 0;
}

bool Character::moving()  const {
  return moving_;
}

Character::Facing Character::facing() const {
  return facing_;
}

void Character::move(Facing facing) {
  facing_ = facing;
  moving_ = true;
}

void Character::stop() {
  moving_ = false;
}

void Character::add_wait(int wait) {
  wait_ += wait;
}

std::pair<int, int> Character::position() const {
  return std::make_pair(x_, y_);
}

void Character::set_position(int x, int y) {
  x_ = x;
  y_ = y;
}

std::string Character::name() const {
  switch (role_) {
    case Role::WIZARD:   return "wizard";
    case Role::KNIGHT:   return "knight";
    case Role::PEASANT:  return "peasant";
    case Role::PRINCESS: return "princess";
  }
}
