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

class Player {
  private:
    BaseScene* gameManager;
    Field field;
    Mino mino;
    Mino ghost;
    StaticMino hold;
    vector<StaticMino> next;
    Bag bag = Bag();

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

    // スコア
    int level;
    int score;
    int ren_num;
    const int max_lines = 100;

    // オートリピート
    int lockdown_count;
    int autorepeat_count;
    bool is_autorepeat;

    // ライン削除
    int erase_linenum;
    int levelup_count;
    int total_lines;
    int shift_count;

    // ホールド
    bool can_hold;

    bool can_control;

    // private関数
    void generate_mino(shared_ptr<BlockId>, bool);
    void make_ghost();
    void set_next();
    void level_control();
    int judge_game();

  public:
    int drop_speed;

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
    int calc_score();
};