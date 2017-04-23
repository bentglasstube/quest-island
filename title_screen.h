#pragma once

#include <memory>

#include "audio.h"
#include "backdrop.h"
#include "graphics.h"
#include "input.h"
#include "screen.h"
#include "text.h"

class TitleScreen : public Screen {
  public:

    void init();
    bool update(const Input& input, Audio& audio, unsigned int elapsed);
    void draw(Graphics& graphics) const;

    Screen* next_screen();
    std::string get_music_track() const;

  private:

    std::unique_ptr<Text> text_;
    std::unique_ptr<Backdrop> backdrop_;
};
