#pragma once

#include <memory>

#include "graphics.h"
#include "spritemap.h"

class Character {
  public:

    enum class Facing { DOWN, UP, RIGHT, LEFT };
    enum class Role { WIZARD, KNIGHT, PEASANT, PRINCESS };

    Character(Role role);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics, int x, int y) const;

    bool waiting() const;

    void set_facing(Facing facing);
    void add_wait(int wait);

  private:

    Role role_;
    Facing facing_;
    std::unique_ptr<SpriteMap> sprites_;
    int wait_;
};
