#pragma once
#include "Bag.h"
#include "BaseScene.h"
#include "BlockId.h"
#include "Coordinates.h"
#include "Dxlib.h"
#include "Field.h"
#include "Mino.h"
#include "MinoRow.h"
#include "Sound.h"
#include "StaticMino.h"
#include "include.h"

#include <array>

using namespace std;

class Player {
  private:
    BaseScene* gameManager;
    Field field;
    Mino mino;
    Mino ghost;
    StaticMino hold;
    vector<StaticMino> next;
    Bag bag;

    // 画像・フォントハンドル
    int background_handle;
    int index_font;
    int figure_font;

    // 音声管理
    Sound sound;

    // 座標
    Coordinates global;
    Coordinates generate_place;
    Coordinates pre_mino_place;
    Coordinates bottom;

    // スコア・レベル管理
    int level;
    int score;

    int ren_count;
    int max_lines = 100;

    const array<int, 5> clear_line_table = {0, 100, 300, 500, 800};

    bool is_back_to_back = false;
    bool is_t_spin       = false;
    int soft_drop_cells  = 0;
    int hard_drop_cells  = 0;

    int lockdown_count;
    int autorepeat_count;
    bool is_autorepeat;

    int clear_line_count;
    int levelup_count;
    int total_clear_lines;
    int shift_count;

    bool can_hold;
    bool can_control;

    // private関数
    void generate_mino(shared_ptr<BlockId>, bool);
    void make_ghost();
    void set_next();
    int calc_t_spin_base(int t_spin_type, int cleared_lines);
    void calc_score_and_level();
    int judge_game();

  public:
    int drop_speed = 60;

    Player(BaseScene* gameManager, Coordinates global);

    void init();
    int update(bool key_pressed);
    void draw();
    void finalize();

    void start_game();
    void rotate_mino(bool is_right);
    void move_mino(bool is_right);
    void hard_drop();
    void soft_drop();
    void hold_mino();
};