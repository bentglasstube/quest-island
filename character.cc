#include "character.h"

Character::Character(Role role, float x, float y) : role_(role), facing_(Facing::DOWN), moving_(false), x_(x), y_(y), wait_(0) {
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

      fprintf(stderr, "Player moving %u, now at %d, %d\n", static_cast<int>(facing_), x_, y_);
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

std::pair<float, float> Character::position() const {
  return std::make_pair(x_, y_);
}

void Character::set_position(float x, float y) {
  x_ = x;
  y_ = y;
}
