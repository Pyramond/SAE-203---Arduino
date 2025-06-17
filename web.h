#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

extern const char *ssid;
extern const char *password;

extern WiFiServer server;

void setupWeb();
void loopWeb();
void webGame(WiFiClient &c);
void webSolo(WiFiClient &c);

#endif // WEBSERVER_H
