#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>

const char *ssid = "morpion-SAE";
const char *password = "12345678";
NetworkServer server(80);
NetworkClient client = server.accept();  

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





void webDuo(NetworkClient &c){
  c.println("<!DOCTYPE html>");
  c.println("<html>");
  c.println("<head>");
  c.println("  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  c.println("  <style>");
  c.println("    body { margin: 0; padding: 0; height:100% }");
  c.println("    html { height: 100%; }");
  c.println("    .menu {");
  c.println("        display: flex;");
  c.println("        justify-content: center;");
  c.println("        margin: 1%;");
  c.println("    }");
  c.println("    .grid {");
  c.println("        display: flex;");
  c.println("        width: 100%;");
  c.println("        height: 90%;");
  c.println("    }");
  c.println("    .column {");
  c.println("        width: 100%;");
  c.println("        height: 100%;");
  c.println("    }");
  c.println("    .cell {");
  c.println("      border: 1px solid black;");
  c.println("      display: flex;");
  c.println("      justify-content: center;");
  c.println("      align-items: center;");
  c.println("      font-size: 3em;");
  c.println("      cursor: pointer;");
  c.println("      user-select: none;");
  c.println("      height: 33%;");
  c.println("    }");
  c.println("  </style>");
  c.println("</head>");
  c.println("<body>");
  c.println("    <div class=\"menu\">");
  c.println("        <button class=\"recommencer\">Recommencer</button>");
  c.println("    </div>");
  c.println("    <div class=\"grid\">");
  c.println("        <div class=\"column\">");
  c.println("            <div class=\"cell\" id=\"1\">1</div>");
  c.println("            <div class=\"cell\" id=\"2\">2</div>");
  c.println("            <div class=\"cell\" id=\"3\">3</div>");
  c.println("        </div>");
  c.println("        <div class=\"column\">");
  c.println("            <div class=\"cell\" id=\"4\">4</div>");
  c.println("            <div class=\"cell\" id=\"5\">5</div>");
  c.println("            <div class=\"cell\" id=\"6\">6</div>");
  c.println("        </div>");
  c.println("        <div class=\"column\">");
  c.println("            <div class=\"cell\" id=\"7\">7</div>");
  c.println("            <div class=\"cell\" id=\"8\">8</div>");
  c.println("            <div class=\"cell\" id=\"9\">9</div>");
  c.println("        </div>");
  c.println("    </div>");

  c.println("    <script>");
  c.println("        let joueur = 1;");
  c.println("        let etat = [0,0,0,0,0,0,0,0,0];");
  c.println("        let jeuFini = false;");
  c.println("        let combinaisons = [");
  c.println("        [0,1,2],");
  c.println("        [3,4,5],");
  c.println("        [6,7,8],");
  c.println("        [0,3,6],");
  c.println("        [1,4,7],");
  c.println("        [2,5,8],");
  c.println("        [0,4,8],");
  c.println("        [2,4,6]");
  c.println("        ];");

  c.println("        let cells = document.querySelectorAll('.cell');");
  c.println("        for (let i=0; i<cells.length; i++) {");
  c.println("        cells[i].addEventListener('click', function() {");
  c.println("            if (jeuFini) return;");
  c.println("            let index = Array.prototype.indexOf.call(cells, this);");
  c.println("            if (etat[index] === 0) {");
  c.println("            etat[index] = joueur;");
  c.println("            if (joueur === 1) {");
  c.println("                this.style.backgroundColor = 'blue';");
  c.println("            } else {");
  c.println("                this.style.backgroundColor = 'red';");
  c.println("            }");

  c.println("            sleep(10).then(() => {");
  c.println("                if (verifieVictoire(joueur)) {");
  c.println("                alert('Le joueur ' + joueur + ' a gagne !');");
  c.println("                jeuFini = true;");
  c.println("            } else if (etat.indexOf(0) === -1) {");
  c.println("                alert('Match nul !');");
  c.println("                jeuFini = true;");
  c.println("            } else {");
  c.println("                joueur = (joueur === 1) ? 2 : 1;");
  c.println("            }");
  c.println("            })");
  c.println("            }");
  c.println("        });");
  c.println("        }");

  c.println("        function verifieVictoire(j) {");
  c.println("        for (let i=0; i<combinaisons.length; i++) {");
  c.println("            let c = combinaisons[i];");
  c.println("            if (etat[c[0]] === j && etat[c[1]] === j && etat[c[2]] === j) {");
  c.println("            return true;");
  c.println("            }");
  c.println("        }");
  c.println("        return false;");
  c.println("        }");

  c.println("        function sleep(ms) {");
  c.println("            return new Promise(resolve => setTimeout(resolve, ms));");
  c.println("        }");

  c.println("        let recommencer = document.querySelectorAll('.recommencer');");
  c.println("        for (let i=0; i<recommencer.length; i++) {");
  c.println("            recommencer[i].addEventListener('click', function() {");
  c.println("                for (let j=0; j<cells.length; j++) {");
  c.println("                    cells[j].style.backgroundColor = 'white';");
  c.println("                }");
  c.println("                jeuFini = false;");
  c.println("                joueur = 1;");
  c.println("                for(let j=0; j<etat.length; j++){");
  c.println("                    etat[j]=0;");
  c.println("                }");
  c.println("            });");
  c.println("        }");
  c.println("    </script>");
  c.println("</body>");
  c.println("</html>");
}


void webSolo(NetworkClient &c){
  c.println("<!DOCTYPE html>");
  c.println("<html>");
  c.println("<head>");
  c.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  c.println("</head>");

  c.println("<body>");
  c.println("<div class='grid'>");
  c.println("Pas encore commence !");
  c.println("</div>");

  c.println("</body>");
  c.println("</html>");
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
            if(solo){
              webSolo(client);
            }else{
              webDuo(client);
            }
            client.println();
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        } 
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}