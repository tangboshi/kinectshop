# Aktueller Stand

Nächste Schritte:
* programmierte C++ Funktionen nun mit Javascript ausführen und ans UI binden.

Liste der noch möglichen noch zu implementierenden Features (nicht vollständig, **nicht** nach Prioritäten:)
* Login: Timout bei mehrfacher falscher Eingabe, Passwort ändern, Passwort vergessen
* Account (löschen, bearbeiten, Profil anlegen, ändern)
* Registrierung: Passwortsichherheit fordern
* Einkaufssystem: anzeigen der Produkte, input-Felder mit id, oder name *+pid der Produkte, für amount
* Produktbeschreibungen nach Vorbild eines Templates von codrops
* Warenstatistiken (Umsatz der User/ beliebteste Produkte/Userdatenauswertung)
* Warenstatistiken grafisch darstellen mit JSON und flot
* Kinect: Kinect Farbbild, Kinect Tiefenbild, Kinect Ton
* Interface mit Lokalisierung:  Markieren des übergebenen Bildausschnitts
* Interface mit Klassifizierung:  Anzeigen des identifizierten Produkts

Liste der implementierten Features:
* Funktionierender Login
* Funktionierender Logout
* Funktionierende Registrierung
* Funktionierende Guthabenverwaltung

Momentane technische Schwierigkeiten:
* C++ String an Javascript übergeben (stream cout in buffer geleitet --> string liest aus stream)
* Kinect-Daten auslesen und darstellen in Qt
* JSON mit QT
