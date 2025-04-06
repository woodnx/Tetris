#pragma once
#include "include.h"
#include "Player.h"
#include "MinoRow.h"
#include "BaseScene.h"
#include <time.h> 
#include <iostream>
#include <fstream>
#include <string>

class OnePlayerGame : public BaseScene {
  private:
    Player* player;
    Sound sound;
    // カウントダウン関連	
    bool is_count;
    bool is_gamestart;
    int count;
    int count_font;
    int t;

    // ポーズ画面関連
    bool is_pause;
    int now_select;
    int pause_y;
    int pause_font;

    // リザルト関連	
    bool is_gameover;
    bool is_gameclear;
    const int GAMECLEAR = 1;
    const int GAMEOVER = -1;

    // スコア関連
    int highscore = 0;

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

