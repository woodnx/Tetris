#pragma once
#include "BlockId.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

using namespace std;

class Bag {
  public:
    inline static int const TOTAL_MINOS = 7;
    inline static int const BAG_SIZE    = TOTAL_MINOS * 2;
    inline static int const NEXT_REFERS = 6;
    Bag();
    shared_ptr<BlockId> increase();
    shared_ptr<BlockId> getMinoId(int index);

  private:
    vector<shared_ptr<BlockId>> bag;
    vector<shared_ptr<BlockId>> generateBlockIds();
    vector<shared_ptr<BlockId>> generateShuffledNexts();
    int random(int);
    int countNexts();
    void joinNexts(int);
};
