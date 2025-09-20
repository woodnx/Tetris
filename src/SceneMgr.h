#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "Menu.h"
#include "OnePlayerGame.h"

class SceneMgr : public ISceneChanger, Task {
  private:
    BaseScene* scene;  // シーン管理変数
    eScene next_scene; // 次のシーン管理変数

  public:
    SceneMgr();
    void initialize() override; // 初期化
    void finalize() override;   // 終了処理
    void update() override;     // 更新
    void draw() override;       // 描画

    // 引数 nextScene にシーンを変更する
    void change_scene(eScene next) override;
};
