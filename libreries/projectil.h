#pragma once
#include <iostream>
#include <ncurses.h>

class Projectil {
public:
  Projectil();
  Projectil(int, int);
  void update();
  void draw();
  bool limite();
  int getY();
  int getX();
  void setY(int);
  void setX(int);
  ~Projectil();

private:
  int x;
  float y;
};

inline Projectil::Projectil() {}

inline Projectil::Projectil(int x, int y) {
  this->x = x;
  this->y = y;
}

inline Projectil::~Projectil() {}

inline void Projectil::update() {
  if (y >= 3) {
    y--;
  }
}

inline void Projectil::draw() {

  attron(COLOR_PAIR(3));

  mvaddch(y, x, '^');

  attroff(COLOR_PAIR(3));
}

inline int Projectil::getX() { return x; }

inline int Projectil::getY() { return y; }

inline void Projectil::setX(int x) { this->x = x; }

inline void Projectil::setY(int y) { this->y = y; }

inline bool Projectil::limite() { return (y <= 2) ? true : false; }
