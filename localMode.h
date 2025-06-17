#ifndef LOCALMODE_H
#define LOCALMODE_H

#include <Adafruit_MPR121.h>
#include <Adafruit_NeoPixel.h>
#include "pin.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

struct MechKey {
  uint8_t touchID;      
  uint8_t dataPin;      
  bool state;           
  Adafruit_NeoPixel led;
};

struct Coord {
  int x;
  int y;
};

extern uint8_t color_pos;
extern MechKey keys[];
extern const int numKeys;
extern int grille[3][3];
extern bool btns[9];
extern bool joueur1;

Coord getCo(int key);
bool victoire(int grille[3][3], int joueur);
bool allInit();
void afficheGrille();
void reset();

void localModeDuo(Adafruit_MPR121& cap, uint16_t& lasttouched, uint16_t& currtouched);
void localModeSolo(Adafruit_MPR121& cap, uint16_t& lasttouched, uint16_t& currtouched);

#endif // LOCALMODE_H