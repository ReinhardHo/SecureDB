\mainpage Vorwort

Dieses Programm SecureDB (Secure-Datenbank) dient zur verschlüsselten Speicherung
von kurzen Texten bis zu ca. 4000 Zeichen Länge. Insbesondere Zugangsdaten zu
verschiedenen Accounts können damit verwaltet (und unter verschiedenen Namen
abgelegt) werden. Ein typischer Eintrag könnte zum Beispiel wie folgt aussehen:

    Amazon
     Zugang: www.amazon.de
     User:   reinhard.hoelscher@t-online.de
     PW:     itzelbritzel
     Datum:  22.01.2012
     
Der dargestellte Text ist nur 119 Zeichen lang. Er beschreibt in diesem Beispiel
einen Zugang zu einem Amazon Konto mit der zugehörigen Internetadresse, dem
Usernamen, dem zum Usernamen gehörendem Passwort und einem Datum der letzten
Änderung. Aber auch PINs, PUKs und TANs oder andere sicherheitskritische Informationen
können mit diesem Programm verwaltet werden.


\page Installation

Kopieren sie alle Dateien aus dem Unterverzeichnis **Install** in ein 
Installationsverzeichnis ihrer Wahl. Legen sie eine Verknüpfung an, die auf die 
Datei **SecureDB.exe** in ihrem Installationsverzeichnis verweist. Nun sollte
alles funktioniern.

Dieses Programm benötigt unter anderem die Programmbibliothek OpenSSL in der
Version 3.x. Sollte eine Version 3.x der Programmbibliothek OpenSSL bereits auf
ihrem Computer installiert sein, so kann beim Kopieren die Datei libcrypto-3-x64.dll
weggelassen werden.


\page Programmbedienung

![Programmoberfläche nach dem ersten Programmstart](Screenshot_2026-04-20.png)

Nach dem Programmstart läuft dieses Programm im Normalmodus. In diesem Modus können
keine verschlüsselten Daten erzeugt und gespeichert werden. Das Programm verhält sich
vielmehr wie ein einfacher Editor. Es können Textdateien geladen, verändert und wieder
(unverschlüsselt) gespeichert werden.

Die Hintergrundfarbe im Editor ist in diesem Modus rot. Soll bedeuten: Achtung
unverschlüsselt! Wird der Text verändert, wechselt die Hintergrundfarbe zu gelb. Nach der
Speicherung wechselt die Hintergrundfarbe wieder zu Rot.

Soll in den (sicheren) Secure-Modus gewechselt werden, so ist zunächst mindesten ein
Secure-Store anzulegen.
    
<br>
<br>

Öffnen ...
----------

Diese Funktion kann über den Menüpunkt: **Datei->Öffnen …**, die Tastenkombination
**Strg+O** oder den **zweiten Schalter** in der Toolbar-Leiste erreicht werden.

Im Normalmodus kann mit dieser Funktion eine Datei in den Editor geladen werden.

Im Securemodus öffnet sich der nachfolgende Dialog:

![Öffnen-Dialog im Securemodus](Screenshot_2026-05-13.png)

Aus der Liste der Einträge (in diesem Beispiel 87) kann nun ein Eintrag ausgewählt
und durch den Klick auf **OK** im Editor (unverschlüsselt) angezeigt werden.

Durch Klicks auf den Schalter Umsortieren werden die Einträge in der Liste jeweils
umsortiert (von A-Z oder von Z-A).

Durch Klicks auf den Schalter **Mülleimer** kann zwischen regulären und als gelöscht
markierten Secure-Store-Einträgen hin und her gewechselt werden.

<br>
<br>

Speichern
---------

Diese Funktion kann über den Menüpunkt: **Datei->Speichern**, die Tastenkombination
**Strg+S** oder den **dritten Schalter** in der Toolbar-Leiste erreicht werden.

Im Normalmodus kann mit dieser Funktion die Datei, die zuvor in den Editor geladen und
bearbeitet wurde, wieder gespeichert werden.

Im Securemodus kann mit dieser Funktion ein Eintrag, der zuvor in den Editor geladen und
bearbeitet wurde, wieder (unter dem neuen Datum) gespeichert werden. Ein ggf. schon
vorhandener älterer Eintrag bleibt unverändert erhalten.

<br>
<br>

Speichern unter ...
-------------------

Diese Funktion kann über den Menüpunkt: **Datei->Speichern unter …** oder die
Tastenkombination **Strg+Alt+S** erreicht werden.

