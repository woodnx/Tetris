/*
 * TETLIS 2π
 * 作成 j1813 木次駿
 * 完成 2021/07/
 *
 * 概要：
 *   基本的に挙動は全てガイドラインに準拠
 *   （具体的には，テトリミノの色，NEXT・ゴーストの表示，「バッグシステム」「HOLD」「SRS」「ロックダウン」等のシステムの実装）
 *   それに加え，
 */
#pragma once
#include "DxLib.h"
#include "SceneMgr.h"
#include "include.h"

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinst2, LPSTR lps, int nCmd) {
  ChangeWindowMode(TRUE), DxLib_Init(),
      SetDrawScreen(DX_SCREEN_BACK); // ウィンドウモード変更と初期化と裏画面設定

  SetGraphMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, 32);
  AddFontResourceEx("Fonts/nicokaku_v1.ttf", FR_PRIVATE, NULL);
  // ChangeFont("ニコ角", DX_CHARSET_DEFAULT);
  // AddFontResourceEx("fonts/Ronde-B_square.otf", FR_PRIVATE, NULL);
  // ChangeFont("ロンド B スクエア", DX_CHARSET_DEFAULT);
  SceneMgr sceneMgr;
  sceneMgr.Initialize();

  // メイン処理
  //  while( 裏画面を表画面に反映, メッセージ処理, 画面クリア, キーの状態更新)
  while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 &&
         gpUpdateKey() == 0) {

    sceneMgr.Update(); // 更新
    sceneMgr.Draw();   // 描画
  }

  DxLib_End(); // DXライブラリ終了処理
  return 0;
}