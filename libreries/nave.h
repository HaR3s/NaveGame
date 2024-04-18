#pragma once
#include <iostream>
#include <ncurses.h>

class Nave {
public:
  Nave(int *, int *);
  Nave();
  ~Nave();
  void setup();
  void draw();
  void setX(int);
  void setY(int);
  int getX();
  int getY();
  void setEnergy(int);
  int getEnergy();
  void setVidas(int);
  int getVidas();
  void update(int *, int *);
  //  void getMaxYX(WINDOW*);
  //void set(int *, int *);

private:
  int m_x, m_y;
  int energy;
  int vidas;
  int *ALTO;
  int *ANCHO;
  bool explotar;
};

inline Nave::Nave() : ALTO(new int), ANCHO(new int){}

inline Nave::Nave(int *alto, int *ancho) : ALTO(new int), ANCHO(new int){
  *ALTO = *alto;
  *ANCHO = *ancho;
}

inline Nave::~Nave() {
  delete ALTO;
  delete ANCHO;
}

inline void Nave::setup() {
  // getMaxYX();

  m_x = *ANCHO / 2 - 3;
  m_y = *ALTO * 0.80 - 4;
  energy = 3;
  vidas = 3;
  explotar = false;
}

inline void Nave::draw() {
  if (!explotar) {
    attron(COLOR_PAIR(3));
    mvaddch(m_y, m_x + 2, '^');
    mvaddch(m_y + 1, m_x + 1, '(');
    mvaddch(m_y + 1, m_x + 2, ACS_PLMINUS);
    mvaddch(m_y + 1, m_x + 3, ')');
    mvaddch(m_y + 2, m_x, '/');
    mvaddch(m_y + 2, m_x + 1, '#');
    mvaddch(m_y + 2, m_x + 2, '^');
    mvaddch(m_y + 2, m_x + 3, '#');
    mvaddch(m_y + 2, m_x + 4, '\\');
    attroff(COLOR_PAIR(3));
  } else {
    attron(COLOR_PAIR(2));
    mvprintw(m_y, m_x - 2, "   ***   ");
    mvprintw(m_y + 1, m_x - 2, "  *****  ");
    mvprintw(m_y + 2, m_x - 2, "   ***  ");

    refresh();
    delay_output(400);

    mvprintw(m_y - 1, m_x - 3, "     +    ");
    mvprintw(m_y, m_x - 2, "  +    + ");
    mvprintw(m_y + 1, m_x - 3, "+    +    +");
    mvprintw(m_y + 2, m_x - 2, " +     +  ");
    mvprintw(m_y + 3, m_x - 3, "     +    ");
    attroff(COLOR_PAIR(2));

    refresh();
    delay_output(500);

    explotar = false;
    draw();
  }
}

inline void Nave::setX(int x) { m_x = x; }

inline void Nave::setY(int y) { m_y = y; }

inline int Nave::getY() { return m_y; }

inline int Nave::getX() { return m_x; }

inline void Nave::setEnergy(int energy) { this->energy = energy; }

inline int Nave::getEnergy() { return energy; }

inline void Nave::setVidas(int vidas) { this->vidas = vidas; }

inline int Nave::getVidas() { return vidas; }

inline void Nave::update(int *alto, int *ancho) {
  // getMaxYX(wind);

  *ALTO = *alto;
  *ANCHO = *ancho;

  if (energy == 0) {

    explotar = true;
    vidas--;
    energy = 3;
  }
}

// inline void Nave::getMaxYX(WINDOW* wind) { getmaxyx(wind, ALTO, ANCHO); }
/*
inline void Nave::set(int *alto, int *ancho) {
  
  *ALTO = *alto;
  *ANCHO = *ancho;
} */
