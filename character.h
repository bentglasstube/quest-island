#pragma once

#include <memory>

#include "graphics.h"
#include "spritemap.h"

class Character {
  public:

    enum class Facing { DOWN, UP, RIGHT, LEFT };
    enum class Role { WIZARD, KNIGHT, PEASANT, PRINCESS };

    Character(Role role, float x, float y);

    virtual void update(unsigned int elapsed);
    void draw(Graphics& graphics, int x, int y) const;

    bool waiting() const;

    void move(Facing facing);
    void stop();
    void add_wait(int wait);

    std::pair<float, float> position() const;
    void set_position(float x, float y);

  private:

    std::unique_ptr<SpriteMap> sprites_;
    Role role_;
    Facing facing_;
    bool moving_;
    float x_, y_;
    int wait_;
};
