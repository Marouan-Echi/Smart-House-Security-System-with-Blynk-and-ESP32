# Smart-House-Security-System-with-Blynk-and-ESP32
Ce projet vise à créer un système de sécurité intelligent pour la maison basé sur ESP32, intégrant plusieurs capteurs (mouvement, gaz, distance, RFID) et une interface cloud via Blynk pour la surveillance en temps réel.

🛠️ Technologies et composants utilisés
Carte ESP32 (ESP-WROOM-32)

Capteur de mouvement PIR pour la détection d’intrusions

Capteur de gaz MQ-2 pour la détection de fumée/gaz

Lecteur RFID RC522 pour le contrôle d'accès avec badges autorisés

Capteur à ultrasons HC-SR04 pour la détection de proximité

Servo-moteur pour la gestion de l'ouverture de porte

LEDs et buzzer pour les alertes visuelles et sonores

Blynk IoT (via SSL) pour la visualisation des données et les notifications push

NTP pour la synchronisation automatique de l’heure

Arduino IDE pour la programmation

⚙️ Fonctionnalités
🔒 Contrôle d’accès par badge RFID
Ouverture automatique d’une porte si le badge est autorisé.

Refus d’accès et alerte sonore en cas de badge inconnu.

Historique des accès via Blynk (log d’événements).

🔥 Détection de gaz
Lecture continue de la qualité de l’air via MQ-2.

Alerte sonore et notification Blynk si un seuil critique est dépassé.

👁️ Détection de mouvement
Activation de la LED verte si un mouvement est détecté.

Affichage de l’état en temps réel dans l’application Blynk.

📏 Détection de proximité (ultrasons)
Détection d’obstacles à une distance critique.

Alerte sonore et envoi d’une notification via Blynk.

📡 Connectivité
Connexion automatique au Wi-Fi avec reconnexion automatique en cas de perte.

Affichage de l'adresse IP dans le moniteur série.

🧠 Architecture logicielle
Le système est basé sur un cycle loop() temps réel, et utilise :

BlynkTimer pour les envois périodiques de données vers le cloud

Synchronisation NTP pour avoir l’heure exacte

MFRC522 pour le scan RFID

pulseIn() pour mesurer la distance par ultrasons

Lecture analogique/numérique des capteurs pour les décisions

🖥️ Interface Blynk
Les widgets utilisés dans le dashboard Blynk :

Widget	Fonction
V0	État LED verte (mouvement)
V1	État LED rouge (gaz)
V2	Mouvement détecté (PIR)
V3	Distance mesurée (ultrason)
V4	Position du servo (porte)
V5	Valeur du capteur de gaz
V6	Message accès RFID (accordé/refusé)

📦 Organisation des fichiers
bash
Copier
Modifier
├── config.h            # Configuration Wi-Fi, UID RFID autorisés
├── main.ino            # Code principal du projet
├── README.md           # Description du projet
✅ Améliorations futures
Intégration d’un écran OLED ou TFT pour afficher les statuts localement.

Enregistrement des événements sur Firebase ou une base de données distante.

Ajout de reconnaissance faciale ou de commandes vocales.

Application mobile Flutter dédiée.
