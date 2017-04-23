#include "dialog.h"

Dialog::Dialog() : page_(0), char_(0), timer_(0) {
  sprites_.reset(new SpriteMap("dialog.png", 3, 8, 8));
  text_.reset(new Text("text.png"));
}

void Dialog::add_page(const std::string& text) {
  pages_.push_back(text);
}

void Dialog::update(unsigned int elapsed) {
  if (!page_done()) {
    timer_ += elapsed;
    if (timer_ > RATE) {
      timer_ -= RATE;
      ++char_;
    }
  }
}

void Dialog::draw(Graphics& graphics, int x, int y, int w, int h) const {
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

  text_->draw(graphics, pages_[page_].substr(0, char_), x + 12, y + 12);
}

void Dialog::advance() {
  if (page_done()) ++page_;
}

bool Dialog::page_done() const {
  return all_done() || char_ >= pages_[page_].length();
}

bool Dialog::all_done() const {
  return page_ >= pages_.size();
}

