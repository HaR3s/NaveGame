#include "libreries/asteroide.h"
#include "libreries/estrella.h"
#include "libreries/nave.h"
#include "libreries/projectil.h"
#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include <ostream>
#include <vector>

using namespace std;

int ANCHO = 131;
int ALTO = 58;
const int DELAY = 100;

bool game_over;
int puntaje;
int auxPuntaje;
bool salir;

Nave nave;
Asteroide asteroide;
Estrella estrella;

vector<Estrella> estrellas;
vector<Asteroide> asteroides;
vector<Projectil> projectiles;

// Prototipos de funciones

void setup();
void imput();
void update();
void draw();
void gameOver();
void getMaxXY();
void dibujarVidEner();
void WgameOver();
void VentanaInicio();
void pause();

int main() {

  initscr();
  start_color();

  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);

  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  raw();
  setup();
  draw();
  dibujarVidEner();
  VentanaInicio();

  salir = false;

  while (!salir) {
    while (!game_over) {

      imput();
      draw();
      update();
    }

    gameOver();
  }

  endwin();

  cout << endl;
  return 0;
}

// Inisializacion

void setup() {
  getMaxXY();
  game_over = false;
  puntaje = 0;
  auxPuntaje = 0;

  nave.setup();

  asteroides.clear();
  projectiles.clear();
  estrellas.clear();

  for (int i = 0; i < 5; i++) {
    asteroides.push_back(
        Asteroide(rand() % ANCHO + 2, rand() % ALTO + 3, 0.2f));
  }

  for (int i = 0; i < (ANCHO + 3) / 2; i++) {

    estrellas.push_back(Estrella(rand() % ANCHO + 2, rand() % ALTO + 3));
  }
}

// Deteccion de teclas

void imput() {
  int tecla = getch();

  switch (tecla) {
  case 27:
    pause();
    break;

  case KEY_UP:
    if (nave.getY() > 3) {
      nave.setY(nave.getY() - 1);
    }
    break;
  case KEY_DOWN:
    if (nave.getY() < ALTO - 4) {
      nave.setY(nave.getY() + 1);
    }
    break;
  case KEY_LEFT:
    if (nave.getX() > 2) {
      nave.setX(nave.getX() - 1);
    }
    break;
  case KEY_RIGHT:
    if (nave.getX() < ANCHO - 7) {
      nave.setX(nave.getX() + 1);
    }
    break;
  case ' ':
    projectiles.push_back(Projectil(nave.getX() + 2, nave.getY()));
    break;
  case 'e':
    nave.setEnergy(nave.getEnergy() - 1);
    break;
  default:

    break;
  }
}

// Actalizacion  de los componentes

void update() {
  getMaxXY();

  // aumento de velosidad

  if (auxPuntaje >= 100) {

    for (int i = 0; i < asteroides.size(); i++) {
      asteroides[i].setVelosidad(asteroides[i].getVelosidad() + 0.1f);
      asteroides[i].update();
      asteroides[i].colicion(nave);
      auxPuntaje = 0;
    }
  } else {
    for (int i = 0; i < asteroides.size(); i++) {
      asteroides[i].update();
      asteroides[i].colicion(nave);
      asteroides[i].setVelosidad(asteroides[i].getVelosidad());
    }
  }

  nave.update();

  for (int i = 0; i < estrellas.size(); i++) {
    estrellas[i].update();
  }

  for (int i = 0; i < projectiles.size(); i++) {
    projectiles[i].update();
    if (projectiles[i].limite()) {
      projectiles.erase(projectiles.begin() + i);
    }
  }

  for (int i = 0; i < asteroides.size(); i++) {
    for (int j = 0; j < projectiles.size(); j++) {

      if (asteroides[i].getX() == projectiles[j].getX() &&
          (asteroides[i].getY() == projectiles[j].getY() ||
           asteroides[i].getY() > projectiles[j].getY())) {

        projectiles.erase(projectiles.begin());
        asteroides.erase(asteroides.begin() + i);
        asteroides.push_back(
            Asteroide(rand() % ANCHO + 2, 3, asteroides[i].getVelosidad()));

        puntaje += 10;
        auxPuntaje += 10;
      }
    }
  }

  if (nave.getVidas() <= 0) {
    game_over = true;
  }
}

// Dibuja en consola

void draw() {
  erase();

  // Dibujar el marco

  attron(COLOR_PAIR(3));

  newwin(ALTO, ANCHO, 0, 0);
  box(stdscr, 0, 0);

  // Dibja la linea inferior a las Vidas y Energia
  for (int i = 0; i < ANCHO; i++) {
    mvhline(2, 1, ACS_HLINE, ANCHO - 2);
  }

  attroff(COLOR_PAIR(3));

  // Dibujar la barra de vida y energia

  dibujarVidEner();

  // Dibuja estrellas

  for (int i = 0; i < estrellas.size(); i++) {
    estrellas[i].draw();
  }

  // Dibujar nave

  nave.draw();

  // Dibujar Asteroide

  for (int i = 0; i < asteroides.size(); i++) {
    asteroides[i].draw();
  }

  // Dibujar projectiles

  for (int i = 0; i < projectiles.size(); i++) {
    projectiles[i].draw();
  }

  refresh();
  delay_output(DELAY);
}

// Ventana de Game Over

void gameOver() {

  // Dibujar marco de Game Over

  WgameOver();

  int opcion = getch();

  if ((opcion == 'y') || (opcion == 'Y')) {
    game_over = false;
    setup();

  } else if ((opcion == 'n') || (opcion == 'N')) {
    salir = TRUE;
  }
}

