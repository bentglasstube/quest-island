#include "item.h"

Item::Item(Type type) : type_(type) {
  sprites_.reset(new SpriteMap("item.png", 4, 8, 8));
}

void Item::draw(Graphics& graphics, int x, int y) const {
  sprites_->draw(graphics, static_cast<int>(type_), x, y);
}

Item::Type Item::type() const {
  return type_;
}

std::string Item::name() const {
  switch (type_) {
    case Type::AXE:
      return "Axe";

    case Type::ROD:
      return "Fishing Rod";

    case Type::FISH:
      return "Fresh Fish";

    case Type::LOG:
      return "Wood Log";

    case Type::TORCH:
      return "Torch";

    case Type::KEY:
      return "Skeleton Key";

    case Type::BOOK:
      return "Runed Book";

    case Type::SKULL:
      return "Human Skull";

    case Type::CROWN:
      return "Jeweled Crown";

    case Type::FRESH:
      return "Air Freshener";

    case Type::CHICKEN:
      return "Spicy Chicken";

    case Type::POTION:
      return "Strange Potion";

    case Type::RING:
      return "Ring";

    case Type::BAG:
      return "Heavy Bag";

    case Type::SWORD:
      return "Sword";

    case Type::SHIELD:
      return "Shield";
  }
}

std::string Item::description() const {
  switch (type_) {
    case Type::AXE:
      return "A shoddy axe, but it is\nprobably adequate for\nchopping wood.";

    case Type::ROD:
      return "A simple rod for catching\nfish.  Useful when you\nneed a break from\ngrueling adventures.";

    case Type::FISH:
      return "This fish is so fresh it's\nstill wriggling around.";

    case Type::LOG:
      return "A round piece of wood,\nwhat more is there to\nsay about it?";

    case Type::TORCH:
      return "A magical torch that never\ngoes out and also doesn't\nburn the other items\nin your pack.";

    case Type::KEY:
      return "A key of unknown use.  It\nwill probably disappear\nwhen you use it once.";

    case Type::BOOK:
      return "A heavy tome covered in\nsymbols you cannot read.";

    case Type::SKULL:
      return "Wow this is creepy!";

    case Type::CROWN:
      return "A fancy crown that would\nlikely fetch a nice price.";

    case Type::FRESH:
      return "This air freshener smells\nkinda funny, like it\nmight be poisonous.";

    case Type::CHICKEN:
      return "A leg of chicken with so\nmuch hot sauce it somehow\nhasn't spoiled.";

    case Type::POTION:
      return "It's very bubbly, but I\nwouldn't drink it if I\nwere you.";

    case Type::RING:
      return "A plain ring that almost\ndefinitely won't turn\nyou evil if you wear it...";

    case Type::BAG:
      return "This thing weighs a ton!\nIt's probably full of\ngold or something.";

    case Type::SWORD:
      return "Stab things with the\npointy end.";

    case Type::SHIELD:
      return "A garden variety shield.\nThe cross painted on it\nsomehow slipped by the\nlocalization team.";
  }
}