Im Normalmodus kann mit dieser Funktion die Datei, die zuvor in den Editor geladen und
bearbeitet wurde, unter einem anderen Namen wieder gespeichert werden.

Im Securemodus kann mit dieser Funktion ein Eintrag, der zuvor in den Editor geladen und
bearbeitet wurde, unter einem anderen Namen wieder gespeichert werden.

<br>
<br>

Beenden
-------

Diese Funktion kann über den Menüpunkt: **Datei->Beenden**, die Tastenkombination
**Strg+Q** oder den **ersten Schalter** in der Toolbar-Leiste erreicht werden.

Dieses Programm wird damit beendet. Die Lage und Größe des Programmfensters auf dem
Desktop sowie der letzte geöffnete Secure-Store werden gesichert. Beim nächsten
Programmstart erscheint das Programm deshalb in derselben Lage und Größe auf dem Desktop,
in der es zuvor beendet wurde.

<br>
<br>

Neuen Secure-Store anlegen ...
------------------------------

Zum Erstellen eines neuen Secure-Store ist unter dem Menüpunkt
**Extras->Neue Secure-Container anlegen …** der oberste Menüpunkt auszuwählen. 

Nachdem im Standard-Datei-Dialog von Windows ein Dateiname in einem beliebigen Ordner
mit Schreibrechten festgelegt wurde, erscheint nachfolgender Dialog:

![Secure-Informationen-Dialog im Normalmodus](Screenshot2026-04-28.png)

In diesem Dialog sind zwei Passworte zu vergeben, die mindestens acht Zeichen lang sein
müssen. Weitere Anforderungen an die Passworte werden nicht gestellt. Die beiden Passworte
dürfen sogar gleich sein. Die Sicherheit wird jedoch durch verschiedene Passwörter erhöht.

Ebenso sind zwei Hashwerte zu vergeben, deren Wert mindestens 10 betragen muss (nach oben
gibt es keine Begrenzung). Auch die beiden Hashwerte dürfen gleich groß sein. Die
Sicherheit wird jedoch durch verschiedene Werte erhöht.

Zum Schluss sind noch ein Startindex (Wertebereich von 1 – 900.000) und ein Differenzwert
(Wertebereich von 1 – 250.000) zu vergeben.

Um Tippfehler zu vermeiden sind alle Werte unter der Spalte **Eingabewiederholungen**
korrekt zu wiederholen.

Wenn keine Fehler aufgetreten sind, wird der neue Store angelegt und in der Statuszeile
erscheint die Meldung: **Generierung erfolgreich abgeschlossen!** Der neu angelegt Store
wird außerdem in der Toolbar-Leiste als zurzeit ausgewählt angezeigt.

<br>
<br>

Secure Store auswählen ...
--------------------------

Sind vom Benutzer mehrere Secure-Stores angelegt worden, so kann unter dem Menüpunkt
**Extras->Secure-Store auswählen …** ein anderer Secure-Store aktiviert werden. Der
ausgewählte Store wird danach in der Toolbar-Leiste angezeigt.

Dieser Menüpunkt ist nur im Normalmodus des Programms verfügbar.

<br>
<br>

Store zusammen führen ...
-------------------------

Dieser Menüpunkt ist nur im Securemodus des Programms verfügbar.

Es kann ein weiterer Secure-Store ausgewählt werden. Werden die Passwortdaten für diesen
Secure-Store korrekt angegeben, so werden alle Einträge aus diesem Secure-Store, die noch
nicht im aktuellen Secure-Store vorhanden sind, in den aktuellen Secure-Store kopiert.

<br>
<br>

Gelöschte Elemente in einen anderen Store kopieren ...
------------------------------------------------------

Es kann ein weiterer Secure-Store ausgewählt werden. Werden die Passwortdaten für diesen
Secure-Store korrekt angegeben, so werden alle gelöschten Einträge aus dem aktuellen Store,
die boch nicht im ausgewählten Merge-Store vorhanden sind, in diesen kopiert.

<br>
<br>

Sprachauswahl ...
-----------------

Diese Funktion kann über den Menüpunkt: **Hilfe->Sprachauswahl …** erreicht werden. Hier
kann eingestellt werden, in welcher Sprache das Programm dargestellt werden soll.

Es erscheint ein Dialog, in dem zwischen verschiedenen Sprachen ausgewählt werden kann.
Derzeit sind nur die beiden Sprachen **Deutsch** und **Englisch** realisiert. Wird die
Sprachauswahl im Dialog verändert, so muss das Programm anschließend beendet werden. Die 
neue Sprachauswahl wird erst mit dem nächsten Programmstart wirksam.

