#include "OnePlayerGame.h"

OnePlayerGame::OnePlayerGame(ISceneChanger* changer):
    BaseScene(changer),
    player(this, Coordinates(380, 50)) {
  sound.add("select", "sounds/select.mp3");
  sound.add("dicision", "sounds/dicision.mp3");
  sound.changeAllSoundVolume(70);
  sound.add("count", "sounds/countdown.mp3");
  sound.add("pause", "sounds/pause.mp3");
  sound.add("bgm", "sounds/tetlis2pi.mp3");

  // カウントダウン関連
  is_count      = true;
  is_gamestart  = true;
  count         = 3;
  auto fontname = (TCHAR*) "ニコ角";
  count_font    = CreateFontToHandle(fontname, 75, 1, DX_FONTTYPE_ANTIALIASING);

  // ポーズ関連
  is_pause   = false;
  now_select = ePause_Continue;
  pause_y    = 0;
  pause_font = CreateFontToHandle(fontname, 40, 1, DX_FONTTYPE_ANTIALIASING);

  // ゲームリザルト関連
  is_gameover  = false;
  is_gameclear = false;

  /*std::ifstream file("savedatas/one_player.txt");
  std::string data;
  std::getline(file, data);

  highscore = stoi(data);*/
}

void OnePlayerGame::initialize() {
  player.init();

  key_pressed = false;

  // カウントダウン関連
  is_count     = true;
  is_gamestart = true;
  count        = 3;

  // ポーズ関連
  is_pause   = false;
  now_select = ePause_Continue;
  pause_y    = 0;

  // ゲームリザルト関連
  is_gameover  = false;
  is_gameclear = false;
}

void OnePlayerGame::update() {
  if (Key[KEY_INPUT_ESCAPE] == 1 && !is_count) {
    sound.play("pause", DX_PLAYTYPE_BACK);
    is_count = false;
    is_pause = true;
  }

  if (is_count)
    count_down();
  else if (is_pause)
    pause();
  else if (game_result != 0)
    game_result_scene();
  else {
    if (Key[KEY_INPUT_A] >= 1) {
      player.move_mino(false);
      key_pressed = true;
    } else if (Key[KEY_INPUT_D] >= 1) {
      player.move_mino(true);
      key_pressed = true;
    } else if (Key[KEY_INPUT_S] >= 1) {
      player.soft_drop();
      key_pressed = true;
    } else {
      key_pressed = false;
    }

    if (Key[KEY_INPUT_L] == 1) {
      player.rotate_mino(true);
    } else if (Key[KEY_INPUT_K] == 1) {
      player.rotate_mino(false);
    }

    if (Key[KEY_INPUT_W] == 1) {
      player.hard_drop();
    }

    if (Key[KEY_INPUT_SPACE] == 1) {
      player.hold_mino();
    }

    if (frame_count() % player.drop_speed == 0) {
      player.soft_drop(false);
    }

    game_result = player.update(key_pressed);
  }

  BaseScene::update();
}

void OnePlayerGame::draw() {
  player.draw();

  if (is_count)
    count_down_draw();
  else if (is_pause)
    pause_draw();
  else if (game_result)
    game_result_draw(game_result);
}

void OnePlayerGame::finalize() {
  player.finalize();
}

void OnePlayerGame::count_down() {
  if (count == 3)
    sound.play("count", DX_PLAYTYPE_BACK);
  if (frame_count() % 60 == 0) {
    count--;
  }
  if (count < 0) {
    is_count = false;
    count    = 3;
    reset_frame_count();
    sound.play("bgm", DX_PLAYTYPE_LOOP);

    if (is_gamestart) {
      player.start_game();
    }
    is_gamestart = false;
  }
}

void OnePlayerGame::count_down_draw() {
  SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
  DrawBox(0, 0, WINDOW_SIZE_X - 1, WINDOW_SIZE_Y - 1, GetColor(0, 0, 0), TRUE);
  SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードをオフ
  if (count > 0) {
    DrawFormatStringToHandle(
      WINDOW_SIZE_X / 2,
      WINDOW_SIZE_Y / 2 - 50,
      GetColor(255, 255, 255),
      count_font,
      (TCHAR*) "%d",
      count);
  } else
    DrawStringToHandle(
      WINDOW_SIZE_X / 2 - 100,
      WINDOW_SIZE_Y / 2 - 50,
      (TCHAR*) "START",
      GetColor(255, 255, 255),
      count_font);
}

void OnePlayerGame::pause() {
  if (Key[KEY_INPUT_S] == 1) { // 下キーが押されていたら
    sound.play("select", DX_PLAYTYPE_BACK);
    now_select = (now_select + 1) % ePause_Num; // 選択状態を一つ下げる
  }
  if (Key[KEY_INPUT_W] == 1) { // 上キーが押されていたら
    sound.play("select", DX_PLAYTYPE_BACK);
    now_select =
      (now_select + (ePause_Num - 1)) % ePause_Num; // 選択状態を一つ上げる
  }
  if (Key[KEY_INPUT_SPACE] == 1) { // エンターキーが押されたら
    sound.play("dicision", DX_PLAYTYPE_BACK);
    switch (now_select) { // 現在選択中の状態によって処理を分岐
      case ePause_Continue:
        is_count = true;
        is_pause = false;
        break;
      case ePause_Restart:
        initialize();
        player.init();
        break;
      case ePause_End: // 設定選択中なら
        scene_changer->change_scene(eScene_Menu);
        break;
    }
  }
}

