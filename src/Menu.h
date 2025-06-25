#pragma once
#include "BaseScene.h"
#include "DxLib.h"
#include "include.h"

typedef enum {
  eMenu_1PGame, // 1Pゲーム
  eMenu_Config, // 設定
  eMenu_Exit,
  eMenu_Num, // 本項目の数
} eMenu;

// メニュー画面クラス
class Menu : public BaseScene {

  public:
    Menu(ISceneChanger *changer);
    void Initialize() override; // 初期化処理をオーバーライド。
    // void Finalize() override ;        //終了処理をオーバーライド。
    void Update() override; // 更新処理をオーバーライド。
    void Draw() override;   // 描画処理をオーバーライド。

  private:
    int background_Handle;
    const static int GAME_Y   = 400; // 「ゲーム」文字のy位置
    const static int CONFIG_Y = 470; // 「設定」文字のy位置
    const static int EXIT_Y   = 540; // 「設定」文字のy位置
    int menu_font;
    int NowSelect; // 現在の選択状態(初期はゲーム選択中)
};