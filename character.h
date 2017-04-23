#pragma once

#include <memory>

#include "graphics.h"
#include "spritemap.h"

class Character {
  public:

    enum class Facing { DOWN, UP, RIGHT, LEFT };
    enum class Role { WIZARD, KNIGHT, PEASANT, PRINCESS };

    Character(Role role, int x, int y);

    virtual void update(unsigned int elapsed);
    void draw(Graphics& graphics, int x, int y) const;

    bool waiting() const;
    bool moving() const;
    Facing facing() const;

    void move(Facing facing);
    void stop();
    void add_wait(int wait);

    std::pair<int, int> position() const;
    void set_position(int x, int y);

  private:

    std::unique_ptr<SpriteMap> sprites_;
    Role role_;
    Facing facing_;
    bool moving_;
    int x_, y_, wait_;
};
