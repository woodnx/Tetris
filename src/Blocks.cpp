#include "Blocks.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="width">vector配列の横の大きさ</param>
/// <param name="height">vector配列の縦の大きさ</param>
/// <param name="size">1ブロックの縦横の大きさ[px]</param>
Blocks::Blocks(
    Position window, unsigned int width, unsigned int height, unsigned int size
) {
  this->_width = width, this->_height = height;
  this->_size  = size;
  this->window = window;
  rotate_dir   = 0;

  layout.resize(height);
  for (int i = 0; i < height; i++) {
    layout.at(i).resize(width);
  }
}

/// <summary>
/// vectorを破壊的に90°回転する
/// </summary>
/// <param name="dangle"></param>
/// <returns></returns>
int Blocks::rotate(bool right_flag) {
  if (layout.size() == 0)
    return -1;

  int x, y;
  int X = 0, Y = 0;
  int lr                  = right_flag ? -1 : 1;
  float a                 = (this->_width - 1) / 2.0;
  float b                 = (this->_height - 1) / 2.0;
  vector<vector<int>> tmp = this->layout;

  fillLayout(0);

  for (y = 0; y < _height; y++) {
    for (x = 0; x < _width; x++) {
      // 回転行列を使って回転
      // 変換式は下の通り（ x -> X, y -> Y )
      // --------------------------------------------------
      // | X:  cos(90)( x - a ) - sin(90)( y - b ) + a	|
      // |	 = -y + b + a
      // | |
      // | | Y:  sin(90)( x - a ) + cos(90)( y - b ) + b	| |	 =  x -
      // a + b								|
      // --------------------------------------------------

      X = (int) (lr * (-y + a) + b);
      Y = (int) (lr * (x - a) + b);
      if (X < 0 || Y < 0 || X > _width || Y > _height)
        continue;

      layout[y][x] = tmp[Y][X];
    }
  }
  if (right_flag) {
    rotate_dir++;
    if (rotate_dir >= 4) {
      rotate_dir = rotate_dir % 4;
    }
  }
  else {
    rotate_dir--;
    if (rotate_dir <= -1) {
      rotate_dir = (4 + rotate_dir) % 4;
    }
  }

  return 0;
}

unsigned int Blocks::width() {
  return _width;
}

unsigned int Blocks::height() {
  return _height;
}

unsigned int Blocks::block_size() {
  return _size;
}

/// <summary>
///
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="value"></param>
/// <returns></returns>
int Blocks::initLayout(unsigned int width, unsigned int height) {
  if (width <= 0 || height <= 0 || layout.empty())
    return -1;
  this->_width  = width;
  this->_height = height;

  layout.resize(height);
  for (int i = 0; i < height; i++) {
    layout.at(i).resize(width);
  }

  return 0;
}

int Blocks::fillLayout(int value) {
  if (_width <= 0 || _height <= 0 || layout.empty())
    return -1;

  for (int i = 0; i < layout.size(); i++) {
    for (int j = 0; j < layout.at(0).size(); j++) {
      layout[i][j] = value;
    }
  }
  return 0;
}

int Blocks::selectColor(int color_num) {
  switch (color_num) {
    case Imino:
      return GetColor(0, 191, 255);
    case Lmino:
      return GetColor(255, 165, 0);
    case Jmino:
      return GetColor(65, 105, 225);
    case Smino:
      return GetColor(50, 205, 50);
    case Zmino:
      return GetColor(255, 99, 71);
    case Omino:
      return GetColor(0xff, 0xff, 0x66);
    case Tmino:
      return GetColor(218, 112, 214);
    default:
      return -1;
      break;
  }
  return 0;
}

/// <summary>
/// ブロック配列を描画する
/// </summary>
/// <param name="color"></param>
/// <param name="fill_flag"></param>
/// <returns></returns>
int Blocks::draw(bool fill_flag) {
  unsigned int i, j;

  if (_width <= 0 || _height <= 0 || layout.empty())
    return -1;

  for (i = 0; i < _height; i++) {
    for (j = 0; j < _width; j++) {
      if (this->layout[i][j] > Empty) {
        int color = selectColor(layout[i][j]);
        DrawBox(
            window.x + _size * j, window.y + _size * i,
            window.x + _size * (j + 1) - 1, window.y + _size * (i + 1) - 1,
            color, fill_flag
        );
      }
    }
  }
  return 0;
}

/// <summary>
/// ブロック配列を描画する
/// </summary>
/// <param name="color"></param>
/// <param name="fill_flag"></param>
/// <param name="alpha"></param>
/// <returns></returns>
int Blocks::draw(bool fill_flag, int alpha) {
  unsigned int i, j;

  if (_width <= 0 || _height <= 0 || layout.empty())
    return -1;

  for (i = 0; i < _height; i++) {
    for (j = 0; j < _width; j++) {
      if (this->layout[i][j] > 0) {
        int color = selectColor(layout[i][j]);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawBox(
            window.x + _size * j, window.y + _size * i,
            window.x + _size * (j + 1) - 1, window.y + _size * (i + 1) - 1,
            color, fill_flag
        );
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードをオフ
      }
    }
  }
  return 0;
}
