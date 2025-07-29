# 🔐 Smart House Security System with ESP32, Blynk, and RFID

![ESP32](https://img.shields.io/badge/Board-ESP32-blue)
![Blynk](https://img.shields.io/badge/Blynk-Enabled-green)
![RFID](https://img.shields.io/badge/RFID-RC522-lightgrey)
![Status](https://img.shields.io/badge/Project-Active-brightgreen)

## 📖 Description

Ce projet implémente un **système de sécurité domestique intelligent** basé sur la carte **ESP32**, utilisant des capteurs pour détecter les mouvements, les gaz, les obstacles, et pour gérer les accès via un **lecteur RFID**. Grâce à **Blynk**, les données sont envoyées vers une application mobile pour permettre une surveillance en **temps réel**.

---

## 🎯 Objectifs

- Contrôle d'accès sécurisé par badge RFID
- Détection de mouvement avec capteur PIR
- Détection de gaz avec MQ-2 (fumée, propane, etc.)
- Détection de proximité avec capteur à ultrasons
- Contrôle d’un servo pour ouverture automatique
- Notifications via Blynk et suivi en direct

---

## 🛠️ Matériel utilisé

| Composant               | Rôle                                |
|------------------------|-------------------------------------|
| ESP32 DevKit V1        | Microcontrôleur principal           |
| Capteur PIR            | Détection de mouvement              |
| Capteur MQ-2           | Détection de gaz/fumée              |
| Capteur à ultrasons HC-SR04 | Détection de proximité          |
| Lecteur RFID RC522     | Contrôle d’accès par badge          |
| Servo-moteur           | Ouverture/fermeture de porte        |
| LEDs + Buzzer          | Signalisations sonores et visuelles |
| Application Blynk      | Interface mobile de surveillance    |

---

## 🧪 Fonctionnalités

✅ **RFID** :  
- Lecture de badge et comparaison avec liste autorisée (`validUIDs`)
- Ouverture automatique via servo en cas d’accès autorisé
- Rejet et alerte sonore en cas de badge inconnu  
- Log des accès dans Blynk (V6)

✅ **Détection de gaz (MQ-2)** :  
- Lecture analogique continue
- Alerte sonore + notification Blynk en cas de dépassement de seuil

✅ **Capteur PIR** :  
- Détection de mouvement
- Allumage LED verte et signal dans Blynk (V2)

✅ **Capteur à ultrasons** :  
- Mesure de la distance
- Détection d’obstacle à moins de 10 cm
- Alerte sonore + notification Blynk (V3)

✅ **Surveillance cloud avec Blynk** :  
- Visualisation en temps réel
- Notifications d’alerte (gaz, obstacle, badge inconnu)
- Dashboard configurable

---

## 📲 Dashboard Blynk (ESP32 SSL)

Widgets utilisés :

| Pin Virtuel | Fonction                          |
|-------------|-----------------------------------|
| V0          | LED verte (mouvement)             |
| V1          | LED rouge (gaz)                   |
| V2          | Mouvement détecté (PIR)           |
| V3          | Distance en cm (ultrasons)        |
| V4          | Position du servo (angle)         |
| V5          | Valeur brute du capteur de gaz    |
| V6          | Résultat RFID : accès oui/non     |

---

## 🔗 Architecture logicielle

graph TD
  A[ESP32] -->|PIR| B[Mouvement détecté]
  A -->|MQ2| C[Détection gaz]
  A -->|Ultrason| D[Détection obstacle]
  A -->|RC522| E[Badge RFID]
  A -->|Wi-Fi| F[Blynk]
  E --> G{UID autorisé ?}
  G -- Oui --> H[SERVOMOTEUR : Ouvrir porte]
  G -- Non --> I[BUZZER + LED rouge]

##🔧 Configuration
Dans config.h :

#define BLYNK_AUTH_TOKEN "TON_TOKEN_BLYNK"
const char* WIFI_SSID = "TON_SSID";
const char* WIFI_PASSWORD = "TON_MDP";
const String validUIDs[] = { "3351ab0d" };

##🚀 Mise en route
1. Ouvrir le projet dans l'IDE Arduino

2. Installer les bibliothèques suivantes :
Blynk
MFRC522
ESP32Servo

3. Modifier le fichier config.h avec vos informations personnelles

4. Flasher le code sur l’ESP32

5. Ouvrir l'application Blynk et créer les widgets mentionnés ci-dessus

6. Lancer le moniteur série et tester les capteurs

## 📦 Structure du projet
📁 SmartHouseSecurity/
├── config.h             # Configuration Wi-Fi et RFID
├── main.ino             # Code principal
├── images               #contient les images réels 
├── README.md            # Documentation du projet
##🧠 À venir
* Ajout d’un écran TFT/OLED pour affichage local

* Intégration Firebase pour logs longue durée

* Authentification à double facteur (ex: badge + code)

* Envoi automatique d’email en cas d’alerte

