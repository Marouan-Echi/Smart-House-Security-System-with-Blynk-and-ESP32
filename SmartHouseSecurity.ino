#define BLYNK_TEMPLATE_ID "TMPL2nfmIOY-M"
#define BLYNK_TEMPLATE_NAME "smartHouse"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <BlynkSimpleEsp32_SSL.h>
#include<config.h>

// Déclaration des pins pour les capteurs
#define PIR_PIN 21       // Pin du capteur PIR
#define MQ2_PIN 34       // Pin analogique du capteur MQ-2
#define LED_GREEN 15     // Pin de la LED verte
#define LED_RED 12       // Pin de la LED rouge
#define SERVO_PIN 33     // Pin du servo-moteur
#define BUZZER_PIN 27    // Pin du buzzer
// RFID
#define RST_PIN 22       // Pin RST du RC522
#define SS_PIN 5         // Pin SDA (SS) du RC522

// Capteur ultrasonique
#define ECHO_PIN 2       // Pin Echo du capteur ultrason
#define TRIG_PIN 4       // Pin Trig du capteur ultrason

long duration;           // Durée de l'impulsion ultrasonique
float distance;          // Distance mesurée en cm

MFRC522 rfid(SS_PIN, RST_PIN);      // Instance du RC522

// Liste des UID autorisés
//const String validUIDs[] = { "3351ab0d" };
//const int validUIDsCount = sizeof(validUIDs) / sizeof(validUIDs[0]);

// Seuils pour les capteurs
const int GAS_THRESHOLD = 600;         // Seuil pour le capteur de gaz
const float DISTANCE_THRESHOLD = 10.0; // Seuil de distance critique en cm

// Déclaration du servo-moteur
Servo servoMotor;

BlynkTimer timer;

void setupTime() {
  configTime(3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Synchronisation avec le serveur NTP...");

  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)) {
    Serial.println("Échec de la synchronisation de l'heure !");
    return;
  }
  Serial.println("Heure synchronisée avec succès.");
  Serial.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");
}


void sendBlynkData() {
  // Envoyer les valeurs aux widgets Blynk
  int pirState = digitalRead(PIR_PIN);
  Blynk.virtualWrite(V2, pirState);

  int gasValue = analogRead(MQ2_PIN);
  Blynk.virtualWrite(V5, gasValue);

  Blynk.virtualWrite(V3, distance);

  // Afficher l'état du servo et des LEDs
  Blynk.virtualWrite(V0, digitalRead(LED_GREEN));  // Afficher l'état de la LED verte
  Blynk.virtualWrite(V1, digitalRead(LED_RED));    // Afficher l'état de la LED rouge
  Blynk.virtualWrite(V4, servoMotor.read());      // Afficher l'angle du servo-moteur
}
void setupWiFi() {
    Serial.print("Connexion au Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nConnecté au Wi-Fi.");
    Serial.println("Adresse IP : " + WiFi.localIP().toString());
}

void checkWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connexion Wi-Fi perdue. Reconnexion...");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
          int attemps= 0;
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
             attemps ++;
           if (attemps > 3){
           ESP.restart();
    }
  }
        }
        Serial.println("\nReconnecté au Wi-Fi.");
    }

void setup() {
  Serial.begin(115200);
    setupWiFi();


  // Synchroniser l'heure avec NTP
  setupTime();

  // Configuration Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN,WIFI_SSID, WIFI_PASSWORD);

  // Configuration du servo et des autres pins
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servoMotor.attach(SERVO_PIN);
  servoMotor.write(0);  // Position initiale du servo

  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Système prêt. En attente de détection...");

  // Initialisation du timer pour Blynk
  timer.setInterval(1000L, sendBlynkData);
}

void loop() {
  checkWiFi();
  Blynk.run();
  timer.run();

  // Lecture du capteur PIR
  int pirState = digitalRead(PIR_PIN);
  digitalWrite(LED_GREEN, pirState);
  if (pirState == HIGH) {
    Serial.println("Mouvement détecté !");
  }

  // Lecture du capteur de gaz
  int gasValue = analogRead(MQ2_PIN);
  Serial.print("Valeur du capteur de gaz : ");
  Serial.println(gasValue);
  if (gasValue > GAS_THRESHOLD) {
    digitalWrite(LED_RED, HIGH);
     digitalWrite(BUZZER_PIN, HIGH); // Activer le buzzer
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);
    
    Serial.println("Alerte : gaz détecté !");
    Blynk.logEvent("detection_gaz", "Détection de gaz");
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER_PIN, LOW); // Désactiver le buzzer

  }

  // Lecture du lecteur RFID
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String cardUID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      cardUID += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      cardUID += String(rfid.uid.uidByte[i], HEX);
    }
    Serial.print("UID de la carte : ");
    Serial.println(cardUID);

    bool isValid = false;
    for (int i = 0; i < validUIDsCount; i++) {
      if (cardUID == validUIDs[i]) {
        isValid = true;
        break;
      }
    }
    int servoPosition = 0;
    if (isValid) {
      Serial.println("Carte valide, ouverture de la porte.");
      servoPosition = 180;
      servoMotor.write(servoPosition);
       delay(5000); // Laisser la porte ouverte pendant 5 secondes

      Blynk.virtualWrite(V6, 1); // Carte valide
      Blynk.virtualWrite(V6, "Accès accordé");
    } else {
      Serial.println("Carte invalide, accès refusé.");
      digitalWrite(LED_RED, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
       delay(1000);
      digitalWrite(LED_RED, LOW);
       digitalWrite(BUZZER_PIN, LOW);
      servoPosition = 0; // Rester fermé
      servoMotor.write(servoPosition);
      Blynk.virtualWrite(V6, 0); // Carte invalide
      Blynk.logEvent("rfid_invalid", "Accès refusé");
    }

    rfid.PICC_HaltA();
  }

  // Lecture du capteur ultrasonique
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  if (distance < DISTANCE_THRESHOLD) {
    Serial.println("Obstacle détecté à proximité !");
    digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
    Blynk.logEvent("detection_chose", "Obstacle détecté");
   }
     else {
        digitalWrite(BUZZER_PIN, LOW);
    }
}
