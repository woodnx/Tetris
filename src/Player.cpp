#include "Player.h"

Player::Player(BaseScene* gameManager, Coordinates global):
    global(global),
    gameManager(gameManager),
    field(Coordinates(global.x, global.y - BLOCK_SIZE * 4)),
    mino(field),
    ghost(field),
    hold(Coordinates(
      global.x - STATIC_BLOCK_SIZE * 4,
      global.y + STATIC_BLOCK_SIZE * 1.5 + 10)),
    generate_place(Coordinates((int) (FIELD_SIDE_X / 2), 4)),
    pre_mino_place(Coordinates((int) (FIELD_SIDE_X / 2), 4)),
    bottom(Coordinates(0, 1)) {
  // 画像・フォントハンドル
  background_handle = LoadGraph((TCHAR*) "images/Back_Cyber_1920.jpg");
  index_font =
    CreateFontToHandle((TCHAR*) "ニコ角", 25, 1, DX_FONTTYPE_ANTIALIASING);
  figure_font =
    CreateFontToHandle((TCHAR*) "ニコ角", 40, 1, DX_FONTTYPE_ANTIALIASING);

  // 各種サウンド
  sound.add("move", "sounds/カーソル移動2.mp3");
  sound.add("rotate", "sounds/決定、ボタン押下35.mp3");
  sound.add("hold", "sounds/決定、ボタン押下40.mp3");
  sound.add("drop", "sounds/カーソル移動7.mp3");
  sound.add("levelup", "sounds/魔王魂 効果音 ワンポイント11.mp3");
  sound.changeAllSoundVolume(70);
  sound.changeVolume("rotate", 60);

  for (int i = 0; i < NEXT_REFER_SIZE; i++) {
    next.push_back(StaticMino(Coordinates(
      global.x + FIELD_SIDE_X * BLOCK_SIZE + 70,
      global.y + i * STATIC_BLOCK_SIZE * 2.8 + 35)));
  }
  init();
}

void Player::init() {
  field.init();
  mino.init();
  ghost.init();
  hold.init();
  bag.init();

  for (int i = 0; i < NEXT_REFER_SIZE; i++) {
    next[i].init();
  }

  level           = 1;
  score           = 0;
  ren_count       = 0;
  drop_speed      = 60;
  is_back_to_back = false;
  is_t_spin       = false;
  soft_drop_cells = 0;
  hard_drop_cells = 0;

  lockdown_count   = 0;
  autorepeat_count = 0;
  is_autorepeat    = false;

  clear_line_count  = 0;
  total_clear_lines = 0;
  levelup_count     = 0;

  can_hold    = true;
  can_control = true;
}

void Player::move_mino(bool is_right) {
  Coordinates vector(is_right ? 1 : -1, 0);

  if (
    mino.global_coord().x != pre_mino_place.x &&
    mino.global_coord().y != pre_mino_place.y) {
    lockdown_count = 0;
  }

  if (autorepeat_count == 0) {
    mino.move(vector, true);
    sound.play("move", DX_PLAYTYPE_BACK);
    autorepeat_count++;
  } else if (is_autorepeat && gameManager->frame_count() % 3 == 0) {
    mino.move(vector, true);
    sound.play("move", DX_PLAYTYPE_BACK);
  } else if (autorepeat_count == 9) {
    is_autorepeat = true;
  } else {
    autorepeat_count++;
  }
}

void Player::rotate_mino(bool is_right) {
  mino.rotate(is_right);
  lockdown_count = 0;
}

void Player::hard_drop() {
  pre_mino_place = mino.global_coord();
  mino.hard_drop();
  generate_mino(bag.increase(), true);

  can_hold = true;
  hard_drop_cells += field.global_to_local(pre_mino_place).y - bottom.y;
}

void Player::soft_drop() {
  Coordinates vector(0, 1);

  if (gameManager->frame_count() % 2 == 0) {
    mino.move(vector, true);
    sound.play("move", DX_PLAYTYPE_BACK);
    soft_drop_cells++;
  }
}

void Player::hold_mino() {
  if (can_hold) {
    shared_ptr<BlockId> tmp_hold_mino_id = hold.id();
    hold.generate(mino.id());

    sound.play("hold", DX_PLAYTYPE_BACK);
    if (tmp_hold_mino_id == BlockId::Null) {
      generate_mino(bag.increase(), false);
    } else {
      generate_mino(tmp_hold_mino_id, false);
    }

    can_hold        = false;
    hard_drop_cells = 0;
    soft_drop_cells = 0;
  }
}

void Player::make_ghost() {
  ghost = mino;
  ghost.hard_drop();
}

void Player::set_next() {
  for (int i = 0; i < Bag::NEXT_REFERS; i++) {
    next[i].generate(bag.get_mino_id(i));
  }
}

void Player::generate_mino(shared_ptr<BlockId> mino_id, bool is_transcribe) {
  if (is_transcribe) {
    mino.transcribe();
  }
  mino.generate(mino_id, generate_place);

  set_next();
}

int Player::calc_t_spin_base(int t_spin_type, int cleared_lines) {
  if (t_spin_type == 0)
    return 0;

  if (t_spin_type == 1) { // normal t_spin
    switch (cleared_lines) {
      case 0:
        return 0;
      case 1:
        return 800;
      case 2:
        return 1200;
      case 3:
        return 1600;
      default:
        return 0;
    }
  } else if (t_spin_type == 2) { // t_spin mini
    switch (cleared_lines) {
      case 0:
        return 0;
      case 1:
        return 200;
      default:
        return 0;
    }
  }

  return 0;
}

