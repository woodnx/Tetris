#include "SceneMgr.h"

SceneMgr::SceneMgr(): next_scene(eScene_None) {
  scene = (BaseScene*) new Menu(this);
}

// 初期化
void SceneMgr::initialize() {
  scene->initialize();
}

// 終了処理
void SceneMgr::finalize() {
  scene->finalize();
}

// 更新
void SceneMgr::update() {
  if (next_scene != eScene_None) { // 次のシーンがセットされていたら
    scene->finalize();             // 現在のシーンの終了処理を実行
    delete scene;
    switch (next_scene) { // シーンによって処理を分岐
      case eScene_Menu:   // 次の画面がメニューなら
        scene =
          (BaseScene*) new Menu(this); // メニュー画面のインスタンスを生成する
        break;                         // 以下略
      case eScene_Normal:
        scene = (BaseScene*) new OnePlayerGame(this);
        break;
        // case eScene_Tetlis2pi:
        //     mScene = (BaseScene*) new Game2pi(this);
        //     break;
    }
    next_scene = eScene_None; // 次のシーン情報をクリア
    scene->initialize();      // シーンを初期化
  }

  scene->update(); // シーンの更新
}

// 描画
void SceneMgr::draw() {
  scene->draw(); // シーンの描画
}

// 引数 nextScene にシーンを変更する
void SceneMgr::change_scene(eScene next) {
  next_scene = next; // 次のシーンをセットする
}