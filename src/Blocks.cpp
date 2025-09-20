#include "Blocks.h"

Blocks::Blocks(
  Coordinates global,
  unsigned int width,
  unsigned int height,
  unsigned int size):
    global(global),
    _width(width),
    _height(height),
    _size(size),
    rotate_dir(0) {
  layout.resize(height);
  for (int i = 0; i < height; i++) {
    layout.at(i).resize(width);
  }
}

int Blocks::rotate(bool is_right) {
  if (layout.size() == 0)
    return -1;

  int x, y;
  int X = 0, Y = 0;
  int lr  = is_right ? -1 : 1;
  float a = (this->_width - 1) / 2.0;
  float b = (this->_height - 1) / 2.0;

  vector<vector<shared_ptr<BlockId>>> tmp = copy_layout();
  fill_layout(BlockId::Empty);

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
  if (is_right) {
    rotate_dir++;
    if (rotate_dir >= 4) {
      rotate_dir = rotate_dir % 4;
    }
  } else {
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

void Blocks::init_layout(unsigned int width, unsigned int height) {
  this->_width  = width;
  this->_height = height;

  layout.resize(height);
  for (int i = 0; i < height; i++) {
    layout.at(i).resize(width);
  }

  fill_layout(BlockId::Empty);
}

void Blocks::fill_layout(shared_ptr<BlockId> fill_id) {
  for (int i = 0; i < layout.size(); i++) {
    for (int j = 0; j < layout.at(0).size(); j++) {
      layout[i][j] = fill_id;
    }
  }
}

vector<vector<shared_ptr<BlockId>>> Blocks::copy_layout() {
  vector<vector<shared_ptr<BlockId>>> l;

  l.resize(_height);
  for (int i = 0; i < _height; i++) {
    l.at(i).resize(_width);
    for (int j = 0; j < layout.at(0).size(); j++) {
      l[i][j] = BlockId::Empty;
    }
  }

  for (int y = 0; y < _height; y++) {
    for (int x = 0; x < _width; x++) {
      l[y][x] = layout[y][x];
    }
  }
  return l;
}

int Blocks::draw(bool fill_flag) {
  unsigned int i, j;

  if (_width <= 0 || _height <= 0 || layout.empty())
    return -1;

  for (i = 0; i < _height; i++) {
    for (j = 0; j < _width; j++) {
      // Draw only non-empty cells (compare by id, not pointer identity)
      if (layout[i][j] && layout[i][j]->id > BlockId::Empty->id) {
        int color = layout[i][j]->color();
        DrawBox(
          global.x + _size * j,
          global.y + _size * i,
          global.x + _size * (j + 1) - 1,
          global.y + _size * (i + 1) - 1,
          color,
          fill_flag);
      }
    }
  }
  return 0;
}

int Blocks::draw(bool fill_flag, int alpha) {
  unsigned int i, j;

  if (_width <= 0 || _height <= 0 || layout.empty())
    return -1;

  for (i = 0; i < _height; i++) {
    for (j = 0; j < _width; j++) {
      if (layout[i][j]->id > BlockId::Empty->id) {
        int color = layout[i][j]->color();
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawBox(
          global.x + _size * j,
          global.y + _size * i,
          global.x + _size * (j + 1) - 1,
          global.y + _size * (i + 1) - 1,
          color,
          fill_flag);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードをオフ
      }
    }
  }
  return 0;
}
