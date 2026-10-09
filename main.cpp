#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

/*! \file Main.cpp
 *  \brief In dieser Datei liegt der Einstiegspunkt vom Betriebssystem in dieses Programm: `main()`.
 *
 *  Es wird eine Instanz der __QApplication-Klasse__ erzeugt, der die Komandozeilen-Parameter aus `main()` übergeben werden.
 *  Danach wird das Hauptfenster der Anwendung erzeugt (eine Instanz der Klasse __MainWindow__) und dieses zur Anzeige und zum Ablauf gebracht.
 *  Die Ereignisschleife wird nun solange ausgeführt, bis sie mit einem Quit-Ereignis (und damit dieses Programm) beendet wird.
 */

/*! \brief Einstiegspunkt vom Betriebssystem in dieses Programm: `main()`.
 *
 *  Hier wird die Sprachauswahl realisiert. Danach wird das Hauptfenster angezeigt.
 */
int main(int argc, char *argv[]) {
        QApplication a(argc, argv);
        QSettings *s = new QSettings("RH", "SecureDB");
        int lang = s->value("Language").toInt();
        if ((lang < 1) | (lang > 2)) {
                s->setValue("Language", 2);
        }
        delete s;
        QTranslator translator;
        bool ok = false;
        if (lang == 1) {
                ok = translator.load(":/i18n/SecueDB_en_US");
        }
        if (lang == 2) {
                ok = translator.load(":/i18n/SecureDB_de_DE");
        }
        if (ok){
                a.installTranslator(&translator);
        }
        SecureDB::MainWindow w;
        w.show();
        return a.exec();
}
