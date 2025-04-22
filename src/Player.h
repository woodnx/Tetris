#pragma once
#include "BaseScene.h"
#include "Field.h"
#include "Mino.h"
#include "MinoRow.h"
#include "Sound.h"
#include "StaticMino.h"
#include "include.h"

typedef enum {
  ePause_Continue, // 続ける
  ePause_Restart,  // やりなおす
  ePause_End,      // スタートに戻る
  ePause_Num,      // 本項目の数
} ePause;

typedef enum {
  eResult_Restart, // やりなおす
  eResult_End,     // スタートに戻る
  eResult_Num,     // 本項目の数
} eResult;

class Player : public BaseScene {
  public:
    Player(ISceneChanger *changer);

    void Initialize() override;

    void StartCountDown();

    void RestartGame();

    void Update() override;
    void controlMino();
    void dropMino();
    void holdMino();
    void makeGhost();
    void setNext();
    int calcScore(int _level, int _drop_speed);
    void levelControl();
    void pause();
    void installMino();
    void GameResult();
    void GameClear();
    void GameOver();

    void Draw() override;

    void Finalize() override;

  private:
    Field *field;
    Mino *mino;
    Mino *ghost_mino;
    StaticMino *hold_mino;
    StaticMino *next_mino[NEXT_REFER_SIZE];

    MinoRow row;

    FILE *fp;
    int highscore;

    int background_handle;
    int bgm_handle;
    int score_font;

    Sound sound;

    int opening_count;
    bool iscount;
    bool isGamestart;
    int count_font;

    int x, y;
    int gnrt_mx, gnrt_my;

    int level;
    int score;
    int ren_num;

    int t;
    int lockdown_count;
    int autorepeat_count;
    bool isautorepeat;

    int new_gnrt_mino;
    bool can_generate;
    bool can_transcribe;
    bool can_incrrow;

    int erase_linenum;
    int levelup_count;
    int sum_linenum;

    int drop_speed;
    bool isbottom;
    int pre_mino_coordx;
    int pre_mino_coordy;

    int row_num;
    int next[6];

    int hold_mino_num;
    bool hold_enable;

    bool isGameOver;
    bool isGameClear;
    int max_linenum;

    bool ispause;
    int NowSelect;
    int pause_font;
    int pause_y;
    const int CONTINUE_Y = 300;
    const int RESTART_Y  = 370;
    const int END_Y      = 440;
};