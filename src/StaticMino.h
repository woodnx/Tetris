#pragma once
#include "Blocks.h"
#include "DxLib.h"
#define LAYOUT_SIZE       4
#define STATIC_BLOCK_SIZE 20

class StaticMino : public Blocks {
  public:
    StaticMino(int x, int y);
    StaticMino(int x, int y, unsigned int size);
    virtual void initialize();
    void generateMino(int generate_mino_num);

  protected:
    int num = -1; // each Mino unique number
};
