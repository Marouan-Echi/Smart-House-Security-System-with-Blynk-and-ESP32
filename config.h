#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h> 

// Vos constantes et variables globales
#define BLYNK_AUTH_TOKEN "" // token blynk

const char* WIFI_SSID = "";         // Nom du réseau Wi-Fi
const char* WIFI_PASSWORD = "";     // Mot de passe du réseau Wi-Fi

// Liste des UID autorisés
const String validUIDs[] = { "3351ab0d" };
const int validUIDsCount = sizeof(validUIDs) / sizeof(validUIDs[0]);

#endif  // CONFIG_H
