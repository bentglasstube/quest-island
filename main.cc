#include "game.h"

#include "title_screen.h"

int main(int, char**) {
  Game::Config config;

  config.graphics.title = "Ludum Dare 38";
  config.graphics.width = 256;
  config.graphics.height = 240;
  /* config.graphics.width = 1024; */
  /* config.graphics.height = 960; */
  config.graphics.fullscreen = true;

  TitleScreen *s = new TitleScreen();

  Game game(config);
  game.loop(s);

  return 0;
}
