#ifndef MENU_H
#define MENU_H

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// Variables de configuration du menu
extern bool local;
extern bool solo;
extern int difficulty;

extern uint8_t btnPrevA;
extern uint8_t btnPrevB;
extern uint8_t btnA;
extern uint8_t btnB;

extern bool isMod;
extern bool isModJeu;
extern bool isDifficulty;
extern bool isPlay;
extern bool isWebSet;

#define BUTTON_A 15
#define BUTTON_B 32

extern Adafruit_SH1107 display;

extern unsigned long lastDebounceTimeA;
extern unsigned long lastDebounceTimeB;
extern const unsigned long debounceDelay;

void setupMenu();
void choixMode();
void choixModeJeu();
void choixDifficulty();
void menu();

void handleButtonA();
void handleButtonB();
void readButton();

#endif // MENU_H
