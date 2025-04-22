#pragma once
#include "DxLib.h"
#include "Field.h"
#include "StaticMino.h"
#include "include.h"

#define LAYOUT_SIZE 4

class Mino : public StaticMino {
  public:
    Mino(Field *_field);

    void initialize() override;

    int getMinoNum();
    int getMinoCoordX();
    int getMinoCoordY();

    void generateMinoWithPos(int generate_mino_num, int field_x, int field_y);

    void rotateMinoWithCollision(bool right_flag);
    bool superRotation(int dir_old, bool right_flag);

    void moveMino(int dx, int dy);
    void moveMinoWithCollision(int dx, int dy);
    void dropToMaxBottom();

    bool collisionField();
    bool collisionField(int dx, int dy);

    void transcribeMinoToField();
    void drawStatus();

  private:
    Field *field;
    int field_x, field_y;
};