void OnePlayerGame::pause_draw() {
  SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
  DrawBox(0, 0, WINDOW_SIZE_X - 1, WINDOW_SIZE_Y - 1, GetColor(0, 0, 0), TRUE);
  SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードをオフ

  DrawStringToHandle(
    WINDOW_SIZE_X / 2 - 50,
    300,
    (TCHAR*) "つづける",
    GetColor(255, 255, 255),
    pause_font);
  DrawStringToHandle(
    WINDOW_SIZE_X / 2 - 50,
    300 + 70,
    (TCHAR*) "やりなおす",
    GetColor(255, 255, 255),
    pause_font);
  DrawStringToHandle(
    WINDOW_SIZE_X / 2 - 50,
    300 + 70 * 2,
    (TCHAR*) "やめる",
    GetColor(255, 255, 255),
    pause_font);

  switch (now_select) {   // 現在の選択状態に従って処理を分岐
    case ePause_Continue: // コンティニュー選択中
      sound.stop("bgm");
      pause_y = 300;
      break;
    case ePause_Restart: // リスタート選択中
      pause_y = 300 + 70;
      break;
    case ePause_End: // エンド選択中なら
      pause_y = 300 + 70 * 2;
      break;
  }
  DrawStringToHandle(
    WINDOW_SIZE_X / 2 - 100,
    pause_y,
    (TCHAR*) "■",
    GetColor(255, 255, 255),
    pause_font);
}

void OnePlayerGame::game_result_scene() {
  sound.stop("bgm");
  if (Key[KEY_INPUT_S] == 1) { // 下キーが押されていたら
    sound.play("select", DX_PLAYTYPE_BACK);
    now_select = (now_select + 1) % eResult_Num; // 選択状態を一つ下げる
  }
  if (Key[KEY_INPUT_W] == 1) { // 上キーが押されていたら
    sound.play("select", DX_PLAYTYPE_BACK);
    now_select =
      (now_select + (eResult_Num - 1)) % eResult_Num; // 選択状態を一つ上げる
  }
  if (Key[KEY_INPUT_SPACE] == 1) { // エンターキーが押されたら
    sound.play("dicision", DX_PLAYTYPE_BACK);
    switch (now_select) {
      case eResult_Restart:
        scene_changer->change_scene(eScene_Normal);
        break;
      case eResult_End:
        scene_changer->change_scene(eScene_Menu);
        break;
    }
  }
}

void OnePlayerGame::game_result_draw(int game_result) {
  SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
  DrawBox(0, 0, WINDOW_SIZE_X - 1, WINDOW_SIZE_Y - 1, GetColor(0, 0, 0), TRUE);
  SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードをオフ

  if (game_result == 1) {
    // if (score > highscore) {

    //    if (fopen_s(&fp, "savedates/highscore_normal.txt", "w") != NULL) {
    //        //ファイルオープン失敗
    //    }
    //    else {
    //        fprintf(fp, "%d", score);
    //        fclose(fp);
    //    }
    //    DrawStringToHandle(WINDOW_SIZE_X / 2 - 100, 180, "HIGH SCORE",
    //    GetColor(255, 255, 255), pause_font);
    //}
    DrawStringToHandle(
      WINDOW_SIZE_X / 2 - 200,
      100,
      (TCHAR*) "GAME CLEAR",
      GetColor(255, 255, 255),
      count_font);
  } else if (game_result == -1) {
    DrawStringToHandle(
      WINDOW_SIZE_X / 2 - 200,
      100,
      (TCHAR*) "GAME OVER",
      GetColor(255, 255, 255),
      count_font);
  }

  DrawStringToHandle(
    WINDOW_SIZE_X / 2 - 50,
    300,
    (TCHAR*) "もういちど遊ぶ",
    GetColor(255, 255, 255),
    pause_font);
  DrawStringToHandle(
    WINDOW_SIZE_X / 2 - 50,
    300 + 70,
    (TCHAR*) "メニューに戻る",
    GetColor(255, 255, 255),
    pause_font);

  switch (now_select) {   // 現在の選択状態に従って処理を分岐
    case eResult_Restart: // ゲーム選択中なら
      pause_y = 300;      // ゲームの座標を格納
      break;
    case eResult_End:     // 設定選択中なら
      pause_y = 300 + 70; // 設定の座標を格納
      break;
  }
  DrawStringToHandle(
    WINDOW_SIZE_X / 2 - 100,
    pause_y,
    (TCHAR*) "■",
    GetColor(255, 255, 255),
    pause_font);
}