void Player::calc_score_and_level() {
  int tmp_clear_line_count = clear_line_count;
  clear_line_count         = field.clear_lines();
  total_clear_lines += clear_line_count;
  int t_spin_type = mino.check_t_spin();
  int t_spin_base = calc_t_spin_base(t_spin_type, clear_line_count);

  if (tmp_clear_line_count != 0 && clear_line_count != 0)
    ren_count++;
  else
    ren_count = 0;

  int line_point    = clear_line_table[clear_line_count] * level;
  int t_score_point = t_spin_base * level;
  int ren_point     = 50 * ren_count * level;
  int drop_point    = soft_drop_cells + 2 * hard_drop_cells;
  score += (line_point + t_score_point + ren_point + drop_point);

  int tmp_level = level;
  level         = total_clear_lines / 10 + 1;

  if (level != tmp_level) {
    sound.play("levelup", DX_PLAYTYPE_BACK);
    if (drop_speed != 1) {
      drop_speed /= 1.5;
    } else
      drop_speed = 1;
  }
  soft_drop_cells = 0;
  hard_drop_cells = 0;
}

void Player::start_game() {
  init();
  generate_mino(bag.increase(), false);
  hold.generate(BlockId::Empty);
}

int Player::judge_game() {
  if (
    field.contain_mino(generate_place) ||
    field.contain_mino(Coordinates(generate_place.x + 1, generate_place.y))) {
    return -1;
  } else if (total_clear_lines >= max_lines) {
    return 1;
  }

  return 0;
}

int Player::update(bool key_pressed) {
  if (!key_pressed) {
    autorepeat_count = 0;
    is_autorepeat    = false;
  }
  calc_score_and_level();

  if (mino.collision(bottom)) {
    if (lockdown_count >= 30) {
      lockdown_count = 0;
      generate_mino(bag.increase(), true);

      can_hold = true;
    }
    lockdown_count++;
  }
  make_ghost();

  return judge_game();
}

void Player::draw() {
  DrawGraph(0, 0, background_handle, TRUE);
  int drx = global.x - STATIC_BLOCK_SIZE * 5, dry = global.y;
  DrawRoundRect(
    drx,
    dry,
    drx + STATIC_BLOCK_SIZE * 5 + 10,
    dry + STATIC_BLOCK_SIZE * 4 + 10,
    10,
    10,
    GetColor(0, 0, 0),
    TRUE);
  drx = global.x + FIELD_SIDE_X * BLOCK_SIZE + 50, dry = global.y;
  DrawRoundRect(
    drx,
    dry,
    drx + STATIC_BLOCK_SIZE * 5 + 10,
    dry + STATIC_BLOCK_SIZE * 6 * 3,
    10,
    10,
    GetColor(0, 0, 0),
    TRUE);

  field.draw();
  ghost.draw(false);
  mino.draw(true);
  if (can_hold) {
    hold.draw(true);
  } else {
    hold.draw(true, 100);
  }

  for (int i = 0; i < NEXT_REFER_SIZE; i++) {
    next[i].draw(true);
  }

  DrawStringToHandle(
    global.x - STATIC_BLOCK_SIZE * 5 + 5,
    global.y + 5,
    (TCHAR*) "HOLD",
    GetColor(255, 255, 255),
    index_font);
  DrawStringToHandle(
    global.x + FIELD_SIDE_X * BLOCK_SIZE + 50 + 5,
    global.y + 5,
    (TCHAR*) "NEXT",
    GetColor(255, 255, 255),
    index_font);
  DrawStringToHandle(
    global.x + FIELD_SIDE_X * BLOCK_SIZE + 50,
    global.y + STATIC_BLOCK_SIZE * 6 * 3,
    (TCHAR*) "SCORE",
    GetColor(255, 255, 255),
    index_font);
  DrawFormatStringToHandle(
    global.x + FIELD_SIDE_X * BLOCK_SIZE + 70,
    global.y + STATIC_BLOCK_SIZE * 6 * 3 + 20,
    GetColor(255, 255, 255),
    figure_font,
    (TCHAR*) "%d",
    score);
  DrawStringToHandle(
    global.x + FIELD_SIDE_X * BLOCK_SIZE + 50,
    global.y + STATIC_BLOCK_SIZE * 6 * 3 + 60,
    (TCHAR*) "HIGH SCORE",
    GetColor(255, 255, 255),
    index_font);
  // DrawFormatStringToHandle(x + FIELD_SIDE_X * BLOCK_SIZE + 70, y +
  // STATIC_BLOCK_SIZE * 6 * 3 + 80, GetColor(255, 255, 255), figure_font,
  // "%d", highscore);
  DrawStringToHandle(
    global.x + FIELD_SIDE_X * BLOCK_SIZE + 50,
    global.y + STATIC_BLOCK_SIZE * 6 * 3 + 120,
    (TCHAR*) "LEVEL",
    GetColor(255, 255, 255),
    index_font);
  DrawFormatStringToHandle(
    global.x + FIELD_SIDE_X * BLOCK_SIZE + 70,
    global.y + STATIC_BLOCK_SIZE * 6 * 3 + 140,
    GetColor(255, 255, 255),
    figure_font,
    (TCHAR*) "%d",
    level);
  DrawStringToHandle(
    global.x + FIELD_SIDE_X * BLOCK_SIZE + 50,
    global.y + STATIC_BLOCK_SIZE * 6 * 3 + 180,
    (TCHAR*) "LINES",
    GetColor(255, 255, 255),
    index_font);
  DrawFormatStringToHandle(
    global.x + FIELD_SIDE_X * BLOCK_SIZE + 70,
    global.y + STATIC_BLOCK_SIZE * 6 * 3 + 200,
    GetColor(255, 255, 255),
    figure_font,
    (TCHAR*) "%d",
    total_clear_lines);
}

void Player::finalize() {
  InitFontToHandle();
  sound.finalize();
}
