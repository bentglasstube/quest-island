#pragma once

#include "character.h"

class NPC : public Character {
  public:

    NPC(Role role, int x, int y);

    void update(const Map& map, unsigned int elapsed) override;

  private:

    int x_, y_;
};