<br>
<br>

Programmmodus wechseln
----------------------

Mit dem **Rot/Weis-Schalter** in der Toolbar-Leiste kann zwischen dem Normalmodus und dem
Securemodus des Programms hin und her gewechselt werden.

Soll vom Normalmodus zum Securemodus gewechselt werden, so erscheint ein Dialog zur
Passworteingabe. Hier sind die Daten einzugeben, die bei der Anlage des Secure-Stores
vergeben wurden.

Beim Wechsel vom Securemodus in den Normalmodus wird ein gegebenenfalls noch im Editor
vorhandener Text vollständig gelöscht. Dies passiert auch dann, wenn der Text noch nicht
gesichert wurde! Plötzlich im Zimmer auftauchenden Personen kann so die Sicht auf
vertrauliche Daten mit einem einfachen Mausklick verwehrt werden.

\page Abkürzungen

AES
---

Der **Advanced Encryption Standard**(**AES**) (deutsch etwa „fortschrittlicher
Verschlüsselungsstandard“) ist eine Blockchiffre, die als Nachfolger des DES im Oktober
2000 vom National Institute of Standards and Technology (NIST) als US-amerikanischer
Standard bekanntgegeben wurde. Der Algorithmus wurde von Joan Daemen und Vincent
Rijmen unter der Bezeichnung **Rijndael** entwickelt. (Dieser Text stammt aus der
deutschen Wikipedia; mehr siehe dort)

<br>
<br>
OTP
---

Das **One-Time-Pad** (Abkürzung: **OTP**, deutsch: Einmalverschlüsselung oder
Einmalschlüssel-Verfahren, wörtlich Einmal-Block, nicht zu verwechseln mit dem
Einmalkennwort) ist ein symmetrisches Verschlüsselungsverfahren zur geheimen
Kommunikation. Kennzeichnend ist, dass ein Schlüssel verwendet wird, der mindestens so
lang wie die Nachricht ist. Das OTP ist informationstheoretisch sicher und kann
nachweislich nicht gebrochen werden, wenn es bestimmungsgemäß genutzt wird.
(Dieser Text stammt aus der deutschen Wikipedia; mehr siehe dort)

<br>
<br>
PBKDF2
------

**PBKDF2** (**Password-Based Key Derivation Function 2**) ist eine genormte Funktion,
um von einem Passwort einen Schlüssel abzuleiten, der in einem symmetrischen
Verfahren eingesetzt werden kann. PBKDF2 ist Bestandteil der Public-Key Cryptography
Standards der RSA-Laboratorien (PKCS #5), wurde im September 2000 auch von der
Internet Engineering Task Force im RFC 2898 veröffentlicht und im Dezember 2010
offiziell vom National Institute of Standards and Technology (NIST) empfohlen.
Der Standard wurde inzwischen überarbeitet und als RFC 8018 im Januar 2017 veröffentlicht.
(Dieser Text stammt aus der deutschen Wikipedia; mehr siehe dort)

<br>
<br>
PIN
---

Eine **Persönliche Identifikationsnummer** (**PIN**), engl. **Personal Identification
Number** oder **Geheimzahl** ist eine nur einer Person offenbarte Ziffernfolge,
mit der diese sich gegenüber einer Maschine authentisieren kann. Häufig werden auch das
redundante Akronym **PIN-Nummer** oder die Bezeichnung **PIN-Code** verwendet.
Im engeren Sinne sind PINs numerische Passwörter.
(Dieser Text stammt aus der deutschen Wikipedia; mehr siehe dort)

<br>
<br>
PUK
---

Ein **Personal Unblocking Key** (**PUK**, oft auch **SuperPIN**) ist ein elektronischer
Schlüssel, der zum Entsperren einer Chipkarte dient, nachdem eine PIN mehrmals falsch
eingegeben worden ist. Beispiele für solche Chipkarten sind SIM-Karten oder der
deutsche Personalausweis mit eID-Funktion.
(Dieser Text stammt aus der deutschen Wikipedia; mehr siehe dort)

<br>
<br>
TAN
---

Eine **Transaktionsnummer** (**TAN**) ist ein Einmalkennwort, das üblicherweise aus
sechs Dezimalziffern besteht und vorwiegend im Online-Banking verwendet wird. Eine
Transaktionsnummer wird üblicherweise benötigt, um eine einzelne Transaktion zu
bestätigen, etwa eine Bank-Überweisung, daher der Name.
(Dieser Text stammt aus der deutschen Wikipedia; mehr siehe dort)












