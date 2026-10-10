# Programm SecureDB

Dieses Windows-Programm SecureDB (Secure-Datenbank) dient zur verschlüsselten Speicherung
von kurzen Texten bis zu ca. 4000 Zeichen Länge. Insbesondere Zugangsdaten zu
verschiedenen Accounts können damit verwaltet (und unter verschiedenen Namen
abgelegt) werden. Ein typischer Eintrag könnte zum Beispiel wie folgt aussehen:

    Amazon
     Zugang: www.amazon.de
     User:   vorname.nachname@t-online.de
     PW:     itzelbritzel
     Datum:  22.01.2012
     
Der dargestellte Text ist nur 119 Zeichen lang. Er beschreibt in diesem Beispiel
einen Zugang zu einem Amazon Konto mit der zugehörigen Internetadresse, dem
Usernamen, dem zum Usernamen gehörendem Passwort und einem Datum der letzten
Änderung. Aber auch PINs, PUKs und TANs oder andere sicherheitskritische Informationen
können mit diesem Programm verwaltet werden.

This Windows program, SecureDB (Secure Database), is used for the encrypted storage
of short text strings up to approximately 4,000 characters in length. In particular, login credentials for
various accounts can be managed (and stored under different names
) using it. A typical entry might look like this, for example:

    Amazon
     Login:    www.amazon.de
     User:     givenname.surname@t-online.de
     Password: itzelbritzel
     Date:     January 22, 2012
     
The text shown is only 119 characters long. In this example, it describes
access to an Amazon account with the corresponding web address, the
username, the password associated with the username, and the date of the last
change. However, PINs, PUKs, TANs, and other security-critical information
can also be managed with this program.


## Installation

Kopieren sie alle Dateien und Verzeichnisse aus dem Unterverzeichnis **Install** in ein 
Installationsverzeichnis ihrer Wahl. Legen sie eine Verknüpfung an, die auf die 
Datei **SecureDB.exe** in ihrem Installationsverzeichnis verweist. Nun sollte
alles funktioniern.

Dieses Programm benötigt unter anderem die Programmbibliothek OpenSSL in der
Version 3.x. Sollte eine Version 3.x der Programmbibliothek OpenSSL bereits auf
ihrem Computer installiert sein, so kann beim Kopieren die Datei libcrypto-3-x64.dll
weggelassen werden.

Copy all files and directories from the **Install** subdirectory to an
installation directory of your choice. Create a shortcut that points to the
**SecureDB.exe** file in your installation directory. Now
everything should work.

Among other things, this program requires the OpenSSL library in
version 3.x. If a version 3.x of the OpenSSL library is already
installed on your computer, you can omit the file libcrypto-3-x64.dll
when copying.


## Dokumentation

Die Dokumentation dieses Programms, die Bedienung dieses Programms sowie die Funktionsweise
der generellen Verschlüsselung und der einzelnen Verschlüsselungsschritte sind in den
einzelnen Source-Dateien hinterlegt. Mit dem Programm Doxygen (https://www.doxygen.nl)
und der Datei Doxyfile.txt aus diesem Projekt kann eine komplette Dokumentation
erstellt werden.

The documentation for this program, instructions for using it, and details on how
encryption works in general and the individual encryption steps are stored in the
individual source files. Using the Doxygen program (https://www.doxygen.nl)
and the Doxyfile.txt file from this project, you can generate a complete set of documentation.