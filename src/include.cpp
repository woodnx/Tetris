#include "include.h"

extern int Key[256] = {0}; // キーが押されているフレーム数を格納する
// extern int Key[28] = { 0 }; // キーが押されているフレーム数を格納する

int gpUpdateKey() {
  // int Pad = GetJoypadInputState(DX_INPUT_KEY_PAD1); //入力状態をPadに格納 for
  // (int i = 0; i < 28; i++) {      //ボタン28個分ループ
  //     if (Pad & (1<<i)) {             //ボタンiの入力フラグが立っていたら
  //         Key[i]++;
  //     }
  //     else {                  // 押されていなければ
  //         Key[i] = 0;         // 0にする
  //     }
  // }
  char tmpKey[256];          // 現在のキーの入力状態を格納する
  GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
  for (int i = 0; i < 256; i++) {
    if (tmpKey[i] != 0) { // i番のキーコードに対応するキーが押されていたら
      Key[i]++; // 加算
    }
    else {        // 押されていなければ
      Key[i] = 0; // 0にする
    }
  }
  return 0;
}