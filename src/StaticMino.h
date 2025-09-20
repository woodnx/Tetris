#pragma once
#include "Blocks.h"
#include "DxLib.h"
#define LAYOUT_SIZE       4
#define STATIC_BLOCK_SIZE 20

class StaticMino : public Blocks {
  public:
    StaticMino(Coordinates global);
    StaticMino(Coordinates global, unsigned int size);
    virtual void init();
    shared_ptr<BlockId> id();
    void generate(shared_ptr<BlockId> generate_mino_id);

  protected:
    shared_ptr<BlockId> _id = BlockId::Null; // each Mino unique number
};
