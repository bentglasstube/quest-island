#pragma once

#include <memory>
#include <set>
#include <string>

#include "audio.h"
#include "graphics.h"
#include "input.h"
#include "spritemap.h"
#include "text.h"

#include "item.h"

class PauseMenu {
  public:

    PauseMenu(std::set<Item::Type> inventory);

    void update(const Input& input, Audio& audio);
    void draw(Graphics& graphics, int x, int y) const;

  private:

    std::unique_ptr<SpriteMap> sprites_;
    std::unique_ptr<Text> text_;
    std::set<Item::Type> inventory_;

    int selected_;

};
