# Roboter-Roboter-Kommunikation
Dieses interdisziplinare Projekt haben wir in einem Team von insgesamt vier Studierenden umgesetzt. Drei der Studierenden waren vom Studiengang Elektrotechnik und ich von Informatik.
Die Bestandteile des Projekts, die in diesem Repository zu finden sind, wurden von mir entwickelt.
Die anderen Bestandteile, wie z.B. die Programmierung auf den Robotern selbst, wurde von meinen Kommolitonen umgesetzt und sind hier nicht zu finden.

## Beschreibung
Das Projekt soll die Kommunikation zwischen zwei Robotern und einem PC über ein IIOT-Protokoll ermöglichen, die in einem gemeinsamen Labornetzwerk miteinander verbunden sind. Dazu wurden folgende Komponenten implementiert:
- [x] TCP-Client zur Kommunikation mit der Mitsubishi/Festo Schulungszelle
- [x] TCP-Server zur Kommunikation mit der KUKA Schulungszelle
- [x] Kommunikation mit dem KUKA LBR iiwa
- [x] übergeordneter Server zum Sammeln und Senden von Daten von und zu den Robotern
- [x] GUI zum Anzeigen und Visualisieren der Daten, die von den Robotern gesendet werden, sowie die Steuerung der Roboter ermöglicht

## connected-robots
Dies ist der Client für einen Roboter. Mit ihm verbindet sich der Roboter über TCP. Der Client selber stellt eine Verbindung zum MQTT-Broker her und ist für die Konvertierung und Weiterleitung der Daten zwischen dem Roboter und MQTT-Broker verantwortlich.

## connected-robots-gui
Dies ist die GUI, die sich mit dem MQTT-Broker verbindet und die Daten visualisiert, die von den Clients gesendet wurden. Außerdem können Befehle an die Clients gesendet werden, sodass diese an die Roboter weitergeleitet werden und sich entsprechend verhalten.

## Interdisziplinares_Projekt-Leonard.pdf
Dies ist der Teil der Abschlusspräsentation, für den ich verantwortlich war und umgesetzt habe.