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
    std::vector<int> add_arr;
    initRow(add_arr);
    shuffleRow(add_arr);
    mino_arr.insert(mino_arr.end(), add_arr.begin(), add_arr.end());
  }

  return 0;
}

int MinoRow::getMinoNum(int refer_num) {
  return mino_arr[refer_num];
}

void MinoRow::initRow(std::vector<int> &arr) {
  arr.resize(ALL_MINO_NUM);
  for (int i = 0; i < arr.size(); i++) {
    arr[i] = i;
  }
}

void MinoRow::shuffleRow(std::vector<int> &arr) {
  for (int i = ALL_MINO_NUM - 1; i >= 0; i--) {
    int r   = GetRand(ALL_MINO_NUM - 1);
    int tmp = arr[i];
    arr[i]  = arr[r];
    arr[r]  = tmp;
  }
}