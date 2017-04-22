#pragma once

#include <memory>

#include "graphics.h"
#include "spritemap.h"

class Character {
  public:

    enum class Facing { DOWN, UP, RIGHT, LEFT };
    enum class Role { WIZARD, KNIGHT, PEASANT };

    Character(Role role);

    void draw(Graphics& graphics, int x, int y) const;

    void set_facing(Facing facing);

  private:

    Role role_;
    Facing facing_;
    std::unique_ptr<SpriteMap> sprites_;
};
