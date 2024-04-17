#pragma once
#include <cstdlib>
#include <ncurses.h>

class Estrella {
public:
  Estrella();
  Estrella(int, int);
  ~Estrella();
  void getMaxXY();
  void draw();
  void update();
  void setVelosidad(int);
  float getVelosidad();

private:
  int ALTO, ANCHO;
  float x, y, velosidad = 1;
};

inline Estrella::Estrella() {}

inline Estrella::Estrella(int x, int y) {
  this->x = x;
  this->y = y;
}

inline Estrella::~Estrella() {}

inline void Estrella::getMaxXY() { getmaxyx(stdscr, ALTO, ANCHO); }

inline void Estrella::update() {
  getMaxXY();

  y += velosidad;

  if (y >= ALTO - 1) {
    x = rand() % (ANCHO - 3) + 1;
    y = 3;
  }
}

inline void Estrella::draw() {

  attron(COLOR_PAIR(1));
  mvaddch(y, x, '.');
  attroff(COLOR_PAIR(1));
}

inline void Estrella::setVelosidad(int velosidad) {
  this->velosidad = velosidad;
}

inline float Estrella::getVelosidad() { return velosidad; }
