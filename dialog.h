#pragma once

#include <memory>
#include <random>
#include <string>
#include <vector>

#include "audio.h"
#include "spritemap.h"
#include "text.h"

class Dialog {

  public:

    Dialog();

    void add_page(const std::string& text);

    void update(Audio& audio, unsigned int elapsed);
    void draw(Graphics& graphics, int x, int y, int w, int h) const;

    void advance();
    bool page_done() const;
    bool all_done() const;

  private:

    static const int RATE = 100;

    std::unique_ptr<SpriteMap> sprites_;
    std::unique_ptr<Text> text_;

    std::default_random_engine engine_;

    std::vector<std::string> pages_;
    int page_, char_, timer_;

};
