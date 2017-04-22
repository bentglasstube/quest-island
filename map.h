#pragma once

#include <memory>
#include <string>

#include "graphics.h"
#include "spritemap.h"

class Map {
  public:
    Map(int seed);

    enum class Tile { WATER, SAND, GRASS, TREES, SWAMP, MOUNTAINS, TOWN, CAVE };

    void draw(Graphics& graphics, int x, int y, int mx, int my, int mw, int mh) const;
    Tile get_tile(int x, int y) const;
    bool walkable(int x, int y) const;

  private:

    const int TILE_SIZE = 8;
    const float NOISE_ZOOM = 0.03f;

    std::unique_ptr<SpriteMap> sprites_;
    float seed_;

    float elevation(int x, int y) const;
    float moisture(int x, int y) const;
    float noise(int x, int y, int z) const;

};
