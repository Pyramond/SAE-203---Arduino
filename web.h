const char *ssid = "morpion-SAE";
const char *password = "12345678";
NetworkServer server(80);

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
              webSolo();
            }else{
              webDuo();
            }
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





void webDuo(){
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
}


void webSolo(){
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("</head>");

  client.println("<body>");
  client.println("<div class='grid'>");
  client.println("Pas encore commence !")
  client.println("</div>");

  client.println("</body>");
  client.println("</html>");
}