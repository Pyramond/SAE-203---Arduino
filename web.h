#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>

// SSID & mot de passe du réseau WiFi
extern const char *ssid;
extern const char *password;

// Serveur et client web
extern NetworkServer server;

void setupWeb();
void loopWeb();
void webGame(NetworkClient &c);
void webSolo(NetworkClient &c);

#endif // WEBSERVER_H
