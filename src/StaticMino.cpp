#include "StaticMino.h"

StaticMino::StaticMino(Coordinates global):
    Blocks(global, LAYOUT_SIZE, LAYOUT_SIZE, STATIC_BLOCK_SIZE) {}

StaticMino::StaticMino(Coordinates global, unsigned int size):
    Blocks(global, LAYOUT_SIZE, LAYOUT_SIZE, size) {}

void StaticMino::init() {
  fill_layout(BlockId::Empty);
  this->_id = BlockId::Empty;
}

shared_ptr<BlockId> StaticMino::id() {
  return this->_id;
}

void StaticMino::generate(shared_ptr<BlockId> mino_id) {
  fill_layout(BlockId::Empty);
  this->_id = mino_id;

  if (mino_id && mino_id->id == BlockId::Imino->id) {
    init_layout(4, 4);
    //			 Y  X
    this->layout[1][0] = this->_id;
    this->layout[1][1] = this->_id;
    this->layout[1][2] = this->_id;
    this->layout[1][3] = this->_id;
  } else if (mino_id && mino_id->id == BlockId::Lmino->id) {
    init_layout(3, 3);
    //			 Y  X
    this->layout[0][2] = this->_id;
    this->layout[1][0] = this->_id;
    this->layout[1][1] = this->_id;
    this->layout[1][2] = this->_id;
  } else if (mino_id && mino_id->id == BlockId::Jmino->id) {
    init_layout(3, 3);
    //			 Y  X
    this->layout[0][0] = this->_id;
    this->layout[1][0] = this->_id;
    this->layout[1][1] = this->_id;
    this->layout[1][2] = this->_id;
  } else if (mino_id && mino_id->id == BlockId::Smino->id) {
    init_layout(3, 3);
    //			 Y  X
    this->layout[0][1] = this->_id;
    this->layout[0][2] = this->_id;
    this->layout[1][0] = this->_id;
    this->layout[1][1] = this->_id;
  } else if (mino_id && mino_id->id == BlockId::Zmino->id) {
    init_layout(3, 3);
    //			 Y  X
    this->layout[0][0] = this->_id;
    this->layout[0][1] = this->_id;
    this->layout[1][1] = this->_id;
    this->layout[1][2] = this->_id;
  } else if (mino_id && mino_id->id == BlockId::Omino->id) {
    init_layout(2, 2);
    //			 Y  X
    this->layout[0][0] = this->_id;
    this->layout[0][1] = this->_id;
    this->layout[1][0] = this->_id;
    this->layout[1][1] = this->_id;
  } else if (mino_id && mino_id->id == BlockId::Tmino->id) {
    init_layout(3, 3);
    //			 Y  X
    this->layout[0][1] = this->_id;
    this->layout[1][0] = this->_id;
    this->layout[1][1] = this->_id;
    this->layout[1][2] = this->_id;
  } else {
    this->_id = BlockId::Null;
  }
}
