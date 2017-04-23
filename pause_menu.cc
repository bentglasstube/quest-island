#include "pause_menu.h"

PauseMenu::PauseMenu(std::set<Item::Type> inventory) : inventory_(inventory) {
  sprites_.reset(new SpriteMap("box.png", 3, 8, 8));
  text_.reset(new Text("text.png"));
}

void PauseMenu::update(const Input& input, Audio&) {
  if (inventory_.size() > 1) {
    if (input.key_pressed(SDL_SCANCODE_A)) --selected_;
    if (input.key_pressed(SDL_SCANCODE_D)) ++selected_;

    selected_ %= inventory_.size();
  }
}

void PauseMenu::draw(Graphics& graphics, int x, int y) const {
  const int w = graphics.width() - 32;
  const int h = 80;

  sprites_->draw(graphics, 0, x, y);
  for (int iy = y + 8; iy < y + h - 8; iy += 8) {
    sprites_->draw(graphics, 3, x, iy);
  }
  sprites_->draw(graphics, 6, x, y + h - 8);

  for (int ix = x + 8; ix < x + w - 8; ix += 8) {
    sprites_->draw(graphics, 1, ix, y);
    for (int iy = y + 8; iy < y + h - 8; iy += 8) {
      sprites_->draw(graphics, 4, ix, iy);
    }
    sprites_->draw(graphics, 7, ix, y + h - 8);
  }

  sprites_->draw(graphics, 2, x + w - 8, y);
  for (int iy = y + 8; iy < y + h - 8; iy += 8) {
    sprites_->draw(graphics, 5, x + w - 8, iy);
  }
  sprites_->draw(graphics, 8, x + w - 8, y + h - 8);

  text_->draw(graphics, "- Inventory -", x + w / 2, y + 8, Text::Alignment::CENTER);

  int n = 0;
  for (auto i = inventory_.begin(); i != inventory_.end(); ++i) {
    const int ix = x + 24 + (n % 8) * 24;
    const int iy = y + 32 + (n / 8) * 24;

    Item item(*i);
    item.draw(graphics, ix, iy);

    ++n;
  }

  if (inventory_.size() > 0) {
    const int sx = x + 24 + (selected_ % 8) * 24;
    const int sy = y + 32 + (selected_ / 8) * 24;

    sprites_->draw(graphics,  9, sx - 4, sy - 4);
    sprites_->draw(graphics, 10, sx + 4, sy - 4);
    sprites_->draw(graphics, 12, sx - 4, sy + 4);
    sprites_->draw(graphics, 13, sx + 4, sy + 4);

    auto i = inventory_.begin();
    std::advance(i, selected_);
    Item item(*i);

    text_->draw(graphics, item.name(), x + w / 2, y + 80, Text::Alignment::CENTER);
    text_->draw(graphics, item.description(), x + 8, y + 96);
  }
}
