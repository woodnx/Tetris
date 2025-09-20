#pragma once
#include "BlockId.h"
#include "DxLib.h"
#include "Field.h"
#include "StaticMino.h"
#include "include.h"

#include <memory>

#define LAYOUT_SIZE 4

using namespace std;

class Mino : public StaticMino {
  public:
    // static int const LATOUT_SIZE = 4;

    Mino(Field& field);
    Mino& operator=(const Mino& other);
    void init() override;

    Coordinates global_coord();

    void generate(shared_ptr<BlockId> generate_mino_num, Coordinates local);
    bool collision();
    bool collision(Coordinates diff);

    void rotate(bool right_flag);
    bool super_rotate(int dir_old, bool right_flag);

    void move(Coordinates d, bool with_collision = false);
    void hard_drop();

    void transcribe();
    void draw_status();

  private:
    Field& field;
    Coordinates local;
};
