Nicole Pehr
Team 8 - Perio
IMD SS SS23 - h_da

### Inbetriebnahme

----> Periodentracker

- 1 x RFID RC522 mit Reader & Chip 13,56MHz SPI
- 1x DC 3 V 80mA Mini Vibrationsmotoren 11000 RPM 1020 
- 1x USB-Kabel A
- 1x Breadboard
- 2x Krokodilklemmen mit Kabel
- 1x LED
- 20x Jumper-Wire

1. [ArduinoIDE](https://www.arduino.cc/en/software) installieren
2. Arduino Uno R3 mit dem USB-Kabel an den PC anschließen
3. Die Messen_beschreiben.ino in der ArduinoIDE öffnen
4. Den Code über die ArduinoIDE auf den Arduino Uno R3 aufspielen
5. Die oben beschriebenen Komponenten wie im folgenden Fritzing gezeigt verbinden

[P4_SS23 Fritzing-Perio_Pehr.pdf](https://github.com/P4CreativeCoding/team8-Nicole/files/11962202/P4_SS23.Fritzing-Perio_Pehr.pdf)

Um das Periodentracking zu simulieren, wird eine RFID Gerät inklusive Chip verwendet (rechts im Bild). Diese repräsentiert den Writer. Auf dieser sind ebenfalls eine LED, ein Vibrationsmotor sowie drei 220 Ohm Widerstände angeschlossen. Um die Berührung zu messen müssen die zwei Ende des orangenen und des grünen Jumper-Wire berührt werden. Wurde die Berührung erfolgreich gemessen vibriert der Vibrationsmotor kurz um so die Messung zu bestätigen. Um über die Messung ein direktes Feedback über die gemessenen Daten zu bekommen werden LEDs verwendet. Diese leuchtet bei Berührung ebenfalls auf.

Das Datum, an dem die Jumper-Wire berührt wurden wird gespeichert und auf den RFID Reader übertragen, um dann mithilfe des RFID Chips dieses Datum am Auslesegerät zu übetragen.

----> Auslesegrät

- 1x RFID RC522 mit Reader & Chip 13,56MHz SPI
- 1x LED
- 1x Arduino UNO und USB-Kabel A
- 1x Breadboard
- 1x Widerstand
- 10x Jumper-Wire

1. Arduino Uno R3 mit dem USB-Kabel an den PC anschließen
2. Die auslesen.ino in der ArduinoIDE öffnen
3. Den Code über die ArduinoIDE auf den Arduino Uno R3 aufspielen
4. Die oben beschriebenen Komponenten wie im folgenden Fritzing gezeigt verbinden

Der Schaltplan (links im Bild) zeigt ein Breadboard, welches mit einem Microcontroller, in diesem Fall ein Arduino UNO, angeschlossen ist.

Das RFID Gerät repräsentiert den Reader. Hier ist eine LED mit einem 220 Ohm Widerstand in den Stromfluss integriert, diese soll leuchten, wenn ein Chip ausgelesen wird.
