#pragma once
#include "DxLib.h"

#include <vector>

#define NEXT_REFER_SIZE 6
#define ALL_MINO_NUM    7

class MinoRow {
  public:
    MinoRow();
    void initialize();
    int increase();
    int getMinoNum(int refer_num);

  private:
    void initRow(std::vector<int> &);
    void shuffleRow(std::vector<int> &);
    std::vector<int> mino_arr;
};
