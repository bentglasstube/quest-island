#pragma once

#include "screen.h"

#include <memory>

#include "cave.h"
#include "character.h"
#include "map.h"

class MapScreen : public Screen {
  public:

    void init();
    bool update(const Input& input, Audio&, unsigned int elapsed);
    void draw(Graphics& graphics) const;

    Screen* next_screen();

  private:

    Map* map_;

    std::unique_ptr<Map> island_;
    std::unique_ptr<Character> player_;
};
