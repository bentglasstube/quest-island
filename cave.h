#pragma once

#include <functional>
#include <set>

#include "item.h"
#include "map.h"

class Cave : public Map {
  public:

    Tile get_tile(int x, int y) const override;
    void generate(unsigned int seed) override;
    void place_chest(Item::Type treasure);
    std::string music_type() const override;

    float visibility() const override;
    Item* open_chest(int x, int y) override;


  private:

    static const int SIZE = 64;

    void iterate(std::function<bool(int, int)> selector);
    int walls_within(int x, int y, int r) const;
    void place_entrance();

    Tile data_[SIZE][SIZE];

    std::map<std::pair<int, int>, Item> chests_;

};
