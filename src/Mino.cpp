#include "Mino.h"

Mino::Mino(Field *_field): StaticMino(0, 0, BLOCK_SIZE) {
  field         = _field;
  this->field_x = 0, this->field_y = 0;
}

void Mino::initialize() {
  fillLayout(0);
  this->rotate_dir = 0;
}

int Mino::getMinoNum() {
  return num - 1;
}

int Mino::getMinoCoordX() {
  return this->window.x;
}

int Mino::getMinoCoordY() {
  return this->window.y;
}

void Mino::generateMinoWithPos(
    int generate_mino_num, int field_x, int field_y
) {
  fillLayout(0);
  this->field_x = field_x, this->field_y = field_y;
  this->window.x   = field->elemXToCoordX(this->field_x);
  this->window.y   = field->elemYToCoordY(this->field_y);
  this->rotate_dir = 0;

  this->generateMino(generate_mino_num);
}

void Mino::rotateMinoWithCollision(bool right_flag) {
  Mino tmp_mino    = *this;
  int tmp_rota_dir = this->rotate_dir;
  tmp_mino.rotate(right_flag);

  if (!tmp_mino.collisionField()) {
    this->rotate(right_flag);
  }
  else if (tmp_mino.superRotation(tmp_rota_dir, right_flag)) {
    this->rotate(right_flag);
    this->superRotation(tmp_rota_dir, right_flag);
  }
}

bool Mino::superRotation(int dir_old, bool right_flag) {
  // 参考（ほぼ丸パクリ）:http://www.terasol.co.jp/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0/6335
  // 参考２（Iミノ部分の実装）：https://tetrisch.github.io/main/srs.html
  int dx = 0, dy = 0;
  int dir = this->rotate_dir;

  if (this->num != 1) // Iミノ以外の場合
  {
    // 1. 軸を左右に動かす
    // 0が90度（B）の場合は左，-90度（D）の場合は右へ移動
    // 0が0度（A），180度（C）の場合は回転前の方向の逆方向へ移動
    switch (dir) {
      case 1:
        dx = -1;
        break;
      case 3:
        dx = 1;
        break;
      case 0:
      case 2:
        switch (dir_old) {
          case 1: // 回転前が右向き
            dx = 1;
            break;
          case 3: // 回転前が左向き
            dx = -1;
            break;
        }
        break;
    }
    if (this->collisionField(dx, dy)) {
      // 2.その状態から軸を上下に動かす
      // 0が90度（B），-90度（D）の場合は上へ移動
      // 0が0度（A），180度（C）の場合は下へ移動
      switch (dir) {
        case 1:
        case 3:
          dy = -1;
          break;
        case 0:
        case 2:
          dy = 1;
          break;
      }
      if (this->collisionField(dx, dy)) {
        // 3.元に戻し、軸を上下に2マス動かす
        // 0が90度（B），-90度（D）の場合は下へ移動
        // 0が0度（A），180度（C）の場合は上へ移動
        dx = 0;
        dy = 0;
        switch (this->rotate_dir) {
          case 1:
          case 3:
            dy = 2;
            break;
          case 0:
          case 2:
            dy = -2;
            break;
        }
        if (this->collisionField(dx, dy)) {
          // 4.その状態から軸を左右に動かす
          // 0が90度（B）の場合は左，-90度（D）の場合は右へ移動
          // 0が0度（A），180度（C）の場合は回転した方向の逆へ移動
          switch (dir) {
            case 1:
              dx = -1;
              break;
            case 3:
              dx = 1;
              break;
            case 0:
            case 2:
              switch (dir_old) {
                case 1: // 回転前が右向き
                  dx = 1;
                  break;
                case 3: // 回転前が左向き
                  dx = -1;
                  break;
              }
              break;
          }
          if (this->collisionField(dx, dy))
            return false;
        }
      }
    }
  }
  else {
    int p1dx, p2dx;

    // 1. 軸を左右に動かす
    // 0が90度（B）の場合は右，-90度（D）の場合は左へ移動（枠にくっつく）
    // 0が0度（A），180度（C）の場合は回転した方向の逆へ移動 0度は２マス移動
    switch (dir) {
      case 1:
        if (dir_old == 0)
          dx = -2;
        else
          dx = 1;
        break;
      case 3:
        if (dir_old == 2)
          dx = 2;
        else
          dx = -1;
        break;
      case 0:
      case 2:
        switch (dir_old) {
          case 1: // 回転前が右向き
            dx = -1;
            break;
          case 3: // 回転前が左向き
            dx = 1;
            break;
        }
        if (dir == 0)
          dx *= -2;
        break;
    }
    p1dx = dx;
    if (this->collisionField(dx, dy)) {
      // 2. 軸を左右に動かす
      // 0が90度（B）の場合は左，-90度（D）の場合は右へ移動（枠にくっつく）
      // 0が0度（A），180度（C）の場合は回転した方向へ移動 180度は２マス移動
      switch (dir) {
        case 1:
          if (dir_old == 2)
            dx = -2;
          else
            dx = 1;
          break;
        case 3:
          if (dir_old == 0)
            dx = 2;
          else
            dx = -1;
          break;
        case 0:
        case 2:
          switch (dir_old) {
            case 1: // 回転前が右向き
              dx = -1;
              break;
            case 3: // 回転前が左向き
              dx = 1;
              break;
          }
          if (dir == 2)
            dx *= -2;
          break;
      }
      p2dx = dx;
      if (this->collisionField(dx, dy)) {
        // 3. 軸を上下に動かす
        // 0が90度（B）の場合は1を下，-90度（D）の場合は1を上へ移動
        // 0が0度（A），180度（C）の場合は
        // 回転前のミノが右半分にある（B）なら1を上へ
        // 回転前のミノが左半分にある（D）なら2を下へ移動
        // 左回転なら２マス動かす
        switch (dir) {
          case 1:
            dx = p1dx;
            dy = 1;
            if ((dir_old == 0 && dir == 3) || (dir_old == 3 && dir == 2) ||
                (dir_old == 2 && dir == 1) || (dir_old == 1 && dir == 0))
              dy *= 2;
            break;
          case 3:
            dx = p1dx;
            dy = -1;
            if ((dir_old == 0 && dir == 3) || (dir_old == 3 && dir == 2) ||
                (dir_old == 2 && dir == 1) || (dir_old == 1 && dir == 0))
              dy *= 2;
            break;
          case 0:
          case 2:
            switch (dir_old) {
              case 1: // 回転前が右向き
                dx = p1dx;
                dy = -1;
                break;
              case 3: // 回転前が左向き
                dx = p2dx;
                dy = 1;
                break;
            }
            if ((dir_old == 3 && dir == 0) || (dir_old == 0 && dir == 1) ||
                (dir_old == 1 && dir == 2) || (dir_old == 2 && dir == 3))
              dy *= 2;
            break;
        }

        if (this->collisionField(dx, dy)) {
          // 4. 軸を上下に動かす
          // 0が90度（B）の場合は2を上，-90度（D）の場合は2を下へ移動
          // 0が0度（A），180度（C）の場合は
          // 回転前のミノが右半分にある（B）なら2を下へ
          // 回転前のミノが左半分にある（D）なら1を上へ移動
          // 右回転なら２マス動かす
          switch (dir) {
            case 1:
              dx = p2dx;
              dy = -1;
              if ((dir_old == 3 && dir == 0) || (dir_old == 0 && dir == 1) ||
                  (dir_old == 1 && dir == 2) || (dir_old == 2 && dir == 3))
                dy *= 2;
              break;
            case 3:
              dx = p2dx;
              dy = 1;
              if ((dir_old == 3 && dir == 0) || (dir_old == 0 && dir == 1) ||
                  (dir_old == 1 && dir == 2) || (dir_old == 2 && dir == 3))
                dy *= 2;
              break;
            case 0:
            case 2:
              switch (dir_old) {
                case 1: // 回転前が右向き
                  dx = p2dx;
                  dy = 1;
                  break;
                case 3: // 回転前が左向き
                  dx = p1dx;
                  dy = -1;
                  break;
              }
              if ((dir_old == 0 && dir == 3) || (dir_old == 3 && dir == 2) ||
                  (dir_old == 2 && dir == 1) || (dir_old == 1 && dir == 0))
                dy *= 2;
              break;
          }
          if (this->collisionField(dx, dy))
            return false;
        }
      }
    }
  }
  this->moveMino(dx, dy);

  return true;
}

