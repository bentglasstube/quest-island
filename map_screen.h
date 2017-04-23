#pragma once

#include "screen.h"

#include <memory>

#include "cave.h"
#include "character.h"
#include "dialog.h"
#include "map.h"

class MapScreen : public Screen {
  public:

    void init();
    bool update(const Input& input, Audio&, unsigned int elapsed);
    void draw(Graphics& graphics) const;

    Screen* next_screen();
    std::string get_music_track() const override;

  private:

    static const int FADE_DURATION = 2000;

    enum class DisplayState {
      PLAYING, FADEOUT, FADEIN
    };

    DisplayState state_;
    int fade_time_;

    Map *map_, *next_map_;

    std::unique_ptr<Map> island_;
    std::unique_ptr<Character> player_;
    std::unique_ptr<Dialog> dialog_;

    void switch_maps(Map* next_map);
};
