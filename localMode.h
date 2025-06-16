#ifndef LOCALMODE_H
#define LOCALMODE_H

#include <Adafruit_MPR121.h>
#include <Adafruit_NeoPixel.h>

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

struct MechKey {
  uint8_t touchID;       // Index sur le MPR121
  uint8_t dataPin;       // Broche de l'ESP32 vers NeoPixel
  bool state;            // État (toggle)
  Adafruit_NeoPixel led; // Instance NeoPixel
};

struct Coord {
  int x;
  int y;
};

uint8_t color_pos = 16;

// Création tableau global, mais pas de capteurs ici !
MechKey keys[] = {
  {0, A0, false, Adafruit_NeoPixel(1, A0, NEO_GRB + NEO_KHZ800)},
  {1, 25, false, Adafruit_NeoPixel(1, 25, NEO_GRB + NEO_KHZ800)},
  {2, 34, false, Adafruit_NeoPixel(1, 34, NEO_GRB + NEO_KHZ800)},
  {3, 39, false, Adafruit_NeoPixel(1, 39, NEO_GRB + NEO_KHZ800)},
  {4, 36, false, Adafruit_NeoPixel(1, 36, NEO_GRB + NEO_KHZ800)},
  {5, 4, false, Adafruit_NeoPixel(1, 4, NEO_GRB + NEO_KHZ800)},
  {6, 14, false, Adafruit_NeoPixel(1, 14, NEO_GRB + NEO_KHZ800)},
  {7, 33, false, Adafruit_NeoPixel(1, 33, NEO_GRB + NEO_KHZ800)},
  {8, 15, false, Adafruit_NeoPixel(1, 15, NEO_GRB + NEO_KHZ800)},
};
const int numKeys = 9;

int grille[3][3] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};

bool btns[9] = { false, false, false, false, false, false, false, false, false };

bool joueur1 = true;

Coord getCo(int key);
bool victoire(int grille[3][3], int joueur);
bool allInit();

void localMode(Adafruit_MPR121& cap, uint16_t& lasttouched, uint16_t& currtouched);

#endif // LOCALMODE_H


Coord getCo(int key) {
  switch(key) {
    case 0: return { 0, 0 };
    case 1: return { 1, 0 };
    case 2: return { 2, 0 };
    case 3: return { 0, 1 };
    case 4: return { 1, 1 };
    case 5: return { 2, 1 };
    case 6: return { 0, 2 };
    case 7: return { 1, 2 };
    case 8: return { 2, 2 };
    default: return { -1, -1 };
  }
}

bool victoire(int grille[3][3], int joueur) {
    // Vérification des lignes et colonnes
    for (int i = 0; i < 3; ++i) {
        // Ligne
        if (grille[i][0] == joueur && grille[i][1] == joueur && grille[i][2] == joueur)
            return true;
        // Colonne
        if (grille[0][i] == joueur && grille[1][i] == joueur && grille[2][i] == joueur)
            return true;
    }
    // Vérification des deux diagonales
    if (grille[0][0] == joueur && grille[1][1] == joueur && grille[2][2] == joueur)
        return true;
    if (grille[0][2] == joueur && grille[1][1] == joueur && grille[2][0] == joueur)
        return true;

    // Aucun alignement trouvé
    return false;
}

bool allInit() {
  for(int i = 0; i < 9; i++) {
    if (!btns[i]) return false;
  } 
  return true;
}

void localMode(Adafruit_MPR121& cap, uint16_t& lasttouched, uint16_t& currtouched) {
  currtouched = cap.touched();

  for (int i = 0; i < numKeys; i++) {
    uint8_t t = keys[i].touchID;
    keys[i].led.setPixelColor(0, 255); // Bleu
    keys[i].led.show();

    if ((currtouched & _BV(t)) && !(lasttouched & _BV(t))) {
      Serial.print("Touch "); Serial.print(t); Serial.println(" pressed");
      btns[i] = true;

      if(allInit()) {
        Coord c = getCo(i);
        if(grille[c.y][c.x] == 0) {
          if(joueur1) {
            grille[c.y][c.x] = 1;
            if(victoire(grille, 1)) {
              Serial.println("Joueur 1 a gagne");
            }
          } else {
            grille[c.y][c.x] = 2;
            if(victoire(grille, 2)) {
              Serial.println("Joueur 2 a gagne");
            }
          }
          joueur1 = !joueur1;
        }

         for(int i = 0; i < 3; i++) {
          for(int j = 0; j < 3; j++) {
            Serial.print(grille[i][j]);
          }
          Serial.println();
        }
      }
    }
  }
  lasttouched = currtouched;
}