void Mino::moveMino(int dx, int dy) {
  this->window.x = field->elemXToCoordX(field_x += dx);
  this->window.y = field->elemYToCoordY(field_y += dy);
}

void Mino::moveMinoWithCollision(int dx, int dy) {
  if (!this->collisionField(dx, dy)) {
    this->moveMino(dx, dy);
  }
}

void Mino::dropToMaxBottom() {
  while (!this->collisionField(0, 1)) {
    this->moveMino(0, 1);
  }
}

bool Mino::collisionField() {
  int x, y;
  for (y = 0; y < height(); y++) {
    for (x = 0; x < width(); x++) {
      int coll_x = field->coordXToElemX(this->window.x + block_size() * x);
      int coll_y = field->coordYToElemY(this->window.y + block_size() * y);
      if (this->layout[y][x] > 0 && field->getFieldValue(coll_x, coll_y) != 0) {
        return true;
      }
    }
  }
  return false;
}

bool Mino::collisionField(int dx, int dy) {
  int x, y;
  for (y = 0; y < height(); y++) {
    for (x = 0; x < width(); x++) {
      int coll_x = field->coordXToElemX(this->window.x + block_size() * x) + dx;
      int coll_y = field->coordYToElemY(this->window.y + block_size() * y) + dy;
      if (this->layout[y][x] > 0 && field->getFieldValue(coll_x, coll_y) != 0) {
        return true;
      }
    }
  }
  return false;
}

void Mino::transcribeMinoToField() {
  int i, j;
  for (i = 0; i < height(); i++) {
    for (j = 0; j < width(); j++) {
      int elem_x = field->coordXToElemX(this->window.x) + j;
      int elem_y = field->coordYToElemY(this->window.y) + i;
      if (layout[i][j] != 0) {
        field->setFieldValue(elem_x, elem_y, layout[i][j]);
      }
    }
  }
}

void Mino::drawStatus() {
  // DrawFormatString(250, 0, GetColor(255, 255, 255), "window.x:%d,
  // window.y:%d", window.x, window.y); DrawFormatString(250, 20, GetColor(255,
  // 255, 255), "field_x:%d, field_y:%d", field_x, field_y);
  DrawFormatString(
      250, 30, GetColor(255, 255, 255), "rotate_dir :%d", rotate_dir
  );
}
