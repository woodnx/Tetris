#pragma once
#include "DxLib.h"

#include <vector>


using namespace std;

struct Position {
    int x;
    int y;
};

enum BlockValue {
  Empty,
  Imino,
  Lmino,
  Jmino,
  Smino,
  Zmino,
  Omino,
  Tmino,
  Wall
};

class Blocks {
  private:
    unsigned int _width, _height;
    unsigned int _size;

  protected:
    vector<vector<int>> layout;
    Position window;
    int rotate_dir;

    // Gettor
    unsigned int width();
    unsigned int height();
    unsigned int block_size();

    int initLayout(unsigned int width, unsigned int height);
    int fillLayout(int value);
    int selectColor(int color_num);

  public:
    Blocks(
        Position window, unsigned int width, unsigned int height,
        unsigned int size
    );
    int rotate(bool right_flag);

    int draw(bool fill_flag);
    int draw(bool fill_flag, int alpha);
};
