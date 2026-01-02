#pragma once
#include "DxLib.h"

// タスククラス。何かのモジュールはすべてこのTaskクラスを継承する。
class Task {
  public:
    virtual ~Task() {}
    virtual void initialize() {} // 初期化処理は実装してもしなくてもいい
    virtual void finalize() {}   // 終了処理は実装してもしなくてもいい
    virtual void update() = 0;   // 更新処理は必ず継承先で実装する
    virtual void draw()   = 0;   // 描画処理は必ず継承先で実装する
};