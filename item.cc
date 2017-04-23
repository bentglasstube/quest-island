#include "item.h"

Item::Item(Type type) : type_(type) {
  sprites_.reset(new SpriteMap("item.png", 4, 8, 8));
}

void Item::draw(Graphics& graphics, int x, int y) const {
  sprites_->draw(graphics, static_cast<int>(type_), x, y);
}

std::string Item::name() const {
  switch (type_) {
    case Type::AXE:
      return "Axe";

    case Type::FISHROD:
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
      return "A shoddy axe, but it is probably\ngood for chopping wood.";

    case Type::FISHROD:
      return "A simple rod for catching fish.\nUseful when you need a break\nfrom adventures.";

    case Type::FISH:
      return "This fish is so fresh it's still\nwriggling around.";

    case Type::LOG:
      return "A round piece of wood, what more\nis there to say?";

    case Type::TORCH:
      return "A magical torch that never goes\nout and also doesn't burn the\nother items in your pack.";

    case Type::KEY:
      return "A key of unknown use.  It will\nprobably disappear when you use it.";

    case Type::BOOK:
      return "A heavy tome with all manner of\nsymbols you cannot read.";

    case Type::SKULL:
      return "Wow this is creepy!";

    case Type::CROWN:
      return "A fancy crown that would probably\nfetch a nice price.";

    case Type::FRESH:
      return "This air freshener smells kinda\nfunny, like it might be poisonous.";

    case Type::CHICKEN:
      return "A leg of chicken with so much\nhot sauce it hasn't spoiled.";

    case Type::POTION:
      return "I wouldn't drink it if I were you.";

    case Type::RING:
      return "A plain ring that definitely\nwon't turn you evil if you wear it...";

    case Type::BAG:
      return "This thing weighs a ton!\nIt's probably full of gold.";

    case Type::SWORD:
      return "Stab things with the pointy end.";

    case Type::SHIELD:
      return "A garden variety shield.  The\ncross painted on it somehow slipped\nby the translators.";
  }
}
