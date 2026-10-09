/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \page Verschlüsselung
 *
 * Eine Verschlüsselung kann in diesem Programm nur erfolgen, wenn zunächst ein Satz
 * von drei Secure-Files's vom Benutzer angelegt wurde (dazu weiter unten mehr).
 * Hierbei werden Schlüssel durch einen Zufallszahlengenerator erzeugt, die nur während
 * der Laufzeit des Programms im Heap-Speicher vorhanden sind und nirgendwo (auch nicht
 * verschlüsselt) gespeichert werden. Wie dieses Verfahren funktioniert, wird weiter
 * unten erläutert. Bei der Neuanlage eines Satzes von drei Secure-Containern werden
 * folgende Schlüssel vom Zufallszahlengenerator erzeugt:
 *
 * -	Ein 4096 Byte langer OTP-Schlüssel
 * -	Ein 256 Byte langer Endemarkenblock, in dem jeder Bytewert (von 0x00 bis 0xff) genau einmal vorkommt
 * -	Ein 256 Byte langer Vertauschungsblock, in dem jeder Bytewert (von 0x00 bis 0xff) genau einmal vorkommt
 * -	Ein erster 32 Byte langer Schlüssel zur ersten AES-Verschlüsselung im dritten SecureStore
 * -	Ein erster 16 Byte langer Initialisierungsvektor zur ersten AES-Verschlüsselung im dritten SecureStore
 * -	Ein zweiter 32 Byte langer Schlüssel zur zweiten AES-Verschlüsselung im dritten SecureStore
 * -	Ein zweiter 16 Byte langer Initialisierungsvektor zur zweiter AES-Verschlüsselung im dritten SecureStore
 *
 * Im Editor dieses Programms können bis zu 4093 Zeichen lange Textblöcke erstellt werden,
 * die dann verschlüsselt werden können. Die einzelnen Verschlüsselungsschritte sollen
 * nachfolgend beschrieben werden. Damit man die einzelnen Verfahrensschritte besser
 * nachvollziehen kann, erfolgen die Erläuterungen anhand eines deutlich kürzeren Klartextblockes
 * von nur 24 Byte Länge. Dieses Programm arbeitet aber mit Klartextblöcken von 4096 Byte
 * Länge. Der im Editor erstellte Text für diese Beschreibung lautet:
 * <span style="color:blue">Ein Testtext.</span> und ist nur 13 Zeichen lang. Für jedes
 * dieser 13 Zeichen wird in der UTF-8 Codierung genau ein Byte belegt. Der Datendump
 * dieser Ausgangsdaten sieht damit wie folgt aus:
 *
 *     Ein 13 Zeichen langer Text:
 *      Index  Inhalt (hexadezimal)                             Inhalt(ASCII)
 *          0  45 69 6e 20 54 65 73 74 74 65 78 74 2e           Ein Testtext.
 *
 * <br><br>
 * Erster Verschlüsselungsschritt
 * ------------------------------
 *
 * Ein 24 Byte langer Klartextblock wird wie folgt mit dem Ausgangstext belegt. Am Ende
 * des Klartextblockes wird zunächst die Länge des Ausgangstextes hinterlegt. Dafür sind
 * bis zu einer Länge von 127 Zeichen ein Byte und bis zu einer Länge von 4093 Zeichen
 * zwei Byte erforderlich. In diesem Beispiel reicht also ein Byte (die Länge ist 13) aus.
 * Davor wird der Ausgangstext platziert. Der Anfang des Klartextblockes wird mit Zeichen aus
 * dem Zufallszahlengenerator gefüllt. Dies sind in diesem Falle 10 Byte. Der
 * Zufallszahlengenerator lieferte für diese Beschreibung die (zufälligen) Bytes 0x22, 0x40,
 * 0x07, 0x8d, 0x3a, 0x2f, 0xc1, 0xda und 0x3d. Vom Programm wird sichergestellt, dass
 * mindestens ein Byte Zufallstext am Anfang des Klartextblockes steht. Bei einem 4096 Byte
 * langen Klartextblock ist der Zufallstext am Anfang des Klartextblockes häufig länger als
 * der Ausgangstext. Der Datendump des Klartextblockes sieht damit wie folgt aus:
 *
 * <pre class="fragment"> Der resultierende 24 Byte lange Klartextblock:
 *   Index  Inhalt (hexadezimal)                             Inhalt (ASCII)
 *       0  <span style="color:green">22 40 07 8d 3a 2f c1 3a da 3d</span> <span style="color:blue">45 69 6e 20 54 65</span>  <span style="color:green">..........</span><span style="color:blue">Ein Te</span>
 *      16  <span style="color:blue">73 74 74 65 78 74 2e</span> <span style="color:red">0d</span>                          <span style="color:blue">sttext.</span><span style="color:red">.</span>
 *      <span style="color:green">Zufallstext grün</span>, <span style="color:blue">Ausgangstext blau</span>, <span style="color:red">Ausgangstextlänge rot</span></pre>
 *
 * <br><br>
 * Zweiter Verschlüsselungsschritt
 * -------------------------------
 *
 * Der Klartextblock aus dem ersten Verschlüsselungsschritt wird nun mit einer Vernam-Chiffre
 * verschlüsselt. Hierzu wird vom Programm ein 4096 Byte langer Schlüssel benutzt, der bei
 * der Einrichtung des SecureStores vom Zufallszahlengenerater generiert wurde. Solch einen
 * Schlüssel nennt man in der Kryptografie One-Time-Pad. Zur Beschreibung dieses Verarbeitungsschrittes
 * in diesem Kapitel wurde vom Zufallszahlengenerator folgender 24 Byte langer One-Time-Pad generiert:
 *
 *      24 Byte langer Schlüssel:
 *       Index  Inhalt (hexadezimal)                             Inhalt (ASCII)
 *           0  3a fb 9b 2f 1a 6f a1 92 9b 1a b9 b2 00 08 e2 35  :../.o.........5
 *          16  af f6 b9 40 df d9 6b 58                          ...@..kX
 *
 * Zum Byte 1 des Klartextblockes (0x22) wird das Byte 1 des One-Time-Pad (0x3a) und das
 * vorangehende Byte des Ergebnisses (hier noch 0x00) addiert. Als Ergebnis ergibt sich der
 * Wert 0x5c. Byte 2 des Klartextblockes (0x40) plus Byte 2 des Schlüssels (0xfb) plus das
 * Ergebnis von Byte 1 (0x5c) ergibt das Ergebnis 0x97 (der Übertrag 256 entfällt). Byte 3
 * des Klartextblockes (0x07) plus Byte 3 des Schlüssels (0x9b) plus das Ergebnis von
 * Byte 2 (0x97) ergibt das Ergebnis 0x39 (der Übertrag 256 entfällt). usw.
 *
 * Für die gesamte Verschlüsselung entsteht folgendes Ergebnis:
 *
 *      24 Byte lange Vernam-Chiffre:
 *       Index  Inhalt (hexadezimal)                             Inhalt (ASCII)
 *           0  5c 97 39 f5 49 e7 49 15 8a e1 df fa 68 90 c6 60  \.9.I.I.....h...
 *          16  82 ec 19 be 15 62 fb 60                          .....b..
 *
 * Soll dieser Verschlüsselungsschritt rückgängig gemacht werden, so muss vom Byte 1 der
 * Vernam-Chiffre (0x5c) Byte 1 des One-Time-Pad (0x3a) und das vorangehende Byte
 * der Vernam-Chiffre (hier noch 0x00) abgezogen werden. Als Ergebnis erhält man 0x22
 * (das erste Byte des Klartextblockes). Byte 2 der Vernam-Chiffre (0x97) minus 0xfb
 * (2. Byte des Schlüssels) minus 0x5c (1. Byte der Vernam-Chiffre) ergibt wieder das
 * zweite Byte des Klartextblockes 0x40. usw.
 *
 * <br><br>
  * Dritter Verschlüsselungsschritt:
 * --------------------------------
 *
 * Bei der Einrichtung des SecureStore wurde vom Zufallszahlengenerater ein 256 Byte großer
 * Block mit Endemarken erzeugt. Alle 256 Endemarken sind verschieden. Dies bedeutet, dass
 * jeder möglich Zahlenwert (von 0x00 bis 0xff) in diesem Endemarkenblock genau einmal vorkommt.
 * Zur Beschreibung dieses Verarbeitungsschrittes in diesem Kapitel wurde vom Zufallszahlengenerator
 * folgender 256 Byte langer Endemarkenblock generiert:
 *
 *     256 Byte langer Endemarkenschlüssel:
 *      Index  Inhatl (hexadezimal)                             Inhalt (ASCII)
 *          0  c4 0a ed 09 2c a9 c6 b2 f2 52 6f 28 e6 32 f4 45  ....,....Ro(.2.E
 *         16  24 23 53 e1 59 ea 12 cd 5b 4b ee a4 d9 ba c0 93  $#S.Y...[K......
 *         32  6a 7f 27 df e9 99 dd 81 ca 83 80 15 2d dc 79 c3  j.'.........-.y.
 *         48  e8 11 b8 3f 1d a1 1b 75 49 cf b5 17 29 67 fb 86  ...?...uI...)g..
 *         64  03 e0 20 90 be 8c cb 63 c7 bc d8 a2 0e 21 7d 04  .. ....c.....!}.
 *         80  85 40 51 ae 16 96 36 bf ad b4 9a 8b 9f 1f 2f 26  .@Q...6......./&
 *         96  37 02 65 0b a5 2b 18 ff 1e b3 a7 87 8f 7a e3 7e  7.e..+.......z.~
 *        112  4a db 4f 78 aa 5e fd 46 d4 33 94 01 5f eb ac 4d  J.Ox.^.F.3.._..M
 *        128  14 de 1a 0f b9 68 31 d5 e4 91 ce 8e b1 3d 92 84  .....h1......=..
 *        144  d0 43 48 fa 55 42 8d 62 08 a3 0d 4c 76 5a b6 d2  .CH.UB.b...LvZ..
 *        160  3e c8 57 7b cc f7 a6 77 38 fc 88 6d c5 f3 5c e7  >.W{...w8..m..\.
 *        176  c9 f1 b0 44 d3 3c 98 69 10 7c 71 64 8a f6 4e 95  ...D.<.i.|qd..N.
 *        192  f0 97 30 66 3a d1 6b f8 da a8 ab 6e 0c 82 50 39  ..0f:.k....n..P9
 *        208  9d ec 6c b7 9b 9e 58 d6 22 06 35 a0 00 5d ef 56  ..l...X.".5..].V
 *        224  af 61 3b 47 bd f9 fe e5 74 25 70 2e 60 bb 13 54  .a;G....t%p....T
 *        240  07 41 1c f5 72 9c 2a c1 34 c2 e2 89 05 d7 19 73  .A..r.*.4......s
 *
 * In diesem Verschlüsselungsschritt werden mehrere 256 Byte große Datenblöcke gebildet, in denen
 * jeder mögliche Bytewert (von 0x00 bis 0xff) genau einmal vorkommen. Begonnen wird mit der
 * Einfügung der Endemarke an der Indexposition 0. Block 1 beginnt mit der Endemarke 0xc4,
 * Block 2 mit der Endemarke 0x0a, Block 3 mit der Endemarke 0xed, Block 4 würde mit der
 * Endemarke 0x09 beginnen. usw. Nach 256 Blöcken würde das Spiel wieder von vorne beginnen.
 * So viele Blöcke werden aber in der Regel nicht gebildet.
 *
 * Vor der Endemarke werden nun so lange Datenbytes aus der Vernam-Chiffre in den Block eingefügt,
 * bis es zu einer Bytewiederholung kommen würde. Maximal werden jedoch nur 128 Datenbytes
 * eingefügt (wenn es zu keiner Bytewiederholung gekommen ist). Die Anzahl der Datenbytes aus
 * der Vernam-Chiffre kann also von Block zu Block stark schwanken. Hat das erste Datenbyte,
 * dass aus der Vernam-Chiffre eingefügt werden soll, zufällig denselben Bytewert wie die
 * Endemarke selbst, so wird nur die Endemarke eingefügt.
 *
 * Der Rest der 256 Byte große Datenblöcke wird mit Zufallszahlen (Zufallsbytes) aufgefüllt,
 * die noch nicht im Block enthalten waren. So werden Redundanzbytes erzeugt.
 *
 * In diesem Beispiel ergeben sich so drei 256 Byte große Blöcke, deren Datendump nachfolgend gezeigt wird:
 *
 * <pre class="fragment"> Datenblock1:
 *       Index  Inhalt (hexadezimal)                             Inhalt (ASCII)
 *           0  <span style="color:blue">5c 97 39 f5 49 e7</span> <span style="color:red">c4</span> <span style="color:green">0c 64 4b c6 cd 2a c2 b3 b7</span>  <span style="color:blue">\.9.I.</span><span style="color:red">.</span><span style="color:green">.dK..*...</span>
 *          16  <span style="color:green">23 21 ec ed 05 bc be 2d 8e bd 2c 47 5d 0f 20 dd  #!.....-..,G]. .</span>
 *          32  <span style="color:green">00 fe 85 02 1c 98 b0 ae 80 93 d3 96 a7 9e 09 7e  ...............~</span>
 *          48  <span style="color:green">9b 18 fb 75 68 c5 a4 3c 6b 28 7d 4a 48 f7 f0 c7  ...uh..<k(}JH...</span>
 *          64  <span style="color:green">ad 01 df 71 88 92 0b da 91 69 1f cc 43 8b b6 5b  ...q.....i..C..[</span>
 *          80  <span style="color:green">56 c1 27 e9 0d 3b 9c 15 b5 c9 7a 54 d1 c8 b2 35  V.'..;....zT...5</span>
 *          96  <span style="color:green">e5 f3 b4 46 40 bb 76 db ac 16 73 eb 82 03 a0 4e  ...F@.v...s....N</span>
 *         112  <span style="color:green">bf 55 07 d7 70 f6 fd f1 d8 b8 31 11 2e ef 4d a9  .U..p.....1...M.</span>
 *         128  <span style="color:green">e6 5f 4c e0 72 7c 9a ea ce 66 50 3d 65 b9 d6 25  ._L.r|...fP=e..%</span>
 *         144  <span style="color:green">cf dc 42 44 51 61 6f fa 22 89 6d 17 9d 24 a6 f2  ..BDQao...m..$..</span>
 *         160  <span style="color:green">ee 6c e1 3a f4 e4 12 1e 45 de 8c 94 60 ab 3e 08  .l.:....E.....>.</span>
 *         176  <span style="color:green">f8 30 19 5e 8a 6a 67 06 1a fc 62 af 74 0e 10 77  .0.^.jg...b.t..w</span>
 *         192  <span style="color:green">36 ca 41 a3 32 33 29 aa 63 c3 0a 58 d2 95 c0 84  6.A.23).c..X....</span>
 *         208  <span style="color:green">8f d5 83 d4 86 1d 04 ba 13 6e 1b 9f 5a 34 2b ff  .........n..Z4+.</span>
 *         224  <span style="color:green">78 d0 8d 7b 14 e3 a2 87 d9 a8 38 53 26 52 59 37  x..{......8S&RY7</span>
 *         240  <span style="color:green">99 a5 e2 e8 3f 81 79 4f 2f b1 cb 57 a1 7f 90 f9  ....?.yO/..W....</span></pre>
 *
 * <pre class="fragment"> Datenblock2:
 *       Index  Inhalt (hexadezimal)                             Inhalt (ASCII)
 *           0  <span style="color:blue">49 15 8a e1 df fa 68 90 c6 60 82 ec 19 be</span> <span style="color:red">0a</span> <span style="color:green">b2</span>  <span style="color:blue">I.....h.......</span><span style="color:red">.</span><span style="color:green">.</span>
 *          16  <span style="color:green">25 80 9d d9 4a 75 85 41 11 6d 1b 22 43 2e 21 8c  %...Ju.A.m..C.!.</span>
 *          32  <span style="color:green">b4 f8 5c 0d 0f a5 d4 af a0 f2 5e 9f 83 3a dd 06  ..\.......^..:..</span>
 *          48  <span style="color:green">d5 79 dc c3 b9 1e 5d 47 37 cf 99 a7 e9 a3 30 c4  .y....]G7.....0.</span>
 *          64  <span style="color:green">3d 8f d8 7b 6e 67 d2 0c 07 84 72 de f7 f9 04 7d  =..{ng....r....}</span>
 *          80  <span style="color:green">d0 e3 cb f1 58 4b 36 92 0e 34 31 89 48 1f 81 10  ....XK6..41.H...</span>
 *          96  <span style="color:green">b3 66 39 8d 14 f3 51 a6 65 db 61 7f c9 b0 6b b5  .f9...Q.e.a...k.</span>
 *         112  <span style="color:green">ff 7c 50 e8 78 13 57 87 2f bd 9b 6a bb e7 ef b1  .|P.x.W./..j....</span>
 *         128  <span style="color:green">4e 02 f6 3e 69 3f 5b 29 45 5f 17 da 03 94 ca 1c  N..>i?[)E_......</span>
 *         144  <span style="color:green">ea e4 55 ae 18 42 2b 2c 28 4d fd 46 f5 ba 16 c2  ..U..B+,(M.F....</span>
 *         160  <span style="color:green">33 ac 54 ce 86 95 05 35 44 26 cc 63 08 2d 8e b8  3.T....5D&.c.-..</span>
 *         176  <span style="color:green">59 c7 62 32 e6 3c 53 71 96 00 bc 4f ab 52 91 cd  Y.b2.<Sq...O.R..</span>
 *         192  <span style="color:green">d1 38 e2 ad a8 d6 fc 12 74 a2 97 73 aa 77 5a 7e  .8......t..s.wZ~</span>
 *         208  <span style="color:green">fb b6 70 9a 01 64 24 d7 98 ee 9c f4 6f 1a c1 b7  ..p..d$.....o...</span>
 *         224  <span style="color:green">a9 c0 93 c5 1d 4c 0b 9e 8b eb ed e0 76 23 09 20  .....L......v#. </span>
 *         240  <span style="color:green">27 88 2a c8 e5 a4 6c f0 56 bf a1 d3 3b 7a fe 40  '.*...l.V...;z..</span></pre>
 *
 * <pre class="fragment"> Datenblock3:
 *       Index  Inhalt (hexadezimal)                             Inhalt (ASCII)
 *           0  <span style="color:blue">15 62 fb 60</span> <span style="color:red">ed</span> <span style="color:green">af 50 f6 2f 5c b0 f4 f2 5d ea bb</span>  <span style="color:blue">.b..</span><span style="color:red">.</span><span style="color:green">.P./\...]..</span>
 *          16  <span style="color:green">26 5a ec f9 34 19 7c b4 c4 d2 9d 08 f0 f3 cc 3b  &Z..4.|........;</span>
 *          32  <span style="color:green">3c 5e 24 7a 2b 21 a6 b8 b9 a1 0f 8d 12 ad c0 53  <^$z+!.........S</span>
 *          48  <span style="color:green">6f 9f ab 4f 1f 11 64 10 db d6 ba 97 dc 4e 9c aa  o..O..d......N..</span>
 *          64  <span style="color:green">ee 91 33 25 e9 68 85 2c fe a0 ae 8a 61 58 28 de  ..3%.h.,....aX(.</span>
 *          80  <span style="color:green">b3 c5 f5 dd 6d 3d 14 90 99 d0 1d 32 01 79 ca 0b  ....m=.....2.y..</span>
 *          96  <span style="color:green">1a 9e bf 69 a7 c3 fd c2 22 89 43 95 df 29 5f 1b  ...i......C..)_.</span>
 *         112  <span style="color:green">63 71 bd 80 38 c9 f1 e0 00 d5 2d 17 78 cd 1e 4a  cq..8.....-.x..J</span>
 *         128  <span style="color:green">73 84 fc 87 e2 1c 35 a2 ac 94 04 54 4c 65 74 cf  s.....5....TLet.</span>
 *         144  <span style="color:green">07 fa a4 30 e5 0d 46 52 b2 75 c8 40 6a 44 ef 88  ...0..FR.u..jD..</span>
 *         160  <span style="color:green">e4 77 7e 3a 86 06 4d 45 8f 8c 47 e8 18 c7 0c a9  .w~:..ME..G.....</span>
 *         176  <span style="color:green">6c 3e cb e3 02 b7 81 09 66 d4 16 56 4b eb 7d 05  l>......f..VK.}.</span>
 *         192  <span style="color:green">b1 36 41 72 a8 5b 76 93 9b 59 b5 a3 39 bc 57 3f  .6Ar.[v..Y..9.W?</span>
 *         208  <span style="color:green">96 e1 9a 03 e6 37 6b ff f7 6e f8 49 d7 d9 92 8b  .....7k..n.I....</span>
 *         224  <span style="color:green">0e da 27 31 d3 0a 7b ce 51 a5 2a 67 13 20 be c6  ..'1..{.Q.*g. ..</span>
 *         240  <span style="color:green">c1 55 42 48 b6 d1 e7 7f 82 23 2e 70 83 98 8e d8  .UBH.....#.p....</span>
 *      <span style="color:green">Redundanzbytes grün</span>, <span style="color:blue">Vernam-Chiffre blau</span>, <span style="color:red">Endemarken rot</span></pre>
 *
 * Am Anfang der Datenblöcke stehen die Datenbytes aus der Vernam-Chiffre (<span style="color:blue">hier blau dargestellt</span>),
 * dann folgt die Endemarke (<span style="color:red">hier rot</span>) und anschließend die Redundanzbytes
 * (<span style="color:green">grün dargestellt</span>). Die Reihenfolge der Redundanzbytes ändert sich
 * selbstverständlich bei jedem weiteren Darstellungsversuch.
 *
 * <br><br>
 * Vierter Verschlüsselungsschritt
 * -------------------------------
 *
 * Bei der Einrichtung des SecureStore wurde vom Zufallszahlengenerater ein 256 Byte großer Block mit Vertauschungsbytes
 * erzeugt. Alle 256 Vertauschungsbytes sind verschieden. Dies bedeutet, dass jeder möglich Zahlenwert in diesem
 * Vertauschungsblock genau einmal vorkommt. Zur Beschreibung dieses Verarbeitungsschrittes in diesem Kapitel wurde
 * vom Zufallszahlengenerator folgender 256 Byte langer Vertauschungsblock generiert:
 *
 *      256 Byte langer Vertauschungsschlüssel:
 *       Index  Inhalt (hexadezimal)                             Inhalt (ASCII)
 *           0  56 9f 6b ba da 4f 4c d0 89 e7 7b 10 5e f5 1c 46  V.k..OL...{.^..F
 *          16  b2 0b cb b8 3f e0 6d 4e 5d fa c8 11 eb ca d5 a4  ....?.mN].......
 *          32  a8 5c 02 1e 62 d4 49 bf f9 dc 79 94 91 31 66 a5  .\..b.I...y..1f.
 *          48  6e a9 ad 34 15 a6 f0 ff 44 0a 14 26 80 71 0e 72  n..4....D..&.q.r
 *          64  b6 c7 65 3c 4d bd f8 b9 b5 41 18 2f 69 8d f1 36  ..e<M....A./i..6
 *          80  f4 00 ac 21 e6 55 5f af 09 4b 87 a3 df 6a 3e d9  ...!.U_..K...j>.
 *          96  28 47 60 cd d1 98 dd 8b 1a 61 2c 9e 5b 86 2d fe  (G.......a,.[.-.
 *         112  b3 53 78 b4 81 a2 b1 6c 48 9a 8a 3b 88 25 07 8e  .Sx....lH..;.%..
 *         128  68 e1 6f 7c 33 ee 1d a1 03 db 77 5a cf 52 08 82  h.o|3.....wZ.R..
 *         144  fd 12 2a cc 3d ea e9 2e e2 aa 24 bc 43 3a 0f 01  ..*.=.....$.C:..
 *         160  d8 ce 95 e5 29 7a 90 05 d2 63 97 de 30 4a 20 58  ....)z...c..0J X
 *         176  19 76 42 b7 e8 9c 74 8c f3 7d a0 99 e3 13 c2 c1  .vB...t..}......
 *         192  59 93 9b 84 c3 ec 64 d7 f7 f2 fb d3 ef 9d a7 f6  Y.....d.........
 *         208  c9 70 22 fc 32 40 c4 45 17 75 ae 2b bb 96 38 50  .p".2@.E.u.+..8P
 *         224  27 be 92 ab c6 04 1f 35 e4 c5 51 0d 0c 7f 85 d6  '......5..Q.....
 *         240  73 23 83 7e 8f 67 54 b0 ed 06 37 c0 16 57 39 1b  s#.~.gT...7..W9.
 *
 * In diesem Verschlüsselungsschritt werden mehrere 256 Byte große Datenblöcke gebildet, in denen jeder mögliche
 * Bytewert (von 0x00 bis 0xff) genau einmal vorkommen. Im neuen Block1 wird an Index 0x00 nun das Byte aus Block1
 * des vorangegangenen Kapitels geschrieben, dass dort am Index 0x56 steht. An Index 0x01 wird das Byte aus Index
 * 0x9f geschrieben usw.
 *
 * Vor der Bildung des neuen zweiten Blockes wird der Vertauschungsschlüssel um 1 Byte rotiert. An Index 0x00 des
 * zweiten Blockes wird das Byte aus Block1 des vorangegangenen Kapitels geschrieben, dass dort am Index 0x9f steht.
 * An Index 0x01 wird das Byte aus Index 0x6b geschrieben usw.
 *
 * In diesem Beispiel ergeben sich so drei 256 Byte große Blöcke, deren Datendump nachfolgend gezeigt wird.
 *
 * <pre class="fragment"> Ausgabeatenblock:
 *      Index  Inhalt (hexadezimal)                             Inhalt (ASCII)
 *          0  9c f2 eb 62 1b 5b 43 8f 66 87 11 23 b2 81 5d 0b  ...b.[C.f..#..].
 *         16  19 cd 58 1a c7 78 03 b6 c8 cb 63 21 53 0a 1d f4  ..X..x....c!S...
 *         32  45 d1 <span style="color:blue">39</span> 20 b4 86 69 77 b1 5a b8 51 dc 18 76 e4  E.9 ..iw.Z.Q..v.
 *         48  a0 de ab 68 bc 12 99 f9 88 c6 05 b0 e6 55 b3 07  ...h.........U..
 *         64  67 aa bb 48 8b 0e 2f fc 6a 01 8e 7e 16 b9 a5 a4  g..H../.j..~....
 *         80  3f <span style="color:blue">5c</span> 60 fe a2 3b 35 08 4b cc ea 3a ff 73 f0 6e  ?\`..;5.K..:.s.n
 *         96  80 da e5 95 d5 22 34 3d 2c f3 a7 a6 54 9a 9e 90  ......4=,...T...
 *        112  5e e9 d8 8a 5f e1 30 82 91 6d 50 4a ce 98 0c d6  ^..._.0..mPJ....
 *        128  ac d0 4e 2e 75 59 0f 6c <span style="color:blue">f5</span> 9f f1 7a 84 27 64 4c  ..N.uY.l...z.'dL
 *        144  7f ec d3 d2 f7 38 a8 09 8d 8c 1c 74 71 7d b7 <span style="color:blue">97</span>  .....8.....tq}..
 *        160  13 c0 61 e3 93 31 cf <span style="color:blue">e7</span> 83 46 fa 2b 9b 1f 00 b5  ..a..1...F.+....
 *        176  bd fd df 06 d9 9d 70 65 e8 ef ee 89 7b ed 41 ca  ......pe....{.A.
 *        192  c9 44 17 72 a3 26 40 ba 4f e2 57 d4 37 24 1e 79  .D.r.&..O.W.7$.y
 *        208  c3 bf 85 a1 fb ad 32 92 2d f6 3e 96 af 6f 6b 56  ......2.-.>..okV
 *        224  ae 10 42 94 29 <span style="color:blue">49</span> dd c5 14 33 c1 c2 2a a9 7c 04  ..B.)I...3..*.|.
 *        240  d7 02 e0 4d 25 db 0d f8 52 <span style="color:red">c4</span> 3c 36 be 15 28 47  ...M%...R.<6..(G
 *
 *        256  c2 7f bc 9c 7d f7 fb 5f 9e 6a 25 81 a4 43 d2 62  ....}.._.j%..C.b
 *        272  <span style="color:blue">ec</span> 73 96 c4 a9 b0 04 1f a1 74 80 e0 97 64 86 44  .s.......t...d.D
 *        288  48 <span style="color:blue">8a</span> 21 39 01 84 cd bf 6f bd 18 e4 79 51 95 6b  H.!9....o...yQ.k
 *        304  26 2d b9 75 05 27 40 6e <span style="color:blue">82</span> 4a d4 4e 7c <span style="color:red">0a</span> 50 53  &-.u.'.n.J.N|.PS
 *        320  12 f3 e9 f9 52 56 00 3c 8f 11 06 db 94 88 5d e5  ....RV.<......].
 *        336  <span style="color:blue">49</span> 08 f8 0b 4b 10 b8 <span style="color:blue">60</span> de 29 ce b7 61 30 ee a0  I...K..`.)..a0..
 *        352  0c b3 77 b6 28 1a da 1b 66 83 16 89 5b 3a fe 32  ..w.(...f...[:.2
 *        368  f1 2f e6 02 54 c7 c9 07 fd 17 a7 45 a5 <span style="color:blue">90</span> ca 65  ./..T......E...e
 *        384  c0 b5 bb c3 09 2e ac <span style="color:blue">e1</span> f4 87 31 7e cb <span style="color:blue">c6</span> f6 7a  ..........1~...z
 *        400  9d 5e aa a3 ed eb dd 93 cc 0f ab 7b 99 b2 <span style="color:blue">15</span> 98  .^.........{....
 *        416  5a 42 4c f2 9b ea <span style="color:blue">fa</span> 70 8d 2c c1 d5 72 b4 0e 6d  ZBL....p.,..r..m
 *        432  57 d8 71 8b f5 78 03 c8 e7 33 4d c5 d9 e2 38 34  W.q..x...3M...84
 *        448  ae 46 69 ad 76 14 d7 f0 2a d3 9a 20 ba 35 6c a2  .Fi.v...*.. .5l.
 *        464  ff 5c 3b dc 3d a8 67 41 13 8e 9f 4f 2b 37 d0 af  .\;.=.gA...O+7..
 *        480  91 55 63 fc <span style="color:blue">df</span> 8c 1e 1d d6 e3 <span style="color:blue">be 19</span> b1 3f 24 e8  .Uc..........?$.
 *        496  0d 3e ef 1c a6 58 59 23 <span style="color:blue">68</span> 47 d1 85 92 cf 22 36  .>...XY#hG.....6
 *
 *        512  95 16 f8 de 61 96 94 ce 17 26 ca d1 f0 85 cb f4  ....a....&......
 *        528  a3 66 aa 0e 29 28 79 2e 9b 5a 67 b5 37 86 8f 01  .f..)(y..Zg.7...
 *        544  <span style="color:blue">fb</span> cc bf e6 a0 05 23 d7 d5 e5 fa 9f fd 06 5f 8c  ......#......._.
 *        560  c7 1f 19 4d c1 d8 e9 b0 34 a6 73 71 ea bd 81 93  ...M....4.sq....
 *        576  c3 dc 58 eb 82 d4 b7 91 c4 53 89 65 55 64 b6 <span style="color:blue">15</span>  ..X......S.eUd..
 *        592  18 5e 7b 3d 0b a9 5c 8a a2 3a 8b 43 9c 6e b9 2c  .^{=..\..:.C.n.,
 *        608  1a bc e1 b2 d9 54 9d 9e 12 ef 32 35 ad 8e e3 dd  .....T....25....
 *        624  00 02 84 7e 3e df fe c8 04 97 ac 21 f6 74 22 da  ...~>......!.t..
 *        640  1b 78 4f be f3 77 <span style="color:blue">60</span> 49 e0 1d 3f f5 2f fc 98 ec  .xO..w`I..?./...
 *        656  0f 39 4e 2a a5 c0 27 47 2b 4b 25 ba bb <span style="color:blue">62</span> f7 57  .9N*..'G+K%..b.W
 *        672  0d 0a a1 2d 07 af 9a 69 52 92 6f ae 3c 99 d2 f1  ...-...iR.o.<...
 *        688  33 09 51 6a 38 4c 48 cd e4 75 31 f9 41 36 d0 30  3.Qj8LH..u1.A6.0
 *        704  40 e2 72 13 a7 ff 7f 42 70 03 c6 44 45 e7 59 63  ..r....Bp..DE.Yc
 *        720  24 83 ab ee a8 68 b4 c9 0c 8d 56 46 db b3 b8 7d  $....h....VF...}
 *        736  a4 e8 76 <span style="color:red">ed</span> 3b 11 d3 5b c5 5d f2 4a 1c 6b 80 7a  ..v.;..[.].J.k.z
 *        752  87 1e cf c2 6d 6c 20 50 10 b1 7c 90 d6 08 14 88  ....ml P..|.....
 *      <span style="color:blue">Vernam-Chiffre blau</span>, <span style="color:red">Endemarken rot</span></pre>
 *
 * Die Datenbytes aus der Vernam-Chiffre (<span style="color:blue">hier blau</span>) und die Endemarke
 * (<span style="color:red">hier rot</span>) befinden sich nun nicht mehr am Anfang des Datenblockes.
 * Ohne Kenntnis der verschiedenen Schlüssel kann ein Angreifer nicht mehr auf die Klartextdaten schließen.
 */

