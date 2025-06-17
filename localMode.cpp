#include "localMode.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

extern Adafruit_SH1107 display;

MechKey keys[] = {
  {0, A0, false, Adafruit_NeoPixel(1, D4, NEO_GRB + NEO_KHZ800)}, //D4
  {1, 25, false, Adafruit_NeoPixel(1, D5, NEO_GRB + NEO_KHZ800)},
  {2, 34, false, Adafruit_NeoPixel(1, A0, NEO_GRB + NEO_KHZ800)},
  {3, 39, false, Adafruit_NeoPixel(1, A1, NEO_GRB + NEO_KHZ800)}, 
  {4, 36, false, Adafruit_NeoPixel(1, 16, NEO_GRB + NEO_KHZ800)}, 
  {5, 4, false, Adafruit_NeoPixel(1, 17, NEO_GRB + NEO_KHZ800)}, 
  {6, 14, false, Adafruit_NeoPixel(1, D2, NEO_GRB + NEO_KHZ800)}, 
  {7, 33, false, Adafruit_NeoPixel(1, D3, NEO_GRB + NEO_KHZ800)}, //D3
  {8, 15, false, Adafruit_NeoPixel(1, 4, NEO_GRB + NEO_KHZ800)}, 
};
const int numKeys = 9;

int grille[3][3] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};

bool btns[9] = { false, false, false, false, false, false, false, false, false };

bool joueur1 = true;
bool isBtnSet = false;
unsigned long lastDebounceReset = 0;
const unsigned long debounceDelayReset = 200;
uint8_t btnPrev = LOW;
uint8_t btn;
bool isFirstInit = true;


bool isWin = false;

int c1[] = {-1,-1};
int c2[] = {-1,-1};
int c3[] = {-1,-1};

void setBtn(Adafruit_MPR121& cap, uint16_t& lasttouched, uint16_t& currtouched){
  if(isFirstInit){
    turnOff();
    isFirstInit = false;
  }
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Rester appuye sur\nchaqu'un des boutons\npendant 3 secondes\npour les initialiser");
  display.display();

  if(allInit()){
    isBtnSet = true;
    for (int i = 0; i < numKeys; i++) {
      keys[i].led.setPixelColor(0, 0,0,0);
      keys[i].led.show();
    }
  }else{
    currtouched = cap.touched();
    for (int i = 0; i < numKeys; i++) {
      uint8_t t = keys[i].touchID;
      if ((currtouched & _BV(t)) && !(lasttouched & _BV(t))) {
        Serial.print("Touch "); Serial.print(t); Serial.println(" pressed");
        btns[i] = true;
        keys[i].led.setPixelColor(0, 255,255,255);
        keys[i].led.show();
      }
    }
    lasttouched = currtouched;
  }
}

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
      if (grille[i][0] == joueur && grille[i][1] == joueur && grille[i][2] == joueur){
        c1[0]=i;
        c1[1]=0;
        c2[0]=i;
        c2[1]=1;
        c3[0]=i;
        c3[1]=2;
        return true;
      }
      if (grille[0][i] == joueur && grille[1][i] == joueur && grille[2][i] == joueur){
        c1[0]=0;
        c1[1]=i;
        c2[0]=1;
        c2[1]=i;
        c3[0]=2;
        c3[1]=i;
        return true;
      }
    }
    // Vérification des deux diagonales
    if (grille[0][0] == joueur && grille[1][1] == joueur && grille[2][2] == joueur){
      c1[0]=0;
      c1[1]=0;
      c2[0]=1;
      c2[1]=1;
      c3[0]=2;
      c3[1]=2;
      return true;
    }
    if (grille[0][2] == joueur && grille[1][1] == joueur && grille[2][0] == joueur){
      c1[0]=0;
      c1[1]=2;
      c2[0]=1;
      c2[1]=1;
      c3[0]=2;
      c3[1]=0;
      return true;
    }
    return false;
}

bool allInit() {
  for(int i = 0; i < 9; i++) {
    if (!btns[i]) return false;
  } 
  return true;
}

void reset() {
  Serial.println("Reset");

  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      grille[i][j] = 0;
    }
  }
  isWin = false;
  for (int i = 0; i < numKeys; i++) {
    keys[i].led.setPixelColor(0, 0,0,0);
    keys[i].led.show();
  }
  joueur1 = true;
}

