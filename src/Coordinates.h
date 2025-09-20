#pragma once

class Coordinates {
  public:
    int x;
    int y;
    Coordinates(int x, int y);
    Coordinates add(Coordinates c);
    Coordinates sub(Coordinates c);
    Coordinates mult(float value);
    Coordinates div(float value);
};