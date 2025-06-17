#include "pin.h"
#include "logo.h"
#include "menu.h"
#include "web.h"
#include "localMode.h"

#include <SPI.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <Adafruit_NeoPixel.h>

// --- Capteur capacitif ---
// Attention : Une SEULE instance globale !
Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {
  Serial.begin(115200);


  if (!cap.begin(0x5B)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");


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
      if(!isBtnSet){
        setBtn(cap, lasttouched, currtouched);
      }else{
        // Passe cap, lasttouched, currtouched par référence !
        localMode(cap, lasttouched, currtouched);
      }
    }
  }
}
