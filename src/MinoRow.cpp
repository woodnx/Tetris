#include "MinoRow.h"

MinoRow::MinoRow() {
  initialize();
}

void MinoRow::initialize() {
  this->initRow(mino_arr);
  this->shuffleRow(mino_arr);
}

int MinoRow::increase() {
  mino_arr.erase(mino_arr.begin());

  // 配列のサイズがNEXT_REFER_SIZE 以下なら新しくarrを作成し，連結．
  if (mino_arr.size() < NEXT_REFER_SIZE) {
    vector<shared_ptr<BlockId>> add_arr;
    initRow(add_arr);
    shuffleRow(add_arr);
    mino_arr.insert(mino_arr.end(), add_arr.begin(), add_arr.end());
  }

  return 0;
}

shared_ptr<BlockId> MinoRow::getMinoNum(int refer_num) {
  return mino_arr[refer_num];
}

void MinoRow::initRow(vector<shared_ptr<BlockId>>& arr) {
  arr.resize(ALL_MINO_NUM);
  for (int i = 0; i < ALL_MINO_NUM; i++) {
    shared_ptr<BlockId> id = BlockId::from_id(i + 1);
    arr[i]                 = id;
  }
}

void MinoRow::shuffleRow(vector<shared_ptr<BlockId>>& arr) {
  for (int i = ALL_MINO_NUM - 1; i >= 0; i--) {
    int r                   = GetRand(ALL_MINO_NUM - 1);
    shared_ptr<BlockId> tmp = arr[i];
    arr[i]                  = arr[r];
    arr[r]                  = tmp;
  }
}