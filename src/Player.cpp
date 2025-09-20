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

  for (int i = 0; i < NEXT_REFER_SIZE; i++) {
    next[i].init();
  }

  level      = 1;
  score      = 0;
  ren_num    = 0;
  drop_speed = 60;

  lockdown_count   = 0;
  autorepeat_count = 0;
  is_autorepeat    = false;

  erase_linenum = 0;
  total_lines   = 0;
  levelup_count = 0;

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
  mino.hard_drop();
  generate_mino(bag.increase(), true);
  can_hold = true;
}

void Player::soft_drop() {
  Coordinates vector(0, 1);

  if (!mino.collision(vector)) {
    mino.move(vector);
  } else {
    pre_mino_place = mino.global_coord();
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

    can_hold = false;
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

int Player::calc_score() {
  int _score = 0;

  int tmp_erase_lines = erase_linenum;
  erase_linenum       = field.erase_lines();

  if (tmp_erase_lines != 0 && erase_linenum != 0)
    ren_num++;

  int drop_score = (20 * drop_speed / 60 - gameManager->frame_count() / 60);
  int line_score = level * 100 * erase_linenum;

  _score = (drop_score + line_score) * (ren_num + 1);

  return _score;
}

void Player::level_control() {
  int tmp_level = level;
  total_lines += erase_linenum;
  level = total_lines / 10 + 1;
  if (level != tmp_level) {
    sound.play("levelup", DX_PLAYTYPE_BACK);
    if (drop_speed != 1) {
      drop_speed /= 1.5;
    }
  }
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
  } else if (total_lines >= max_lines) {
    return 1;
  }

  return 0;
}

int Player::update(bool key_pressed) {
  if (!key_pressed) {
    autorepeat_count = 0;
    is_autorepeat    = false;
  }

  if (mino.collision(bottom)) {
    if (lockdown_count >= 30) {
      lockdown_count = 0;
      generate_mino(bag.increase(), true);

      can_hold = true;
    }
    lockdown_count++;
  }

  make_ghost();
  calc_score();

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
    total_lines);
}

void Player::finalize() {
  InitFontToHandle();
  sound.finalize();
}
