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
#include "DxLib.h"
#include "SceneMgr.h"
#include "include.h"

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinst2, LPSTR lps, int nCmd) {
  SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
  ChangeWindowMode(TRUE), DxLib_Init(),
    SetDrawScreen(DX_SCREEN_BACK); // ウィンドウモード変更と初期化と裏画面設定

  // 垂直同期を有効化（ちらつき抑制）
  SetWaitVSyncFlag(TRUE);

  SetGraphMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, 32);
  AddFontResourceEx((TCHAR*) "fonts/nicokaku_v1.ttf", FR_PRIVATE, NULL);
  // ChangeFont("ニコ角", DX_CHARSET_DEFAULT);
  // AddFontResourceEx("fonts/Ronde-B_square.otf", FR_PRIVATE, NULL);
  // ChangeFont("ロンド B スクエア", DX_CHARSET_DEFAULT);
  SceneMgr sceneMgr;
  sceneMgr.initialize();

  // メイン処理: 1フレームの最後に ScreenFlip() するのが定石
  while (ProcessMessage() == 0) {
    SetDrawScreen(DX_SCREEN_BACK); // 念のため裏画面を明示
    ClearDrawScreen();             // 画面をクリア
    if (gpUpdateKey() != 0)
      break; // 入力更新（エラーで抜ける）

    sceneMgr.update(); // 更新
    sceneMgr.draw();   // 描画

    ScreenFlip(); // 裏画面→表画面
  }

  DxLib_End(); // DXライブラリ終了処理
  return 0;
}