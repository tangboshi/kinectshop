# Aktueller Stand

Nächste Schritte:
--- in Planung

Liste der noch möglichen noch zu implementierenden Features (nicht vollständig, **nicht** nach Prioritäten):
* Login: Passwort ändern, Passwort vergessen
* Account (löschen, bearbeiten, Profil anlegen, ändern)
* Registrierung: Passwortsichherheit fordern
* Buchung: Unterscheiden zwischen Buchungs-ID und Buchungsgegeständen je Buchung (bid und iid)
* Produktbeschreibungen nach Vorbild eines Templates von codrops
* Warenstatistiken (Umsatz der User/ beliebteste Produkte/Userdatenauswertung)
* Warenstatistiken grafisch darstellen
* Kinect: Kinect Farbbild, Kinect Tiefenbild, Kinect Ton
* Interface mit Lokalisierung:  Markieren des übergebenen Bildausschnitts
* Interface mit Klassifizierung:  Anzeigen des identifizierten Produkts

Liste der implementierten Features:
* Funktionierender Login 
  * Timeout bei mehrfach falscher Eingabe
* Funktionierender Logout
* Funktionierende Registrierung
* Funktionierende Guthabenverwaltung
* Funktionierendes Einkaufssystem (BUG: nur ein Einkauf pro Sitzung BEHEBEN!!!)

Liste der zu lernenden/zum Laufen zu bringenden Techniken/Technologien/Sprachen:
* JSON + flot
* libfreenect

Momentane technische Schwierigkeiten:
* Kinect-Daten auslesen und darstellen in Qt
* JSON mit QT
