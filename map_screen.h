#pragma once

#include "screen.h"

#include "map.h"

class MapScreen : public Screen {
  public:

    void init();
    bool update(const Input& input, Audio&, unsigned int elapsed);
    void draw(Graphics& graphics) const;

    Screen* next_screen();

  private:

    std::unique_ptr<Map> map_;
    int ox, oy;
};
