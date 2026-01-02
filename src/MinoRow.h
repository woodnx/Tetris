#pragma once
#include "BlockId.h"
#include "DxLib.h"

#include <memory>
#include <vector>

#define NEXT_REFER_SIZE 6
#define ALL_MINO_NUM    7

using namespace std;

class MinoRow {
  public:
    MinoRow();
    void initialize();
    int increase();
    shared_ptr<BlockId> getMinoNum(int refer_num);

  private:
    void initRow(vector<shared_ptr<BlockId>> &);
    void shuffleRow(vector<shared_ptr<BlockId>> &);
    vector<shared_ptr<BlockId>> mino_arr;
};
