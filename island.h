#pragma once

#include <set>

#include "cave.h"
#include "item.h"
#include "map.h"

class Island : public Map {
  public:

    Tile get_tile(int x, int y) const override;
    void generate(unsigned int seed) override;
    std::string music_type() const override;

    Map* get_cave(int x, int y) override;

  private:

    const float NOISE_ZOOM = 0.03f;
    const int PLACEMENT_DIST = 100;

    float elevation_z_, moisture_z_;
    std::map<std::pair<int, int>, Cave> caves_;

    float elevation(int x, int y) const;
    float moisture(int x, int y) const;
    float noise(int x, int y, int z) const;

    void place_cave();
    void place_town();
    Character* place_npc(Character::Role role);

    template <class T> T random_from_set(std::set<T> items);
};
