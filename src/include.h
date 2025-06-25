#pragma once
#include "DxLib.h"
#define WINDOW_SIZE_X 1080
#define WINDOW_SIZE_Y 720

typedef enum {
  PAD_DOWN,
  PAD_LEFT,
  PAD_RIGHT,
  PAD_UP,
  PAD_1,
  PAD_2,
  PAD_3,
  PAD_4,
  PAD_5,
  PAD_6,
  PAD_7,
  PAD_8,
  PAD_9,
  PAD_10,
  PAD_11,
  PAD_12,
  PAD_13,
} ePad;

extern int Key[256]; // キーが押されているフレーム数を格納する
// extern int Key[28]; // キーが押されているフレーム数を格納する

// キーの入力状態を更新する
// Ex: if(Key[KEY_INPUT_RIGHT] == 1){ x++; }
int gpUpdateKey();