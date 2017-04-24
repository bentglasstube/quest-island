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

std::string Character::quest_hint() const {
  switch (wants) {
    case Item::Type::FISH:
    case Item::Type::CHICKEN:
      return "I am so hungry.  Do you\nhave any vittles?";

    case Item::Type::LOG:
      return "It's so cold out here.\nI wish I had a campfire.";

    case Item::Type::KEY:
      return "I am locked out of my\nhouse.  Where did I leave it?";

    case Item::Type::BOOK:
      return "I have forgotten all\nof my incantations!";

    case Item::Type::SKULL:
      return "I need something spooky\nfor my magic...";

    case Item::Type::CROWN:
    case Item::Type::RING:
      return "How will anyone know\nhow I am without my jewelry?";

    case Item::Type::POTION:
      return "I'll never find all\nthe ingredients to make\nanother one...";

    case Item::Type::BAG:
      return "I was supposed to wash\nthe laundry, but where\ndid I leave it?";

    case Item::Type::SWORD:
    case Item::Type::SHIELD:
      return "Someone has taken my\nequipment while I was\nasleep!";

    default:
      return "Hello, I am the " + name();
  }
}
