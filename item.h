#pragma once

#include <string>

#include "graphics.h"
#include "spritemap.h"

class Item {
  public:

    enum class Type {
      AXE, FISHROD, FISH, LOG,
      TORCH, KEY, BOOK, SKULL,
      CROWN, FRESH, CHICKEN, POTION,
      RING, BAG, SWORD, SHIELD,
    };

    Item(Type type);

    void draw(Graphics& graphics, int x, int y) const;

    std::string name() const;
    std::string description() const;

  private:

    std::unique_ptr<SpriteMap> sprites_;
    Type type_;

};
