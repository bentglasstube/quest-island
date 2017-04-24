#pragma once

#include <map>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "graphics.h"
#include "item.h"
#include "spritemap.h"
#include "text.h"

#include "character.h"

class Map {
  public:
    Map();

    enum class Tile {
      WATER, SAND, GRASS, TREES, SWAMP, MOUNTAINS, TOWN, CAVE,
      DIRT, ENTRANCE, WALL, CHEST, EMPTY, STUMP,
    };

    std::unique_ptr<Character> player;

    void update(unsigned int elapsed);
    void draw(Graphics& graphics, int x, int y, int w, int h, int radius) const;

    void move_player(Character::Facing direction);

    virtual Tile get_tile(int x, int y) const = 0;
    bool walkable(int x, int y) const;
    bool walkable(int x, int y, Character::Facing direction) const;
    int tile_delay(int x, int y) const;
    virtual float visibility() const;

    void generate();
    virtual void generate(unsigned int seed) = 0;

    const Character* get_npc(int x, int y) const;

    virtual Item* open_chest(int x, int y);
    virtual Map* get_cave(int x, int y);

    virtual std::string music_type() const = 0;

    void add_overlay(int x, int y, Tile tile);

  protected:
    std::default_random_engine rand_;
    std::map<std::pair<int, int>, Tile> overlays_;
    std::vector<Character> npcs_;

    Character* add_npc(Character::Role role, int x, int y);

  private:

    const int TILE_SIZE = 8;

    std::unique_ptr<SpriteMap> sprites_;
    std::unique_ptr<Text> text_;
};
