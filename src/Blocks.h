#pragma once
#include "BlockID.h"
#include "Coordinates.h"
#include "DxLib.h"

#include <memory>
#include <vector>

using namespace std;

class Blocks {
  private:
    unsigned int _width, _height;
    unsigned int _size;

  protected:
    vector<vector<shared_ptr<BlockId>>> layout;
    Coordinates global;
    int rotate_dir;

    // Gettor
    unsigned int width();
    unsigned int height();
    unsigned int block_size();

    void init_layout(unsigned int width, unsigned int height);
    void fill_layout(shared_ptr<BlockId> fill_id);
    vector<vector<shared_ptr<BlockId>>> copy_layout();

  public:
    Blocks(
      Coordinates global,
      unsigned int width,
      unsigned int height,
      unsigned int size);
    int rotate(bool right_flag);

    int draw(bool fill_flag);
    int draw(bool fill_flag, int alpha);
};
