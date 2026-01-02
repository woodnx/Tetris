#include "Menu.h"

Menu::Menu(ISceneChanger* changer): BaseScene(changer) {
  now_select = eMenu_1PGame;
}

// 初期化
void Menu::initialize() {
  now_select = eMenu_1PGame;
  menu_font =
    CreateFontToHandle((TCHAR*) "ニコ角", 40, 1, DX_FONTTYPE_ANTIALIASING);

  image_handle      = LoadGraph((TCHAR*) "images/rogo.png"); // 画像のロード
  background_handle = LoadGraph((TCHAR*) "images/Menu_Back.jpg");
  select_se         = LoadSoundMem((TCHAR*) "sounds/select.mp3");
  decision_se       = LoadSoundMem((TCHAR*) "sounds/dicision.mp3");
  menu_bgm          = LoadSoundMem((TCHAR*) "sounds/menu.mp3");
  ChangeVolumeSoundMem(255 * 80 / 100, menu_bgm);
  PlaySoundMem(menu_bgm, DX_PLAYTYPE_BACK);
}

// 更新
void Menu::update() {
  if (Key[KEY_INPUT_S] == 1) { // 下キーが押されていたら
    PlaySoundMem(select_se, DX_PLAYTYPE_BACK);
    now_select = (now_select + 1) % eMenu_Num; // 選択状態を一つ下げる
  }
  if (Key[KEY_INPUT_W] == 1) { // 上キーが押されていたら
    PlaySoundMem(select_se, DX_PLAYTYPE_BACK);
    now_select =
      (now_select + (eMenu_Num - 1)) % eMenu_Num; // 選択状態を一つ上げる
  }
  if (Key[KEY_INPUT_SPACE] == 1) { // エンターキーが押されたら
    PlaySoundMem(decision_se, DX_PLAYTYPE_BACK);
    StopSoundMem(menu_bgm);
    switch (now_select) { // 現在選択中の状態によって処理を分岐
      case eMenu_1PGame:  // ゲーム選択中なら
        scene_changer->change_scene(eScene_Normal); // シーンをゲーム画面に変更
        break;
      case eMenu_Config: // 設定選択中なら
        scene_changer->change_scene(
          eScene_Tetlis2pi); // シーンをゲーム画面に変更
        break;
      case eMenu_Exit: // ゲーム終了選択中なら
        DxLib_End();
        break;
    }
  }
}

// 描画
void Menu::draw() {
  // BaseScene::Draw();//親クラスの描画メソッドを呼ぶ
  DrawGraph(0, 0, background_handle, TRUE);
  DrawRotaGraph(WINDOW_SIZE_X / 2, 200, 0.7, 0, image_handle, TRUE);
  // DrawString(200, 150, "MENU", GetColor(255, 255, 255));
  // DrawString(200, 170, "上下キーを押し、エンターを押して下さい。",
  // GetColor(255, 255, 255));
  DrawStringToHandle(
    WINDOW_SIZE_X / 2 - 50,
    GAME_Y,
    (TCHAR*) "NORMAL",
    GetColor(255, 255, 255),
    menu_font);
  DrawStringToHandle(
    WINDOW_SIZE_X / 2 - 50,
    CONFIG_Y,
    (TCHAR*) "TETLIS 2π",
    GetColor(255, 255, 255),
    menu_font);
  DrawStringToHandle(
    WINDOW_SIZE_X / 2 - 50,
    EXIT_Y,
    (TCHAR*) "EXIT",
    GetColor(255, 255, 255),
    menu_font);
  int y = 0;
  switch (now_select) { // 現在の選択状態に従って処理を分岐
    case eMenu_1PGame:  // ゲーム選択中なら
      y = GAME_Y;       // ゲームの座標を格納
      break;
    case eMenu_Config: // 設定選択中なら
      y = CONFIG_Y;    // 設定の座標を格納
      break;
    case eMenu_Exit: // ゲーム選択中なら
      y = EXIT_Y;
      break;
  }
  DrawStringToHandle(
    WINDOW_SIZE_X / 2 - 100,
    y,
    (TCHAR*) "■",
    GetColor(255, 255, 255),
    menu_font);
}
