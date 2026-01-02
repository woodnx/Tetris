#include "Mino.h"

Mino::Mino(Field& field):
    StaticMino(Coordinates(0, 0), BLOCK_SIZE),
    field(field),
    local(Coordinates(0, 0)) {}

Mino& Mino::operator=(const Mino& other) {
  if (this != &other) {
    StaticMino::operator=(other);
    this->local      = other.local;
    this->global     = other.global;
    this->rotate_dir = other.rotate_dir;
    this->use_spin   = other.use_spin;
    this->last_srs   = other.last_srs;
  }
  return *this;
}

void Mino::init() {
  fill_layout(BlockId::Empty);
  this->rotate_dir = 0;
  this->use_spin   = false;
  this->last_srs   = 0;
}

Coordinates Mino::global_coord() {
  return this->global;
}

void Mino::generate(shared_ptr<BlockId> generate_mino_num, Coordinates l) {
  fill_layout(BlockId::Empty);
  this->local      = l;
  this->global     = field.local_to_global(l);
  this->rotate_dir = 0;

  StaticMino::generate(generate_mino_num);
}

void Mino::rotate(bool is_right) {
  Mino tmp_mino    = *this;
  int tmp_rota_dir = rotate_dir;
  tmp_mino.Blocks::rotate(is_right);

  if (!tmp_mino.collision()) {
    Blocks::rotate(is_right);
  } else if (tmp_mino.super_rotate(tmp_rota_dir)) {
    Blocks::rotate(is_right);
    this->super_rotate(tmp_rota_dir);
  }
  use_spin = true;
}

// 参考1: http://www.terasol.co.jp/プログラミング/6335
// 参考2：https://tetrisch.github.io/main/srs.html
bool Mino::super_rotate(int dir_old) {
  int dx = 0, dy = 0;
  int dir  = this->rotate_dir;
  last_srs = 0;

  if (this->_id != BlockId::Imino) // Iミノ以外の場合
  {
    // 1. 軸を左右に動かす
    // 0が90度（B）の場合は左，-90度（D）の場合は右へ移動
    // 0が0度（A），180度（C）の場合は回転前の方向の逆方向へ移動
    last_srs++;
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
    if (this->collision(Coordinates(dx, dy))) {
      // 2.その状態から軸を上下に動かす
      // 0が90度（B），-90度（D）の場合は上へ移動
      // 0が0度（A），180度（C）の場合は下へ移動
      last_srs++;
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
      if (this->collision(Coordinates(dx, dy))) {
        // 3.元に戻し、軸を上下に2マス動かす
        // 0が90度（B），-90度（D）の場合は下へ移動
        // 0が0度（A），180度（C）の場合は上へ移動
        dx = 0;
        dy = 0;
        last_srs++;
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
        if (this->collision(Coordinates(dx, dy))) {
          // 4.その状態から軸を左右に動かす
          // 0が90度（B）の場合は左，-90度（D）の場合は右へ移動
          // 0が0度（A），180度（C）の場合は回転した方向の逆へ移動
          last_srs++;
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
          if (this->collision(Coordinates(dx, dy)))
            return false;
        }
      }
    }
  } else {
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
    if (this->collision(Coordinates(dx, dy))) {
      // 2. 軸を左右に動かす
      // 0が90度（B）の場合は左，-90度（D）の場合は右へ移動（枠にくっつく）
      // 0が0度（A），180度（C）の場合は回転した方向へ移動
      // 180度は２マス移動
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
      if (this->collision(Coordinates(dx, dy))) {
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
            if (
              (dir_old == 0 && dir == 3) || (dir_old == 3 && dir == 2) ||
              (dir_old == 2 && dir == 1) || (dir_old == 1 && dir == 0)) {
              dy *= 2;
            }

            break;
          case 3:
            dx = p1dx;
            dy = -1;
            if (
              (dir_old == 0 && dir == 3) || (dir_old == 3 && dir == 2) ||
              (dir_old == 2 && dir == 1) || (dir_old == 1 && dir == 0)) {
              dy *= 2;
            }

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
            if (
              (dir_old == 3 && dir == 0) || (dir_old == 0 && dir == 1) ||
              (dir_old == 1 && dir == 2) || (dir_old == 2 && dir == 3)) {
              dy *= 2;
            }
            break;
        }

        if (this->collision(Coordinates(dx, dy))) {
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
              if (
                (dir_old == 3 && dir == 0) || (dir_old == 0 && dir == 1) ||
                (dir_old == 1 && dir == 2) || (dir_old == 2 && dir == 3)) {
                dy *= 2;
              }
              break;
            case 3:
              dx = p2dx;
              dy = 1;
              if (
                (dir_old == 3 && dir == 0) || (dir_old == 0 && dir == 1) ||
                (dir_old == 1 && dir == 2) || (dir_old == 2 && dir == 3)) {
                dy *= 2;
              }
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
              if (
                (dir_old == 0 && dir == 3) || (dir_old == 3 && dir == 2) ||
                (dir_old == 2 && dir == 1) || (dir_old == 1 && dir == 0)) {
                dy *= 2;
              }

              break;
          }
          if (this->collision(Coordinates(dx, dy)))
            return false;
        }
      }
    }
  }
  this->move(Coordinates(dx, dy));

  return true;
}

