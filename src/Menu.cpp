#include "Menu.h"

Menu::Menu(ISceneChanger *changer): BaseScene(changer) {
  NowSelect = eMenu_1PGame;
}

// 初期化
void Menu::Initialize() {
  NowSelect = eMenu_1PGame;
  menu_font = CreateFontToHandle("ニコ角", 40, 1, DX_FONTTYPE_ANTIALIASING);

  mImageHandle      = LoadGraph("images/rogo.png"); // 画像のロード
  background_Handle = LoadGraph("images/Menu_Back.jpg");
  selectSE          = LoadSoundMem("sounds/カーソル移動2.mp3");
  decisionSE        = LoadSoundMem("sounds/決定、ボタン押下26.mp3");
  menuBGM           = LoadSoundMem("sounds/魔王魂  サイバー13.mp3");
  ChangeVolumeSoundMem(255 * 80 / 100, menuBGM);
  PlaySoundMem(menuBGM, DX_PLAYTYPE_BACK);
}

// 更新
void Menu::Update() {
  if (Key[KEY_INPUT_S] == 1) { // 下キーが押されていたら
    PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
    NowSelect = (NowSelect + 1) % eMenu_Num; // 選択状態を一つ下げる
  }
  if (Key[KEY_INPUT_W] == 1) { // 上キーが押されていたら
    PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
    NowSelect =
        (NowSelect + (eMenu_Num - 1)) % eMenu_Num; // 選択状態を一つ上げる
  }
  if (Key[KEY_INPUT_SPACE] == 1) { // エンターキーが押されたら
    PlaySoundMem(decisionSE, DX_PLAYTYPE_BACK);
    StopSoundMem(menuBGM);
    switch (NowSelect) { // 現在選択中の状態によって処理を分岐
      case eMenu_1PGame: // ゲーム選択中なら
        mSceneChanger->ChangeScene(eScene_Normal); // シーンをゲーム画面に変更
        break;
      case eMenu_Config: // 設定選択中なら
        mSceneChanger->ChangeScene(eScene_Tetlis2pi
        ); // シーンをゲーム画面に変更
        break;
      case eMenu_Exit: // ゲーム終了選択中なら
        DxLib_End();
        break;
    }
  }
}

// 描画
void Menu::Draw() {
  // BaseScene::Draw();//親クラスの描画メソッドを呼ぶ
  DrawGraph(0, 0, background_Handle, TRUE);
  DrawRotaGraph(WINDOW_SIZE_X / 2, 200, 0.7, 0, mImageHandle, TRUE);
  // DrawString(200, 150, "MENU", GetColor(255, 255, 255));
  // DrawString(200, 170, "上下キーを押し、エンターを押して下さい。",
  // GetColor(255, 255, 255));
  DrawStringToHandle(
      WINDOW_SIZE_X / 2 - 50, GAME_Y, "NORMAL", GetColor(255, 255, 255),
      menu_font
  );
  DrawStringToHandle(
      WINDOW_SIZE_X / 2 - 50, CONFIG_Y, "TETLIS 2π", GetColor(255, 255, 255),
      menu_font
  );
  DrawStringToHandle(
      WINDOW_SIZE_X / 2 - 50, EXIT_Y, "EXIT", GetColor(255, 255, 255), menu_font
  );
  int y = 0;
  switch (NowSelect) { // 現在の選択状態に従って処理を分岐
    case eMenu_1PGame: // ゲーム選択中なら
      y = GAME_Y;      // ゲームの座標を格納
      break;
    case eMenu_Config: // 設定選択中なら
      y = CONFIG_Y;    // 設定の座標を格納
      break;
    case eMenu_Exit: // ゲーム選択中なら
      y = EXIT_Y;
      break;
  }
  DrawStringToHandle(
      WINDOW_SIZE_X / 2 - 100, y, "■", GetColor(255, 255, 255), menu_font
  );
}
