#pragma once

#include "screen.h"

#include "character.h"
#include "map.h"

class MapScreen : public Screen {
  public:

    void init();
    bool update(const Input& input, Audio&, unsigned int elapsed);
    void draw(Graphics& graphics) const;

    Screen* next_screen();

  private:

    std::unique_ptr<Map> map_;
    std::unique_ptr<Character> player_;
    float px_, py_;
    int timer_;
};
