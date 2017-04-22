#pragma once

#include <map>
#include <memory>
#include <random>
#include <string>
#include <utility>

#include "graphics.h"
#include "spritemap.h"

class Map {
  public:
    Map();

    enum class Tile { WATER, SAND, GRASS, TREES, SWAMP, MOUNTAINS, TOWN, CAVE };

    void draw(Graphics& graphics, int x, int y, int mx, int my, int mw, int mh) const;

    virtual Tile get_tile(int x, int y) const = 0;
    bool walkable(int x, int y) const;

    void generate();
    virtual void generate(unsigned int seed) = 0;

  protected:
    std::default_random_engine rand_;
    std::map<std::pair<int, int>, Tile> overlays_;

    void add_overlay(int x, int y, Tile tile);

  private:

    const int TILE_SIZE = 8;

    std::unique_ptr<SpriteMap> sprites_;

};
