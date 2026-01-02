#include "Field.h"

Field::Field(Coordinates global):
    Blocks(global, FIELD_ASIDE_X, FIELD_ASIDE_Y, BLOCK_SIZE) {
  for (int y = 0; y < FIELD_ASIDE_Y; y++) {
    for (int x = 0; x < FIELD_ASIDE_X; x++) {
      if (
        x == 0 || x == FIELD_ASIDE_X - 1 || y == 0 || y == FIELD_ASIDE_Y - 1) {
        this->layout[y][x] = BlockId::Null;
      }
    }
  }
  color      = GetColor(255, 255, 255);
  grid_color = GetColor(64, 64, 64);
}

void Field::init() {
  for (int y = 0; y < FIELD_ASIDE_Y; y++) {
    for (int x = 0; x < FIELD_ASIDE_X; x++) {
      if (
        x == 0 || x == FIELD_ASIDE_X - 1 || y == 0 || y == FIELD_ASIDE_Y - 1) {
        this->layout[y][x] = BlockId::Null;
      } else {
        this->layout[y][x] = BlockId::Empty;
      }
    }
  }
}

bool Field::judge_clearable(int line_index) {
  for (int x = 1; x <= FIELD_SIDE_X; x++) {
    if (
      !layout[line_index][x] || layout[line_index][x]->id == BlockId::Empty->id)
      return false;
  }
  return true;
}

void Field::clear_line(int line_index) {
  for (int i = 1; i <= FIELD_SIDE_X; i++) {
    this->layout[line_index][i] = BlockId::Empty;
  }
}

void Field::shift_line(int line_index) {
  vector<shared_ptr<BlockId>> tmp_line;
  tmp_line.resize(FIELD_SIDE_X);

  for (int i = line_index; i > 0; i--) {
    for (int j = 1; j <= FIELD_SIDE_X; j++) {
      tmp_line[j - 1]  = layout[i - 1][j];
      layout[i][j]     = BlockId::Empty;
      layout[i - 1][j] = BlockId::Empty;
      layout[i][j]     = tmp_line[j - 1];
    }
  }
}

int Field::clear_lines() {
  int erase_linenum = 0;
  for (int i = 1; i <= FIELD_SIDE_Y; i++) {
    if (judge_clearable(i)) {
      clear_line(i);
      shift_line(i);
      erase_linenum++;
    }
  }
  return erase_linenum;
}

Coordinates Field::local_to_global(Coordinates l) {
  return global.add(l.mult(block_size()));
}

Coordinates Field::global_to_local(Coordinates g) {
  return (g.sub(global)).div(block_size());
}

shared_ptr<BlockId> Field::get_field_value(Coordinates local) {
  if (
    local.x < 0 || local.x > FIELD_ASIDE_X || local.y < 0 ||
    local.y > FIELD_ASIDE_Y)
    return BlockId::Null;
  else
    return this->layout[local.y][local.x];
}

void Field::set_field_value(Coordinates local, shared_ptr<BlockId> value) {
  layout[local.y][local.x] = value;
}

bool Field::contain_mino(Coordinates local) {
  if (this->layout[local.y][local.x] != BlockId::Empty)
    return true;
  else
    return false;
}

void Field::draw() {
  int i, j;

  for (i = 5; i <= FIELD_SIDE_Y; i++) {
    for (j = 1; j <= FIELD_SIDE_X; j++) {
      DrawBox(
        global.x + block_size() * j,
        global.y + block_size() * i,
        global.x + block_size() * (j + 1),
        global.y + block_size() * (i + 1),
        GetColor(0, 0, 0),
        TRUE);
      if (this->layout[i][j] && this->layout[i][j]->id == BlockId::Null->id) {
        DrawBox(
          global.x + block_size() * j,
          global.y + block_size() * i,
          global.x + block_size() * (j + 1) - 1,
          global.y + block_size() * (i + 1) - 1,
          GetColor(255, 255, 255),
          TRUE);
      } else if (
        this->layout[i][j] && this->layout[i][j]->id != BlockId::Empty->id) {
        DrawBox(
          global.x + block_size() * j,
          global.y + block_size() * i,
          global.x + block_size() * (j + 1) - 1,
          global.y + block_size() * (i + 1) - 1,
          this->layout[i][j]->color(),
          TRUE);
      }
      DrawBox(
        global.x + block_size() * j,
        global.y + block_size() * i,
        global.x + block_size() * (j + 1),
        global.y + block_size() * (i + 1),
        this->grid_color,
        FALSE);
    }
  }
}