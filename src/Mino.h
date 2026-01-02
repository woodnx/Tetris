#pragma once
#include "BlockId.h"
#include "DxLib.h"
#include "Field.h"
#include "StaticMino.h"
#include "include.h"

#include <array>
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
    bool super_rotate(int dir_old);

    void move(Coordinates d, bool with_collision = false);
    void hard_drop();

    // 0: `no rotate`, 1: `normal rotate`, 2: `super rotate`
    int check_t_spin();

    void transcribe();
    void draw_status();

  private:
    Field& field;
    Coordinates local;

    /* Tスピン関連 */
    bool use_spin = false; // 回転使用フラグ
    int last_srs  = 0;     // 最後に行ったSRSパターン(0-4)
};
