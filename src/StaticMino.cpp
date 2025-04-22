#include "StaticMino.h"

StaticMino::StaticMino(int x, int y)
    : Blocks({x, y}, LAYOUT_SIZE, LAYOUT_SIZE, STATIC_BLOCK_SIZE) {
  this->num = -1;
}

StaticMino::StaticMino(int x, int y, unsigned int size)
    : Blocks({x, y}, LAYOUT_SIZE, LAYOUT_SIZE, size) {
  this->num = -1;
}

void StaticMino::initialize() {
  fillLayout(0);
  this->num = -1;
}

void StaticMino::generateMino(int generate_mino_num) {
  fillLayout(0);
  switch (generate_mino_num) {
    case 0: // I
      this->num = Imino;
      initLayout(4, 4);
      //			 Y  X
      this->layout[1][0] = this->num;
      this->layout[1][1] = this->num;
      this->layout[1][2] = this->num;
      this->layout[1][3] = this->num;
      break;

    case 1: // L
      this->num = Lmino;
      initLayout(3, 3);
      //			 Y  X
      this->layout[0][2] = this->num;
      this->layout[1][0] = this->num;
      this->layout[1][1] = this->num;
      this->layout[1][2] = this->num;
      break;

    case 2: // J
      this->num = Jmino;
      initLayout(3, 3);
      //			 Y  X
      this->layout[0][0] = this->num;
      this->layout[1][0] = this->num;
      this->layout[1][1] = this->num;
      this->layout[1][2] = this->num;
      break;

    case 3: // S
      this->num = Smino;
      initLayout(3, 3);
      //			 Y  X
      this->layout[0][1] = this->num;
      this->layout[0][2] = this->num;
      this->layout[1][0] = this->num;
      this->layout[1][1] = this->num;
      break;

    case 4: // Z
      this->num = Zmino;
      initLayout(3, 3);
      //			 Y  X
      this->layout[0][0] = this->num;
      this->layout[0][1] = this->num;
      this->layout[1][1] = this->num;
      this->layout[1][2] = this->num;
      break;

    case 5: // O
      this->num = Omino;
      initLayout(2, 2);
      //			 Y  X
      this->layout[0][0] = this->num;
      this->layout[0][1] = this->num;
      this->layout[1][0] = this->num;
      this->layout[1][1] = this->num;
      break;

    case 6: // T
      this->num = Tmino;
      initLayout(3, 3);
      //			 Y  X
      this->layout[0][1] = this->num;
      this->layout[1][0] = this->num;
      this->layout[1][1] = this->num;
      this->layout[1][2] = this->num;
      break;

    defalt:
      this->num = -1;
      break;
  }
}