void getMaxXY() { getmaxyx(stdscr, ALTO, ANCHO); }

void dibujarVidEner() {

  attron(COLOR_PAIR(3));

  // Dibujar la energia

  int energiax = ANCHO * 0.85;

  mvprintw(1, energiax - 8, "[ Energia       ]");

  for (int i = 0; i < nave.getEnergy(); i++) {
    mvaddch(1, (energiax - 8) + 10 + i * 2, ACS_CKBOARD);
  }

  // Dibujar la vida

  int vidasx = ANCHO * 0.10;

  mvprintw(1, vidasx - 4, "[ Vidas       ]");

  for (int i = 0; i < nave.getVidas(); i++) {
    mvaddch(1, (vidasx - 4) + 8 + i * 2, 64);
  }

  // Dibujar puntaje

  mvprintw(1, ANCHO / 2 - 8, "[ Puntos:       ]");

  mvprintw(1, ANCHO / 2 + 2, "%d", puntaje);

  attroff(COLOR_PAIR(3));
}

void WgameOver() {
  int centro_x = ANCHO / 2 - 20;     // Calcular el centro en el eje x
  int centro_y = (ALTO + 4) / 2 - 4; // Calcular el centro en el eje y

  attron(COLOR_PAIR(3));

  // Dibjar marco de game_over
  for (int i = centro_y; i < centro_y + 6; i++)
    mvhline(i, centro_x, ' ', 40);

  // Marcos esquinas
  mvaddch(centro_y - 1, centro_x + 4, ACS_ULCORNER);
  mvaddch(centro_y - 1, centro_x + 37, ACS_URCORNER);
  mvaddch(centro_y + 6, centro_x + 4, ACS_LLCORNER);
  mvaddch(centro_y + 6, centro_x + 37, ACS_LRCORNER);

  // Lineas Horizontales
  mvhline(centro_y - 1, centro_x + 5, ACS_HLINE, 32);
  mvhline(centro_y + 6, centro_x + 5, ACS_HLINE, 32);

  // Lineas verticales
  mvvline(centro_y, centro_x + 4, ACS_VLINE, 6);
  mvvline(centro_y, centro_x + 37, ACS_VLINE, 6);

  // Dibujar las opciones
  mvprintw(centro_y + 2, centro_x + 16, "Game Over");
  mvprintw(centro_y + 3, centro_x + 11, "Volver a Jugar (Y/N)");

  attroff(COLOR_PAIR(3));
}

void VentanaInicio() {

  char opt;

  int centro_x = ANCHO / 2 - 20;     // Calcular el centro en el eje x
  int centro_y = (ALTO + 4) / 2 - 4; // Calcular el centro en el eje y

  attron(COLOR_PAIR(3));

  // Dibjar marco de game_over
  for (int i = centro_y; i < centro_y + 6; i++)
    mvhline(i, centro_x, ' ', 40);

  // Marcos esquinas
  mvaddch(centro_y - 1, centro_x + 4, ACS_ULCORNER);
  mvaddch(centro_y - 1, centro_x + 37, ACS_URCORNER);
  mvaddch(centro_y + 6, centro_x + 4, ACS_LLCORNER);
  mvaddch(centro_y + 6, centro_x + 37, ACS_LRCORNER);

  // Lineas Horizontales
  mvhline(centro_y - 1, centro_x + 5, ACS_HLINE, 32);
  mvhline(centro_y + 6, centro_x + 5, ACS_HLINE, 32);

  // Lineas verticales
  mvvline(centro_y, centro_x + 4, ACS_VLINE, 6);
  mvvline(centro_y, centro_x + 37, ACS_VLINE, 6);

  // Dibujar  opcion
  mvprintw(centro_y + 2, centro_x + 16, "Bienvenido");
  mvprintw(centro_y + 3, centro_x + 13, "Presine Espasio");

  attroff(COLOR_PAIR(3));

  opt = getch();

  if (opt == ' ') {
    setup();

  } else {
    VentanaInicio();
  }
}

void pause() {
  int centro_x = ANCHO / 2 - 20;     // Calcular el centro en el eje x
  int centro_y = (ALTO + 4) / 2 - 4; // Calcular el centro en el eje y

  attron(COLOR_PAIR(3));

  // Dibujar marco de game_over
  for (int i = centro_y; i < centro_y + 6; i++)
    mvhline(i, centro_x, ' ', 40);

  // Marcos esquinas
  mvaddch(centro_y - 1, centro_x + 4, ACS_ULCORNER);
  mvaddch(centro_y - 1, centro_x + 37, ACS_URCORNER);
  mvaddch(centro_y + 6, centro_x + 4, ACS_LLCORNER);
  mvaddch(centro_y + 6, centro_x + 37, ACS_LRCORNER);

  // Lineas Horizontales
  mvhline(centro_y - 1, centro_x + 5, ACS_HLINE, 32);
  mvhline(centro_y + 6, centro_x + 5, ACS_HLINE, 32);

  // Lineas verticales
  mvvline(centro_y, centro_x + 4, ACS_VLINE, 6);
  mvvline(centro_y, centro_x + 37, ACS_VLINE, 6);

  // Dibujar las opciones
  mvprintw(centro_y + 2, centro_x + 16, "  Pausa ");
  mvprintw(centro_y + 3, centro_x + 11, "Segir jugando (Y/N)");

  attroff(COLOR_PAIR(3));

  int opcion;

  while (true) {
    opcion = getch();
    if ((opcion == 'y') || (opcion == 'Y')) {
      game_over = FALSE;
      break;
    } else if ((opcion == 'n') || (opcion == 'N')) {
      system("clear");
      exit(1);
      break;
    }
  }
}
