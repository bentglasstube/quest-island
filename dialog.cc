#include "dialog.h"

#include <sstream>

Dialog::Dialog(const std::string& message) : message_(message), char_(0), timer_(0) {
  text_.reset(new Text("text.png"));
}

void Dialog::update(Audio& audio, unsigned int elapsed) {
  if (!done()) {
    timer_ += elapsed;
    if (timer_ > RATE) {
      timer_ -= RATE;
      ++char_;

      switch (message_[char_]) {
        case ' ':
        case '\n':
          break;

        default:
          audio.play_sample("beep3.wav");
          break;
      }
    }
  }
}

void Dialog::draw(Graphics& graphics, int x, int y) const {
  text_->draw(graphics, message_.substr(0, char_), x, y);
}

bool Dialog::done() const {
  return char_ >= message_.length();
}
