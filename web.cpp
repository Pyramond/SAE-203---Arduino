#include "web.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "localMode.h"

extern Adafruit_SH1107 display;

const char *ssid = "morpion-SAE";
const char *password = "12345678";
WiFiServer server(80);

void setupWeb() {
  turnOff();
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
  display.println("Connectez-vous a :");
  display.print("Nom : ");
  display.println(ssid);
  display.print("Mdp : ");
  display.println(password);
  display.println();
  display.print("Puis accedez a\nl'adresse ");
  display.print(myIP);
  display.display();
}

void webGame(WiFiClient &c) {
  c.println(R"rawliteral(

<!DOCTYPE html>
  <html lang="fr">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>Morpion Duo/Solo</title>
    <style>
      body {
        margin: 0; padding: 0;
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        background: #121212;
        color: #eee;
        display: flex;
        flex-direction: column;
        height: 100vh;
        align-items: center;
        justify-content: center;
      }
      h1 { margin-bottom: 0.2em; }
      .menu {
        margin-bottom: 10px;
        display: flex;
        gap: 15px;
        align-items: center;
      }
      button.recommencer {
        background-color: #6200ea;
        border: none;
        color: white;
        padding: 12px 25px;
        font-size: 1.2em;
        border-radius: 5px;
        cursor: pointer;
        box-shadow: 0 4px 6px rgba(0,0,0,0.3);
        transition: background-color 0.3s ease;
      }
      button.recommencer:hover {
        background-color: #3700b3;
      }
      select {
        font-size: 1.1em;
        padding: 6px 10px;
        border-radius: 5px;
        border: none;
        background-color: #333;
        color: #eee;
        cursor: pointer;
        box-shadow: 0 2px 4px rgba(0,0,0,0.5);
        transition: background-color 0.3s ease;
      }
      select:hover {
        background-color: #555;
      }
      .status {
        margin-bottom: 15px;
        font-size: 1.3em;
        height: 1.6em;
        color: #bb86fc;
        font-weight: bold;
      }
      .grid {
        display: grid;
        grid-template-columns: repeat(3, 100px);
        grid-template-rows: repeat(3, 100px);
        gap: 8px;
        user-select: none;
        margin-bottom: 15px;
      }
      .cell {
        background-color: #1f1f1f;
        border-radius: 10px;
        display: flex;
        justify-content: center;
        align-items: center;
        font-size: 3.5em;
        font-weight: bold;
        cursor: pointer;
        transition: background-color 0.3s ease, transform 0.15s ease;
        box-shadow: 0 3px 6px rgba(0,0,0,0.5);
      }
      .cell:hover {
        background-color: #3700b3;
        transform: scale(1.05);
      }
      .blue { color: #2196F3; }
      .red { color: #f44336; }
      .winner {
        background-color: #03dac6 !important;
        color: #000 !important;
        animation: pulse 1s infinite;
      }
      @keyframes pulse {
        0%, 100% { box-shadow: 0 0 10px 5px #03dac6; }
        50% { box-shadow: 0 0 20px 10px #03dac6; }
      }
    </style>
  </head>
  <body>
    <h1>Morpion</h1>
    <div class="menu">
      <label for="modeSelect">Mode :</label>
      <select id="modeSelect">
        <option value="duo" selected>Duo (2 joueurs)</option>
        <option value="soloF">Solo (contre IA Facile)</option>
        <option value="soloD">Solo (contre IA Difficile)</option>
      </select>
    </div>
    <div class="status">Joueur 1 (Bleu) à jouer</div>
    <div class="grid">
      <div class="cell" id="0"></div>
      <div class="cell" id="1"></div>
      <div class="cell" id="2"></div>
      <div class="cell" id="3"></div>
      <div class="cell" id="4"></div>
      <div class="cell" id="5"></div>
      <div class="cell" id="6"></div>
      <div class="cell" id="7"></div>
      <div class="cell" id="8"></div>
    </div>
    <button class="recommencer">Recommencer</button>
    <script>
      const modeSelect = document.getElementById('modeSelect');
      const status = document.querySelector('.status');
      const cells = document.querySelectorAll('.cell');
      const combinaisons = [
        [0,1,2], [3,4,5], [6,7,8],
        [0,3,6], [1,4,7], [2,5,8],
        [0,4,8], [2,4,6]
      ];
      let joueur = 1;
      let etat = [0,0,0,0,0,0,0,0,0];
      let jeuFini = false;
  
      function verifieVictoire(j) {
        for (let c of combinaisons) {
          if (etat[c[0]] === j && etat[c[1]] === j && etat[c[2]] === j) {
            return c;
          }
        }
        return null;
      }
  
      function declareVictoire(cellsIndices) {
        jeuFini = true;
        for (let i of cellsIndices) {
          cells[i].classList.add('winner');
        }
        status.textContent = `Le joueur ${joueur} a gagné !`;
      }
  
      function declareMatchNul() {
        jeuFini = true;
        status.textContent = "Match nul !";
      }
  
      function updateStatus() {
        if (!jeuFini) {
          if (modeSelect.value === 'duo') {
            status.textContent = `Joueur ${joueur} (${joueur === 1 ? 'Bleu' : 'Rouge'}) à jouer`;
          } else {
            status.textContent = joueur === 1 ? 'Votre tour (Bleu)' : 'Tour de l\'IA (Rouge)';
          }
        }
      }

      function getIaMove() {
        if(modeSelect.value == "soloF") {

          let libres = [];
          etat.forEach((v, i) => { if (v === 0) libres.push(i); });
          if (libres.length === 0) return;
          return libres[Math.floor(Math.random() * libres.length)];

        } else if(modeSelect.value == "soloD") {

          for (let l of combinaisons) {
              let vals = l.map(i => etat[i]);
              if (vals.filter(v => v == 2).length == 2 && vals.includes(0))
                  return l[vals.indexOf(0)];
          }
          for (let l of combinaisons) {
              let vals = l.map(i => etat[i]);
              if (vals.filter(v => v == 1).length == 2 && vals.includes(0))
                  return l[vals.indexOf(0)];
          }
          if (etat[4] == 0) return 4;
          for (let i of [0,2,6,8]) if (etat[i] == 0) return i;
          for (let i = 0; i < 9; i++) if (etat[i] == 0) return i;

        }
      }

  
      function iaPlay() {
        if (jeuFini) return;
        let choix = getIaMove();
        etat[choix] = 2;
        let cell = cells[choix];
        cell.textContent = 'O';
        cell.classList.add('red');
        let vic = verifieVictoire(2);
        if (vic) {
          joueur = 2;
          declareVictoire(vic);
        } else if (!etat.includes(0)) {
          declareMatchNul();
        } else {
          joueur = 1;
          updateStatus();
        }
      }
  
      function resetGame() {
        jeuFini = false;
        joueur = 1;
        etat.fill(0);
        cells.forEach(c => {
          c.textContent = '';
          c.classList.remove('blue', 'red', 'winner');
        });
        updateStatus();
        if ((modeSelect.value === 'soloF' || modeSelect.value === 'soloD') && joueur === 2) {
          iaPlay();
        }
      }
  
      cells.forEach((cell, index) => {
        cell.addEventListener('click', () => {
          if (jeuFini || etat[index] !== 0) return;
          if (modeSelect.value === 'duo') {
            etat[index] = joueur;
            cell.textContent = joueur === 1 ? 'X' : 'O';
            cell.classList.add(joueur === 1 ? 'blue' : 'red');
            let vic = verifieVictoire(joueur);
            if (vic) {
              declareVictoire(vic);
            } else if (!etat.includes(0)) {
              declareMatchNul();
            } else {
              joueur = joueur === 1 ? 2 : 1;
              updateStatus();
            }
          } else {
            if (joueur !== 1) return;
            etat[index] = 1;
            cell.textContent = 'X';
            cell.classList.add('blue');
            let vic = verifieVictoire(1);
            if (vic) {
              declareVictoire(vic);
            } else if (!etat.includes(0)) {
              declareMatchNul();
            } else {
              joueur = 2;
              updateStatus();
              setTimeout(iaPlay, 700);
            }
          }
        });
      });
  
      document.querySelector('button.recommencer').addEventListener('click', resetGame);
      modeSelect.addEventListener('change', resetGame);
  
      updateStatus();
    </script>
  </body>
  </html>
)rawliteral");
}

void loopWeb() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            webGame(client);
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}