#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "include.h"

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

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

class OnePlayerGame : public BaseScene {
  private:
    Player player;
    Sound sound;

    // カウントダウン関連
    bool is_count;
    bool is_gamestart;
    int count;
    int count_font;

    // ポーズ画面関連
    bool is_pause;
    int now_select;
    int pause_y;
    int pause_font;

    // リザルト関連
    int game_result = 0;
    bool is_gameover;
    bool is_gameclear;
    const int GAMECLEAR = 1;
    const int GAMEOVER  = -1;

    // スコア関連
    int highscore = 0;

    bool key_pressed = false;

  public:
    OnePlayerGame(ISceneChanger* changer);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

    void countDown();
    void countDownDraw();
    void pause();
    void pauseDraw();
    void gameResultScene();
    void gameResultDraw(int);
};
