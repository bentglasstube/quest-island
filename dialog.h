#pragma once

#include <memory>
#include <string>

#include "audio.h"
#include "text.h"

class Dialog {

  public:

    Dialog(const std::string& message);

    void update(Audio& audio, unsigned int elapsed);
    void draw(Graphics& graphics, int x, int y) const;
    bool done() const;

  private:

    static const int RATE = 50;

    std::unique_ptr<Text> text_;
    const std::string message_;
    size_t char_;
    int timer_;

};
