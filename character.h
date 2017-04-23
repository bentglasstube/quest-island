#pragma once

#include <memory>

#include "graphics.h"
#include "map.h"
#include "spritemap.h"

class Character {
  public:

    enum class Facing { DOWN, UP, RIGHT, LEFT };
    enum class Role { WIZARD, KNIGHT, PEASANT, PRINCESS };

    Character(Role role);

    virtual void update(const Map& map, unsigned int elapsed);
    void draw(Graphics& graphics, int x, int y) const;

    bool waiting() const;

    void set_facing(Facing facing);
    void add_wait(int wait);

  protected:

    Role role_;
    Facing facing_;

  private:
    std::unique_ptr<SpriteMap> sprites_;
    int wait_;
};
