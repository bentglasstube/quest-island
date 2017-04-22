#pragma once

#include <functional>

#include "map.h"

class Cave : public Map {
  public:

    Tile get_tile(int x, int y) const override;
    void generate(unsigned int seed) override;
    float visibility() const override;

  private:

    static const int SIZE = 64;

    void iterate(std::function<bool(int, int)> selector);
    int walls_within(int x, int y, int r) const;
    void place_chest();
    void place_entrance();

    std::pair<int, int> spawn() const override;

    Tile data_[SIZE][SIZE];
    std::pair<int, int> entrance_;

};
