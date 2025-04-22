#pragma once

typedef enum {
  eScene_Menu,      // メニュー画面
  eScene_Normal,    // ゲーム画面
  eScene_Tetlis2pi, // 設定画面
  eScene_Exit,      // ゲーム終了
  eScene_None,      // 無し
} eScene;

// シーンを変更するためのインターフェイスクラス
class ISceneChanger {
  public:
    virtual ~ISceneChanger() = 0;
    virtual void ChangeScene(eScene NextScene) = 0; // 指定シーンに変更する
};
