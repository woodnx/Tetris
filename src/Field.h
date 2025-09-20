#pragma once
#include "BlockId.h"
#include "Blocks.h"
#include "DxLib.h"

#include <memory>

#define FIELD_SIDE_X  10
#define FIELD_SIDE_Y  24
#define FIELD_ASIDE_X FIELD_SIDE_X + 2
#define FIELD_ASIDE_Y FIELD_SIDE_Y + 2
#define BLOCK_SIZE    30

using namespace std;

class Field : public Blocks {
  public:
    // static const int FIELD_SIDE_X  = 10;
    // static const int FIELD_SIDE_Y  = 24;
    // static const int FIELD_ASIDE_X = FIELD_SIDE_X + 2;
    // static const int FIELD_ASIDE_Y = FIELD_SIDE_Y + 2;
    // static const int BLOCK_SIZE    = 30;

    Field(Coordinates global);

    void init();

    bool judge_erasing(int line_index);
    void erase_line(int line_index);
    void shift_line(int line_index);
    int erase_lines();

    Coordinates local_to_global(Coordinates local);
    Coordinates global_to_local(Coordinates global);

    shared_ptr<BlockId> get_field_value(Coordinates local);
    void set_field_value(Coordinates local, shared_ptr<BlockId> value);

    bool contain_mino(Coordinates local);

    void draw();

  private:
    int color;
    int grid_color;
    bool shift_stopping;
};
