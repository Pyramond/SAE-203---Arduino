#include "pin.h"
#include "logo.h"
#include "menu.h"
#include "web.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
#define BUTTON_A 15
#define BUTTON_B 32


bool isMod = false;
bool isModJeu = false;
bool isDifficulty = false;
bool isPlay = false;
bool isWebSet = false;



void setup() {
  Serial.begin(115200);
  setupMenu();
}

void loop() {
  readButton();
  if (!isPlay) {
    menu();
  } else {
    if (!local) {
      if(!isWebSet){
        setupWeb();
        isWebSet = true;
      }else{
        loopWeb();
      }
    } else {
      // Mode local
    }
  }
}