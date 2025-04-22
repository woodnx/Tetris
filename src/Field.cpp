#include "Field.h"

Field::Field(int x, int y)
    : Blocks({x, y}, FIELD_ASIDE_X, FIELD_ASIDE_Y, BLOCK_SIZE) {
  int i, j;
  for (i = 0; i < FIELD_ASIDE_Y; i++) {
    for (j = 0; j < FIELD_ASIDE_X; j++) {
      if ((j == 0 || j == FIELD_ASIDE_X - 1 || i == 0 || i == FIELD_ASIDE_Y - 1
          )) {
        this->layout[i][j] = -1;
      }
    }
  }
  color      = GetColor(255, 255, 255);
  grid_color = GetColor(64, 64, 64);
}

void Field::InitField() {
  int i, j;
  for (i = 0; i < FIELD_ASIDE_Y; i++) {
    for (j = 0; j < FIELD_ASIDE_X; j++) {
      if ((j == 0 || j == FIELD_ASIDE_X - 1 || i == 0 || i == FIELD_ASIDE_Y - 1
          )) {
        this->layout[i][j] = -1;
      }
      else {
        this->layout[i][j] = 0;
      }
    }
  }
}

bool Field::judgeLineFull(int line_num) {
  int i = 1;
  while (layout[line_num][i] != 0) {
    if (i == FIELD_SIDE_X)
      return true;
    i++;
  }
  return false;
}

void Field::OneLineErase(int line_num) {
  for (int i = 1; i <= FIELD_SIDE_X; i++) {
    this->layout[line_num][i] = 0;
  }
}

void Field::shiftLine(int line_num) {
  int i, j;
  int tmpline[FIELD_SIDE_X + 1] = {0};
  for (i = line_num; i > 0; i--) {
    for (j = 1; j <= FIELD_SIDE_X; j++) {
      tmpline[j]       = layout[i - 1][j];
      layout[i][j]     = 0;
      layout[i - 1][j] = 0;
      layout[i][j]     = tmpline[j];
    }
  }
}

/// <summary>
/// 列の消去と移動
/// </summary>
/// <returns>消去した列の数</returns>
int Field::LineEraseAndShift() {
  int erase_linenum = 0;
  for (int i = 1; i <= FIELD_SIDE_Y; i++) {
    if (judgeLineFull(i)) {
      OneLineErase(i);
      shiftLine(i);
      erase_linenum++;
    }
  }
  return erase_linenum;
}

/// <summary>
/// フィールド配列のx要素をウィンドウx座標に変換
/// </summary>
/// <param name="elem_x">フィールド配列のx要素</param>
/// <returns>ウィンドウx座標</returns>
int Field::elemXToCoordX(int elem_x) {
  return window.x + elem_x * block_size();
}

/// <summary>
/// フィールド配列のy要素をウィンドウy座標に変換
/// </summary>
/// <param name="elem_y">フィールド配列のy要素</param>
/// <returns>ウィンドウy座標</returns>
int Field::elemYToCoordY(int elem_y) {
  return window.y + elem_y * block_size();
}

/// <summary>
/// ウィンドウx座標をフィールド配列のx要素に変換
/// </summary>
/// <param name="coord_x">ウィンドウx座標</param>
/// <returns>フィールド配列のx要素</returns>
int Field::coordXToElemX(int x) {
  int size = block_size();
  return (x - window.x) / size;
}

/// <summary>
/// ウィンドウy座標をフィールド配列のy要素に変換
/// </summary>
/// <param name="coord_y">ウィンドウy座標</param>
/// <returns>フィールド配列のy要素</returns>
int Field::coordYToElemY(int y) {
  int size = block_size();
  return (y - window.y) / size;
}

/// <summary>
/// 指定配列要素のフィールドの値を返す
/// </summary>
/// <param name="elem_x">フィールド配列のx要素</param>
/// <param name="elem_y">フィールド配列のy要素</param>
/// <returns></returns>
int Field::getFieldValue(int elem_x, int elem_y) {
  if (elem_x < 0 || elem_x > FIELD_ASIDE_X || elem_y < 0 ||
      elem_y > FIELD_ASIDE_Y)
    return -1;
  else
    return this->layout[elem_y][elem_x];
}

/// <summary>
/// フィールドの指定配列要素に値を書き込む
/// </summary>
/// <param name="elem_x">フィールド配列のx要素</param>
/// <param name="elem_y">フィールド配列のy要素</param>
/// <param name="value">書き込む値</param>
void Field::setFieldValue(int elem_x, int elem_y, int value) {
  layout[elem_y][elem_x] = value;
}

/// <summary>
/// フィールドの指定要素にミノが存在するか判定
/// </summary>
/// <param name="elem_x">　フィールド配列のx要素</param>
/// <param name="elem_y">　フィールド配列のy要素</param>
/// <returns>
/// ミノが存在する true　ミノが存在しない false
/// </returns>
bool Field::containMino(int elem_x, int elem_y) {
  if (this->layout[elem_y][elem_x] > 0)
    return true;
  else
    return false;
}

void Field::drawField() {
  int i, j;

  for (i = 5; i <= FIELD_SIDE_Y; i++) {
    for (j = 1; j <= FIELD_SIDE_X; j++) {
      DrawBox(
          this->window.x + block_size() * j, this->window.y + block_size() * i,
          this->window.x + block_size() * (j + 1),
          window.y + block_size() * (i + 1), GetColor(0, 0, 0), TRUE
      );
      if (this->layout[i][j] > 0) {
        DrawBox(
            window.x + block_size() * j, window.y + block_size() * i,
            window.x + block_size() * (j + 1) - 1,
            window.y + block_size() * (i + 1) - 1,
            selectColor(this->layout[i][j]), TRUE
        );
      }
      else if (this->layout[i][j] == -1) {
        DrawBox(
            window.x + block_size() * j, window.y + block_size() * i,
            window.x + block_size() * (j + 1) - 1,
            window.y + block_size() * (i + 1) - 1, GetColor(255, 255, 255), TRUE
        );
      }
      DrawBox(
          window.x + block_size() * j, window.y + block_size() * i,
          window.x + block_size() * (j + 1), window.y + block_size() * (i + 1),
          this->grid_color, FALSE
      );
    }
  }
}