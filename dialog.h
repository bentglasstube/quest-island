#pragma once

#include <memory>
#include <string>
#include <vector>

#include "spritemap.h"
#include "text.h"

class Dialog {

  public:

    Dialog();

    void add_page(const std::string& text);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics, int x, int y, int w, int h) const;

    void advance();
    bool page_done() const;
    bool all_done() const;

  private:

    static const int RATE = 100;

    std::unique_ptr<SpriteMap> sprites_;
    std::unique_ptr<Text> text_;

    std::vector<std::string> pages_;
    int page_, char_, timer_;

};
