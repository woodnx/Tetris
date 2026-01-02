#include "Coordinates.h"

Coordinates::Coordinates(int x, int y) {
  this->x = x;
  this->y = y;
}

Coordinates Coordinates::add(Coordinates c) {
  int x = this->x + c.x;
  int y = this->y + c.y;

  return Coordinates(x, y);
}

Coordinates Coordinates::sub(Coordinates c) {
  int x = this->x - c.x;
  int y = this->y - c.y;

  return Coordinates(x, y);
}

Coordinates Coordinates::mult(float value) {
  int x = (int) (this->x * value);
  int y = (int) (this->y * value);

  return Coordinates(x, y);
}

Coordinates Coordinates::div(float value) {
  int x = (int) (this->x / value);
  int y = (int) (this->y / value);

  return Coordinates(x, y);
}
