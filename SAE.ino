#include "pin.h"
#include "logo.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
const char *ssid = "morpion-SAE";
const char *password = "12345678";
NetworkServer server(80);

#define BUTTON_A 15
#define BUTTON_B 32

int tour=0;

bool local = true;
bool solo = true;
int difficulty = 1;

bool isMod = false;
bool isModJeu = false;
bool isDifficulty = false;
bool isPlay = false;
bool isWebSet = false;

uint8_t btnPrevA;
uint8_t btnPrevB;
uint8_t btnA;
uint8_t btnB;

unsigned long lastDebounceTimeA = 0;
unsigned long lastDebounceTimeB = 0;
const unsigned long debounceDelay = 200; // délai de rebond (200 ms)


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

void loopWeb(){
  NetworkClient client = server.accept();  

  if (client) {
    Serial.println("New Client.");
    String currentLine = ""; 
    while (client.connected()) {
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);
        if (c == '\n') {            // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK) and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            //client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            //client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");
            
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<style>");
            client.println("body { margin: 0; padding: 0; }");
            client.println(".grid { display: grid; grid-template-columns: repeat(3, 1fr); grid-template-rows: repeat(3, 1fr); height: 100vh; width: 100vw; }");
            client.println(".cell { border: 1px solid black; display: flex; justify-content: center; align-items: center; font-size: 3em; }");
            client.println("</style>");
            client.println("</head>");

            client.println("<body>");
            client.println("<div class='grid'>");
            for (int i = 0; i < 9; i++) {
              client.print("<div class='cell' id='");
              client.print(i + 1);
              client.print("'>");
              client.print(i + 1);
              client.println("</div>");
            }
            client.println("</div>");

            client.println("<script>");
            client.println("var joueur = 1;");
            client.println("var etat = [0,0,0,0,0,0,0,0,0];");
            client.println("var jeuFini = false;");
            client.println("var combinaisons = [[0,1,2],[3,4,5],[6,7,8],[0,3,6],[1,4,7],[2,5,8],[0,4,8],[2,4,6]];");

            client.println("var cells = document.querySelectorAll('.cell');");
            client.println("for (var i=0; i<cells.length; i++) {");
            client.println("  cells[i].addEventListener('click', function() {");
            client.println("    if (jeuFini) return;");
            client.println("    var index = Array.prototype.indexOf.call(cells, this);");
            client.println("    if (etat[index] === 0) {");
            client.println("      etat[index] = joueur;");
            client.println("      if (joueur === 1) {");
            client.println("        this.style.backgroundColor = 'blue';");
            client.println("      } else {");
            client.println("        this.style.backgroundColor = 'red';");
            client.println("      }");

            client.println("      if (verifieVictoire(joueur)) {");
            client.println("        alert('Le joueur ' + joueur + ' a gagne !');");
            client.println("        jeuFini = true;");
            client.println("      } else if (etat.indexOf(0) === -1) {");
            client.println("        alert('Match nul !');");
            client.println("        jeuFini = true;");
            client.println("      } else {");
            client.println("        if (joueur === 1) { joueur = 2; } else { joueur = 1; }");
            client.println("      }");
            client.println("    }");
            client.println("  });");
            client.println("}");

            client.println("function verifieVictoire(j) {");
            client.println("  for (var i=0; i<combinaisons.length; i++) {");
            client.println("    var c = combinaisons[i];");
            client.println("    if (etat[c[0]] === j && etat[c[1]] === j && etat[c[2]] === j) {");
            client.println("      return true;");
            client.println("    }");
            client.println("  }");
            client.println("  return false;");
            client.println("}");
            client.println("</script>");

            client.println("</body>");
            client.println("</html>");

            client.println();
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        } 

        if (currentLine.endsWith("GET /H")) {
          //digitalWrite(LED_BUILTIN, HIGH);
        }
        if (currentLine.endsWith("GET /L")) {
          //digitalWrite(LED_BUILTIN, LOW);
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
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

void setupWeb(){
  Serial.println();
  Serial.println("Configuring access point...");
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Connectez vous au\nreseau ");
  display.println(ssid);
  display.println();
  display.print("Puis accedez a \nl'adresse ");
  display.print(myIP);
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
