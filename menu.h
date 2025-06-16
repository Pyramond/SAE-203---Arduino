#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>


bool local = true;
bool solo = true;
int difficulty = 1;

uint8_t btnPrevA;
uint8_t btnPrevB;
uint8_t btnA;
uint8_t btnB;

bool isMod = false;
bool isModJeu = false;
bool isDifficulty = false;
bool isPlay = false;
bool isWebSet = false;

#define BUTTON_A 15
#define BUTTON_B 32
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

unsigned long lastDebounceTimeA = 0;
unsigned long lastDebounceTimeB = 0;
const unsigned long debounceDelay = 200; // délai de rebond (200 ms)

void setupMenu() {
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);

  btnPrevA = digitalRead(BUTTON_A);
  btnPrevB = digitalRead(BUTTON_B);

  display.begin(0x3C, true);
  display.clearDisplay();
  display.drawBitmap(0, 0, epd_bitmap_logo, 128, 64, SH110X_WHITE);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();

  display.setRotation(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.display();
}












void choixMode() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Choisissez le mode :\n");
  display.print("[A] Local\n");
  display.print("[B] Web\n");
  display.display();
}

void choixModeJeu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Choisissez le type :\n");
  display.print("[A] Solo\n");
  display.print("[B] Duo\n");
  display.display();
}

void choixDifficulty() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Choisissez la \ndifficulte :\n");
  display.print("[A] Debutant\n");
  display.print("[B] Expert\n");
  display.display();
}


void menu() {
  if (!isMod) {
    choixMode();
  } else if (!isModJeu) {
    choixModeJeu();
  } else if (solo && !isDifficulty) {
    choixDifficulty();
  } else {
    isPlay = true; // mode duo sans choix difficulté
  }
}









void handleButtonA() {
  if (!isMod) {
    local = true;
    isMod = true;
  } else if (!isModJeu) {
    solo = true;
    isModJeu = true;
  } else if (solo && !isDifficulty) {
    difficulty = 1;
    isDifficulty = true;
    isPlay = true;
  }
}

void handleButtonB() {
  if (!isMod) {
    local = false;
    isMod = true;
  } else if (!isModJeu) {
    solo = false;
    isModJeu = true;
  } else if (solo && !isDifficulty) {
    difficulty = 2;
    isDifficulty = true;
    isPlay = true;
  }
}


void readButton(){
  btnA = digitalRead(BUTTON_A);
  if (btnA == LOW && btnPrevA == HIGH && (millis() - lastDebounceTimeA > debounceDelay)) {
    lastDebounceTimeA = millis();
    handleButtonA();
  }
  btnPrevA = btnA;
  btnB = digitalRead(BUTTON_B);
  if (btnB == LOW && btnPrevB == HIGH && (millis() - lastDebounceTimeB > debounceDelay)) {
    lastDebounceTimeB = millis();
    handleButtonB();
  }
  btnPrevB = btnB;
}