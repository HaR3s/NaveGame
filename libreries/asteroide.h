#pragma once
#include "nave.h"
#include <cstdlib>
#include <iostream>
#include <ncurses.h>

class Asteroide {
public:
  Asteroide();
  Asteroide(int, int, float);
  ~Asteroide();
  void update();
  void draw();
  void colicion(Nave *&);
  void getMaxYX();
  void setY(int &);
  void setX(int);
  float getY();
  float getX();
  void setVelosidad(float);
  float getVelosidad();

private:
  float x, y, velosidad;
  int ALTO, ANCHO;
};

inline Asteroide::Asteroide() {}

inline Asteroide::Asteroide(int x, int y, float velosidad) {
  x = x;
  y = y;
  velosidad = velosidad;
}

inline Asteroide::~Asteroide() {}

inline void Asteroide::update() {

  getMaxYX();
  y += velosidad;

  if (y >= ALTO - 1) {

    x = rand() % (ANCHO - 3) + 2;
    y = 3;
  }
}

inline void Asteroide::draw() {

  attron(COLOR_PAIR(1));

  mvaddch(y, x, '@');

  attroff(COLOR_PAIR(1));
}

inline void Asteroide::colicion(Nave *&nave) {
  if (x >= nave->getX() + 2 && x <= nave->getX() + 2 && y <= nave->getY() + 3 &&
      y >= nave->getY() + 2) {
    nave->setEnergy(nave->getEnergy() - 1);

    x = rand() % ANCHO - 3;
    y = 3;
  }

  else if (x >= nave->getX() && x <= nave->getX() + 3 &&
           y <= nave->getY() + 2 && y >= nave->getY() - 2) {
    nave->setEnergy(nave->getEnergy() - 1);

    x = rand() % ANCHO - 3;
    y = 3;
  }

  else if (x >= nave->getX() && x <= nave->getX() + 5 &&
           y <= nave->getY() + 3 && y >= nave->getY() + 3) {
    nave->setEnergy(nave->getEnergy() - 1);

    x = rand() % ANCHO - 3;
    y = 3;
  }
}

inline void Asteroide::getMaxYX() { getmaxyx(stdscr, ALTO, ANCHO); }

inline void Asteroide::setY(int &y) { this->y = y; }

inline void Asteroide::setX(int x) { this->x = x; }

inline float Asteroide::getX() { return x; }

inline float Asteroide::getY() { return y; }

inline void Asteroide::setVelosidad(float velosidad) {
  this->velosidad = velosidad;
}

inline float Asteroide::getVelosidad() { return velosidad; }
