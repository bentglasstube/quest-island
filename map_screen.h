#pragma once

#include "screen.h"

#include <set>
#include <memory>

#include "character.h"
#include "dialog.h"
#include "item.h"
#include "map.h"
#include "pause_menu.h"

class MapScreen : public Screen {
  public:

    void init();
    bool update(const Input& input, Audio&, unsigned int elapsed);
    void draw(Graphics& graphics) const;

    Screen* next_screen();
    std::string get_music_track() const override;

  private:

    static const int FADE_DURATION = 1000;

    enum class DisplayState {
      PLAYING, FADEOUT, FADEIN
    };

    DisplayState state_;
    int fade_time_, selected_;

    Map *map_, *next_map_;

    std::unique_ptr<Map> island_;
    std::unique_ptr<Character> player_;
    std::unique_ptr<Dialog> dialog_;
    std::unique_ptr<PauseMenu> paused_;

    std::set<Item::Type> inventory_;

    void switch_maps(Map* next_map);
    void handle_chest(Audio& audio, int x, int y);
    bool player_has(Item::Type item) const;
};