void Mino::move(Coordinates d, bool with_collision) {
  if (!with_collision) {
    this->global = field.local_to_global(local = local.add(d));
  } else if (!this->collision(d)) {
    this->move(d);
  }
  use_spin = false;
}

void Mino::hard_drop() {
  Coordinates vector = Coordinates(0, 1);

  while (!this->collision(vector)) {
    this->move(vector);
  }
}

int Mino::check_t_spin() {
  if (this->_id != BlockId::Tmino)
    return 0;

  array<Coordinates, 4> target = {
    Coordinates(0, 0),
    Coordinates(0, 2),
    Coordinates(2, 2),
    Coordinates(2, 0),
  };
  array<int, 2> t_spin_target_idx = {
    (rotate_dir + 0) % 4,
    (rotate_dir + 1) % 4,
  };

  int filled_count      = 0;
  int filled_mini_count = 0;
  for (int i = 0; i < 4; i++) {
    if (field.contain_mino(target[i])) {
      filled_count++;

      // judging mini
      if (i == t_spin_target_idx[0] || i == t_spin_target_idx[1]) {
        filled_mini_count++;
      }
    }
  }

  if (filled_count >= 3 && use_spin)
    return 1; // t-spin
  else if (last_srs == 4)
    return 1; // t-spin
  else if (filled_mini_count == 2)
    return 2; // t-spin mini
  else
    return 0;
}

bool Mino::collision() {
  int x, y;
  for (y = 0; y < height(); y++) {
    for (x = 0; x < width(); x++) {
      Coordinates c = Coordinates(x, y);
      Coordinates coll =
        field.global_to_local(global.add(c.mult(block_size())));
      if (
        layout[y][x]->id > BlockId::Empty->id &&
        field.get_field_value(coll) != BlockId::Empty) {
        return true;
      }
    }
  }
  return false;
}

bool Mino::collision(Coordinates diff) {
  for (int y = 0; y < height(); y++) {
    for (int x = 0; x < width(); x++) {
      Coordinates c = Coordinates(x, y);
      Coordinates coll =
        field.global_to_local(global.add(c.mult(block_size()))).add(diff);
      if (
        layout[y][x]->id > BlockId::Empty->id &&
        field.get_field_value(coll) != BlockId::Empty) {
        return true;
      }
    }
  }
  return false;
}

void Mino::transcribe() {
  for (int y = 0; y < height(); y++) {
    for (int x = 0; x < width(); x++) {
      Coordinates c = Coordinates(x, y);
      Coordinates l = field.global_to_local(global).add(c);

      if (layout[y][x] != BlockId::Empty) {
        field.set_field_value(l, layout[y][x]);
      }
    }
  }
}

void Mino::draw_status() {
  // DrawFormatString(250, 0, GetColor(255, 255, 255), "global.x:%d,
  // global.y:%d", global.x, global.y); DrawFormatString(250, 20,
  // GetColor(255, 255, 255), "local.x:%d, local.y:%d", local.x, local.y);
  DrawFormatString(
    250,
    30,
    GetColor(255, 255, 255),
    (TCHAR*) "rotate_dir :%d",
    rotate_dir);
}
