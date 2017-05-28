#pragma once

#include <memory>

#include "graphics.h"
#include "item.h"
#include "spritemap.h"

class Character {
  public:

    enum class Facing { DOWN, UP, RIGHT, LEFT };
    enum class Role { WIZARD, KNIGHT, PEASANT, PRINCESS };
    enum class QuestState { NEW, WAITING, COMPLETE };

    Character(Role role, int x, int y);

    virtual void update(unsigned int elapsed);
    void draw(Graphics& graphics, int x, int y) const;

    bool waiting() const;
    bool moving() const;
    Facing facing() const;
    QuestState state() const;

    void move(Facing facing);
    void stop();
    void add_wait(int wait);
    void next_quest_state();

    std::pair<int, int> position() const;
    void set_position(int x, int y);

    std::string name() const;
    std::string quest_hint() const;

    Item::Type wants, gift;

  private:

    std::unique_ptr<SpriteMap> sprites_;
    Role role_;
    Facing facing_;
    QuestState state_;
    bool moving_;
    int x_, y_, wait_;
};