void readButtonReset() {
    btn = digitalRead(A2);
    
    if (btn == LOW && btnPrev == HIGH && (millis() - lastDebounceReset > debounceDelayReset)) {
      lastDebounceReset = millis();
      reset();
    }
    btnPrev = btn;
}

void localModeDuo(Adafruit_MPR121& cap, uint16_t& lasttouched, uint16_t& currtouched) {
  currtouched = cap.touched();
  readButtonReset();
  
  if(!isWin){
    menuGame();
    for (int i = 0; i < numKeys; i++) {
      uint8_t t = keys[i].touchID;
      
      if ((currtouched & _BV(t)) && !(lasttouched & _BV(t))) {
        Serial.print("Touch "); Serial.print(t); Serial.println(" pressed");

        Coord c = getCo(i);
        if(grille[c.y][c.x] == 0) {
          if(joueur1) {
            grille[c.y][c.x] = 1;
            keys[i].led.setPixelColor(0, 0,0,255);
            keys[i].led.show();
            if(victoire(grille, 1)) {
              Serial.println("Joueur 1 a gagne");
              affichResultat();
            }
          } else {
            grille[c.y][c.x] = 2;
            keys[i].led.setPixelColor(0, 255,0,0);
            keys[i].led.show();
            if(victoire(grille, 2)) {
              Serial.println("Joueur 2 a gagne");
              affichResultat();
            }
          }
          joueur1 = !joueur1;
        }
      }
    }
  }else{
    menuWin();
  }
  lasttouched = currtouched;
}

void localModeSolo(Adafruit_MPR121& cap, uint16_t& lasttouched, uint16_t& currtouched) {
  currtouched = cap.touched();
  readButtonReset();
  if(!isWin){
    menuGame();
    for (int i = 0; i < numKeys; i++) {
      uint8_t t = keys[i].touchID;

      if ((currtouched & _BV(t)) && !(lasttouched & _BV(t))) {
        Serial.print("Touch "); Serial.print(t); Serial.println(" pressed");

        Coord c = getCo(i);
        if(grille[c.y][c.x] == 0) {
          grille[c.y][c.x] = 1;
          keys[i].led.setPixelColor(0, 0,0,255);
          keys[i].led.show();
          if(victoire(grille, 1)) {
            Serial.println("Joueur 1 a gagne");
            affichResultat();
            break;
          }
          
          int x, y;
          do {
            x = random(0, 3);
            y = random(0, 3);
          } while(grille[y][x] != 0);

          grille[y][x] = 2;
          keys[(3*y)+x].led.setPixelColor(0, 255,0,0);
          keys[(3*y)+x].led.show();
          if(victoire(grille, 2)) {
            Serial.println("Joueur 2 a gagne");
            affichResultat();
          }
        }
      }
    }
  }else{
    menuWin();
  }
  lasttouched = currtouched;
}


void affichResultat(){
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      if((i!=c1[0] || j!=c1[1]) && (i!=c2[0] || j!=c2[1]) && (i!=c3[0] || j!=c3[1])){
        keys[(3*i)+j].led.setPixelColor(0, 0,0,0);
        keys[(3*i)+j].led.show();
      }
    }
  }
  isWin = true;
}


void turnOff(){
  for (int i = 0; i < numKeys; i++) {
      keys[i].led.setPixelColor(0, 0,0,0);
      keys[i].led.show();
    }
}

void menuGame(){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Cliquer sur un boutonpour commencer");
  display.display();
}

void menuWin(){
  display.clearDisplay();
  display.setCursor(0, 0);
   if(victoire(grille, 1)){
      display.println("Victoire du joueur 1");
      display.println();
      display.println("Cliquer sur le boutonreplay pour rejouer !");
      display.println("Cliquer sur le boutonreset pour revenir\nau menu !");
   }else{
      display.println("Victoire du joueur 2");
      display.println();
      display.println("Cliquer sur le boutonreplay pour rejouer !");
      display.println();
      display.println("Cliquer sur le boutonreset pour revenir\nau menu !");
   }
  display.display();
}
