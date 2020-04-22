#include "title_screen.h"

#include "map_screen.h"

TitleScreen::TitleScreen() : text_("text.png"), backdrop_("title.png") {}

bool TitleScreen::update(const Input& input, Audio&, unsigned int) {
  return !input.any_pressed();
}

void TitleScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);
  text_.draw(graphics, "Press any key", 128, 208, Text::Alignment::Center);
}

Screen* TitleScreen::next_screen() const {
  return new MapScreen();
}

std::string TitleScreen::get_music_track() const {
  return "island.ogg";
}
