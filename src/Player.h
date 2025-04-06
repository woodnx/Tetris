#pragma once
#include "include.h"
#include "Mino.h"
#include "StaticMino.h"
#include "MinoRow.h"
#include "Field.h"
#include "Sound.h"

typedef enum {
	ePause_Continue,	//続ける
	ePause_Restart,		//やりなおす
	ePause_End,			//スタートに戻る
	ePause_Num,			//本項目の数
} ePause;

typedef enum {
	eResult_Restart,	//やりなおす
	eResult_End,		//スタートに戻る
	eResult_Num,			//本項目の数
} eResult;

class Player
{
private:
	Field* field;
	Mino* mino;
	Mino* ghost_mino;
	StaticMino* hold_mino;
	StaticMino* next_mino[NEXT_REFER_SIZE];

	MinoRow row;

  bool can_control;

	// 画像・フォントハンドル
	int background_handle;
	int index_font;
	int figure_font;

	// 音声管理
	Sound sound;

	// 座標
	int x, y;
	int gnrt_mx, gnrt_my;
	int pre_mino_coordx;
	int pre_mino_coordy;

	// スコア
	int level;
	int score;
	int ren_num;
	int drop_speed;
	const int max_linenum = 100;

	// オートリピート
	int t;
	int lockdown_count;
	int autorepeat_count;
	bool isautorepeat;

	// mino生成フラグ
	int new_gnrt_mino;
	bool isbottom;
	bool can_generate;
	bool can_transcribe;
	bool can_incrrow;

	// ライン削除
	int erase_linenum;
	int levelup_count;
	int sum_linenum;
  int shift_count;

	// ホールド
	int hold_mino_num;
	bool hold_enable;

	// private関数
	void controlMino();
  void moveMino(bool is_right);
	void dropMino();
	void holdMino();
	void makeGhost();
	void setNext();
	void levelControl();
	void installMino();
  int eraseAndShitLine();
public:
	Player();
	Player(int x, int y);

	void initialize();
	void update();
	void draw();
	void finalize();

	void startProcess();
	int judgeGameResult();
	int calcScore(int _level, int _drop_speed);
};