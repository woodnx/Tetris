#pragma once

enum eScene {
  eScene_Menu,      // メニュー画面
  eScene_Normal,    // ゲーム画面
  eScene_Tetlis2pi, // 設定画面
  eScene_Exit,      // ゲーム終了
  eScene_None,      // 無し
};

// シーンを変更するためのインターフェイスクラス
class ISceneChanger {
  public:
    virtual ~ISceneChanger()                     = 0;
    virtual void change_scene(eScene next_scene) = 0; // 指定シーンに変更する
};
