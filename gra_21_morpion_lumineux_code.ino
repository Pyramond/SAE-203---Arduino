#include "pin.h"
#include "logo.h"
#include "menu.h"
#include "web.h"

#include <SPI.h>
#include <Wire.h>



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