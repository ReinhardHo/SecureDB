/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file mainwindow.h
 *  \brief Definiert die Klasse __MainWindow__.
 *
 *  Die Klasse MainWindow ist eine Ableitung der Qt-Klasse QMainWindow. Sie definiert alle graphischen Elemente des Hauptfensters und stellt alle Methoden bereit,
 *  die der Benutzer durch die Auswahl von Menüpunkten oder Buttons auslösen kann.
 */
#ifndef SECUREDB_MAINWINDOW_H
#define SECUREDB_MAINWINDOW_H
#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QToolBar>
#include <QLineEdit>
#include <QProgressBar>
#include <QSettings>
#include "secure.h"
#include "securefile.h"
#include "Itemtable.h"
//#include "test.h"

namespace SecureDB {


        /*! \brief Die Klasse __MainWindow__ ist eine Ableitung von der Qt-Klasse __QMainWindow__. Dies ist das Hauptfenster der Anwendung __SecureDB__.
         *
         *  Die Klasse MainWindow definiert alle graphischen Bestandteile der Hauptfensters (Menüs, Toolbar, Statusbar, usw.).
         *
         *  Außerdem sind in dieser Klasse alle Methoden realisiert, die der Benutzer durch die Bedienung des Programms auslösen kann.
         */
        class MainWindow : public QMainWindow {
                Q_OBJECT
        public:
                MainWindow(QWidget *parent = nullptr);
                ~MainWindow();
        signals:
        private slots:
                void DeleteButton_Click();
                void ExitButton_Click();
                void languages_Clicked();
                void NewStore_Click();
                void NextButton_Click();
                void OpenButton_Click();
                void PriorButton_Click();
                void RedWhite_Click();
                void SaveAs_Click();
                void SaveButton_Click();
                void SetBackgroundColor();
                void StoreMerge_Click();
                void StoreMerge_Trash_Click();
                void StoreSelect_Click(bool newFile = false);

        private:
                void ClearSecureItems();
                void ClearMergeItems();
                void DeleteKeyGenerationData1();
                void DeleteKeyGenerationData2();
                int  FindByteDown(int index, unsigned char c);
                int  FindByteUp(int index, unsigned char c);
                bool IsByteInArray(ByteArray &ref, char c, size_t count);
                void LoadItemTable();
                bool SaveItemTable();
                void SecureItem(QString &ref);
                void SetupMenuBar();
                void SetupMergeItems();
                void SetupSecureItems();
                void SetupStatusBar();
                void SetupToolBar();
                bool StartSecureMerge();
                bool StartSecureMode();
                bool UnsecureItem(size_t storeIndex);
                /*!
                 * \brief Aktion Delete
                 *
                 * Diese Aktion steht nur im Securemodus zur Verfügung.
                 *
                 * Mit dieser Aktion wird das gerade ausgewählte Element im Secure-Store in den Mülleiner verschoben.
                 * War das Element schon im Mülleimer, so wird es entgültig gelöscht.
                 */
                QAction *actDelete;
                /*!
                 * \brief Aktion Kopiere aus einem MergeStore in den aktuellen Secure-Store
                 *
                 * Mit dieser Aktion kann im Securemodus ein weiterer SecureStore geöffnet werden.
                 * Alle Elemente aus diesem (Merge)-Store, die im aktuellen Store noch nicht vorhanden sind, werden in den aktuellen Store kopiert.
                 * \sa actMergeStoreTrash
                 */
                QAction *actMergeStore;
                /*!
                 * \brief Aktion Kopiere aus dem aktuellen Secure-Store gelöschte Elemente in einen anderen (Merge)-Store
                 *
                 * Mit dieser Aktion kann im Securemodus ein weiterer SecureStore geöffnet werden.
                 * Alle gelöschten Elemente aus dem aktuellen Store, die im (Merge)-Store noch nicht vorhanden sind, werden in den (Merge)-Store kopiert.
                 * \sa actMergeStore
                 */
                QAction *actMergeStoreTrash;
                /*!
                 * \brief Aktion Next
                 *
                 * Diese Aktion steht nur im Securemodus zur Verfügung.
                 *
                 * Mit dieser Aktion wird das nächst folgende Element im Secure-Store ausgewählt.
                 * \sa actPrior
                 */
                QAction *actNext;
                /*!
                 * \brief Aktion Open
                 *
                 * Mit dieser Aktion wird im Normalmodus eine Datei geöffnet und in den Editor geladen.
                 * Im Securemodus wird ein Dialog angezeigt, in dem ein Element aus dem Secure-Store ausgewählt und in den
                 * Editor geladen werden kann.
                 */
                QAction *actOpen;
                /*!
                 * \brief Aktion Prior
                 *
                 * Diese Aktion steht nur im Securemodus zur Verfügung.
                 *
                 * Mit dieser Aktion wird das vorherige Element im Secure-Store ausgewählt.
                 * \sa actNext
                 */
                QAction *actPrior;
                /*!
                 * \brief Aktion Quit
                 *
                 * Mit dieser Aktion wird das Programm beendet.
                 */
                QAction *actQuit;
                /*!
                 * \brief Aktion Save
                 *
                 * Mit dieser Aktion wird im Normalmodus eine zuvor geöffnete Datei wieder gespeichert.
                 * Im Securemodus wird ein geladenes (und ggf. bearbeitete) Element mit demselben Namen und aktualisierten Datum
                 * verschlüsselt im Secure-Store gespeichert.
                 * \sa actSaveAs
                 */
                QAction *actSave;
                /*!
                 * \brief Aktion SaveAS
                 *
                 * Mit dieser Aktion wird im Normalmodus eine zuvor geöffnete Datei wieder (unter einem anderen Namen) gespeichert.
                 * Im Securemodus wird ein geladenes (und ggf. bearbeitete) Element unter einem anderen Namen und aktualisierten Datum
                 * verschlüsselt im Secure-Store gespeichert.
                 * \sa actSave
                 */
                QAction *actSaveAs;
                /*!
                 * \brief Aktion SelectStore
                 *
                 * Mit dieser Aktion wird im Normalmodus ein SecureStore ausgewählt.
                 * Im Securemodus steht diese Aktion nicht zur Verfügung.
                 */
                QAction *actSelectStore;
                /*! \brief Zeiger auf eine Instanz der Klasse QTextEdit
                 *
                 * Mit diesem Editor werden alle Textbearbeitungen vorgenommen.
                 */
                QTextEdit *editor;
                /*!
                 * \brief Zeiger auf eine ByteArray-Instanz mit Ende-Etiketten
                 *
                 * Zeiger auf eine ByteArray-Instanz, die einen 256-Byte-Schlüssel enthält,
                 * bei dem ein Byte dazu dient, das Ende der Daten
                 * in einem Datenfeld zu markieren, das zudem Redundanzbytes enthält.
                 * Alle Byte-Werte in diesem Schlüssel sind unterschiedlich und kommen genau einmal vor.
                 * \sa EndLabelKeyM
                 */
                ByteArray *EndLabelKey;
                /*!
                 * \brief Zeiger auf eine ByteArray-Instanz mit Ende-Etiketten (im Merge-Store)
                 *
                 * Zeiger auf eine ByteArray-Instanz, die einen 256-Byte-Schlüssel enthält,
                 * bei dem ein Byte dazu dient, das Ende der Daten
                 * in einem Datenfeld zu markieren, das zudem Redundanzbytes enthält.
                 * Alle Byte-Werte in diesem Schlüssel sind unterschiedlich und kommen genau einmal vor.
                 * \sa EndLabelKey
                 */
                ByteArray *EndLabelKeyM;
                /*! \brief Die Klassse __ItemTable__ verwaltet die verschlüsselten Datenbereiche im SecureStore.
                 *
                 * Eine Instanz dieser Klasse __ItemTable__ verwaltet die gespeicherten Elemente im Secure-Store und hält diese sortiert nach Name und Schreibzeitpunkt (Schlüssel 0) vor.
                 */
                ItemTable *ItemTable;
                /*! \brief Zeiger auf eine ByteArray-Instanz, mit deren Hilfe alle benötigten Schlüssel generiert werden können.
                 *
                 *  In dieser ByteArray-Instanz wrd zunächst ein 64 Byte langer (SHA512) Hashwert gespeichert, der von den ein Megabyte langen Zufallszahlen generiert wurde.
                 *  Danach folgt ein 48 Byte langer (SHA384) Hashwert, der von den ein Megabyte langen Zufallszahlen generiert wurde. Nun folgt für jedes benötogte Schlüsselbyte
                 *  ein Integerwert, mit dem dieses Schlüsselbyte in den Zufallsdaten gefunden und zurück gewonnen werden kann.
                 */
                ByteArray *KeyGenerationArray;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die erste Ver- oder Entschlüsselung des ersten SecureFile (mit der Endung *.rnd für einen Merge-Vorgang).
                 *  Der erste Initialisierungsvektor für die erste AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __ersten__ eingegebenen
                 *  Passwort, der __ersten__ Hashrunden-Anzahl und einem ersten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1IV2, Merge2IV1, Merge2IV2, Merge3IV1, Merge3IV2, SecFile1IV1, SecFile1IV2, SecFile2IV1, SecFile2IV2, SecFile3IV1 und SecFile3IV2
                 */
                ByteArray *Merge1IV1;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die zweite Ver- oder Entschlüsselung des ersten SecureFile (mit der Endung *.rnd für einen Merge-Vorgang).
                 *  Der zweite Initialisierungsvektor für die zweite AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __ersten__ eingegebenen
                 *  Passwort, der __ersten__ Hashrunden-Anzahl und einem zweiten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1IV1, Merge2IV1, Merge2IV2, Merge3IV1, Merge3IV2, SecFile1IV1, SecFile1IV2, SecFile2IV1, SecFile2IV2, SecFile3IV1 und SecFile3IV2
                 */
                ByteArray *Merge1IV2;
                /*! \brief Referenz auf ein ByteArray, das einen 32 Byte langen Schlüssel zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die erste Ver- oder Entschlüsselung des ersten SecureFile (mit der Endung *.rnd für einen Merge-Vorgang).
                 *  Der erste Schlüssel für die erste AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __ersten__ eingegebenen
                 *  Passwort, der __ersten__ Hashrunden-Anzahl und einem ersten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1Key2, Merge2Key1, Merge2Key2, Merge3Key1, Merge3Key2, SecFile1Key1, SecFile1Key2, SecFile2Key1, SecFile2Key2, SecFile3Key1 und SecFile3Key2
                 */
                ByteArray *Merge1Key1;
                /*! \brief Referenz auf ein ByteArray, das einen 32 Byte langen Schlüssel zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die zweite Ver- oder Entschlüsselung des ersten SecureFile (mit der Endung *.rnd für einen Merge-Vorgang).
                 *  Der zweite Schlüssel für die zweite AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __ersten__ eingegebenen
                 *  Passwort, der __ersten__ Hashrunden-Anzahl und einem zweiten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1Key1, Merge2Key1, Merge2Key2, Merge3Key1, Merge3Key2, SecFile1Key1, SecFile1Key2, SecFile2Key1, SecFile2Key2, SecFile3Key1 und SecFile3Key2
                 */
                ByteArray *Merge1Key2;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die erste Ver- oder Entschlüsselung des zweiten SecureFile (mit der Endung *.key für einen Merge-Vorgang).
                 *  Der erste Initialisierungsvektor für die erste AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __zweiten__ eingegebenen
                 *  Passwort, der __zweiten__ Hashrunden-Anzahl und einem ersten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1IV1, Merge1IV2, Merge2IV2, Merge3IV1, Merge3IV2, SecFile1IV1, SecFile1IV2, SecFile2IV1, SecFile2IV2, SecFile3IV1 und SecFile3IV2
                 */
                ByteArray *Merge2IV1;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die zweite Ver- oder Entschlüsselung des zweiten SecureFile (mit der Endung *.key für einen Merge-Vorgang).
                 *  Der zweite Initialisierungsvektor für die zweite AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __zweiten__ eingegebenen
                 *  Passwort, der __zweiten__ Hashrunden-Anzahl und einem zweiten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1IV1, Merge1IV2, Merge2IV1, Merge3IV1, Merge3IV2, SecFile1IV1, SecFile1IV2, SecFile2IV1, SecFile2IV2, SecFile3IV1 und SecFile3IV2
                 */
                ByteArray *Merge2IV2;
                /*! \brief Referenz auf ein ByteArray, das einen 32 Byte langen Schlüssel zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die erste Ver- oder Entschlüsselung des zweiten SecureFile (mit der Endung *.key für einen Merge-Vorgang).
                 *  Der erste Schlüssel für die erste AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __zweiten__ eingegebenen
                 *  Passwort, der __zweiten__ Hashrunden-Anzahl und einem ersten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1Key1, Merge1Key2, Merge2Key2, Merge3Key1, Merge3Key2, SecFile1Key1, SecFile1Key2, SecFile2Key1, SecFile2Key2, SecFile3Key1 und SecFile3Key2
                 */
                ByteArray *Merge2Key1;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die zweite Ver- oder Entschlüsselung des zweiten SecureFile (mit der Endung *.key für einen Merge-Vorgang).
                 *  Der zweite Schlüssel für die zweite AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __zweiten__ eingegebenen
                 *  Passwort, der __zweiten__ Hashrunden-Anzahl und einem zweiten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1Key1, Merge1Key2, Merge2Key1, Merge3Key1, Merge3Key2, SecFile1Key1, SecFile1Key2, SecFile2Key1, SecFile2Key2, SecFile3Key1 und SecFile3Key2
                 */
                ByteArray *Merge2Key2;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die erste Ver- oder Entschlüsselung des dritten SecureFile (mit der Endung *.bin für einen Merge-Vorgang).
                 *  Der erste Initialisierungsvektor für die erste AES-Verschlüsselung dieser Datei wurde bei der Erstellung des Secure-Store vom Zufallszahlengenerator generiert.
                 *  Die Daten werden bei der Eröffnung des Secure-Store aus den Zufallsbytes aus der ersten Datei und den Integerwerten aus der zweiten Datei dann richtig zurück berechnet,
                 *  wenn der Startindex und der Differenzwert vom Benutzer korrekt eingegeben wurde.
                 *  \sa Merge1IV1, Merge1IV2, Merge2IV1, Merge2IV2, Merge3IV2, SecFile1IV1, SecFile1IV2, SecFile2IV1, SecFile2IV2, SecFile3IV1 und SecFile3IV2
                 */
                ByteArray *Merge3IV1;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die zweite Ver- oder Entschlüsselung des dritten SecureFile (mit der Endung *.bin für einen Merge-Vorgang).
                 *  Der zweite Initialisierungsvektor für die zweite AES-Verschlüsselung dieser Datei wurde bei der Erstellung des Secure-Store vom Zufallszahlengenerator generiert.
                 *  Die Daten werden bei der Eröffnung des Secure-Store aus den Zufallsbytes aus der ersten Datei und den Integerwerten aus der zweiten Datei dann richtig zurück berechnet,
                 *  wenn der Startindex und der Differenzwert vom Benutzer korrekt eingegeben wurde.
                 *  \sa Merge1IV1, Merge1IV2, Merge2IV1, Merge2IV2, Merge3IV1, SecFile1IV1, SecFile1IV2, SecFile2IV1, SecFile2IV2, SecFile3IV1 und SecFile3IV2
                 */
                ByteArray *Merge3IV2;
                /*! \brief Referenz auf ein ByteArray, das einen 32 Byte langen Schlüssel zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die erste Ver- oder Entschlüsselung des dritten SecureFile (mit der Endung *.bin für einen Merge-Vorgang).
                 *  Der erste Schlüssel für die erste AES-Verschlüsselung dieser Datei wurde bei der Erstellung des Secure-Store vom Zufallszahlengenerator generiert.
                 *  Die Daten werden bei der Eröffnung des Secure-Store aus den Zufallsbytes aus der ersten Datei und den Integerwerten aus der zweiten Datei dann richtig zurück berechnet,
                 *  wenn der Startindex und der Differenzwert vom Benutzer korrekt eingegeben wurde.
                 *  \sa Merge1Key1, Merge1Key2, Merge2Key1, Merge2Key2, Merge3Key2, SecFile1Key1, SecFile1Key2, SecFile2Key1, SecFile2Key2, SecFile3Key1 und SecFile3Key2
                 */
                ByteArray *Merge3Key1;
                /*! \brief Referenz auf ein ByteArray, das einen 32 Byte langen Schlüssel zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die zweite Ver- oder Entschlüsselung des dritten SecureFile (mit der Endung *.bin für einen Merge-Vorgang).
                 *  Der zweite Schlüssel für die zweite AES-Verschlüsselung dieser Datei wurde bei der Erstellung des Secure-Store vom Zufallszahlengenerator generiert.
                 *  Die Daten werden bei der Eröffnung des Secure-Store aus den Zufallsbytes aus der ersten Datei und den Integerwerten aus der zweiten Datei dann richtig zurück berechnet,
                 *  wenn der Startindex und der Differenzwert vom Benutzer korrekt eingegeben wurde.
                 *  \sa Merge1Key1, Merge1Key2, Merge2Key1, Merge2Key2, Merge3Key1, SecFile1Key1, SecFile1Key2, SecFile2Key1, SecFile2Key2, SecFile3Key1 und SecFile3Key2
                 */
                ByteArray *Merge3Key2;
                /*! \brief Zeiger auf eine Instanz der Klasse Store::SecureStore.
                 *
                 * Die Instanz wird nacheinander für alle drei Dateien eines Secure-Store benutzt. Details sind im Projekt HelperClasses unter der Klasse Store::SecureStore nachzulesen.
                 */
                Store::SecureFile *MergeFile;
                /*! \brief Referenz auf eine QString-Instanz
                 *
                 *  Enthält den Pfad und den Dateinamen des aktuell benutzten ersten SecureFile für einen Merge-Vorgang (mit der Endung *.rnd).
                 */
                QString *MergeFileName1;
                /*! \brief Referenz auf eine QString-Instanz
                 *
                 *  Enthält den Pfad und den Dateinamen des aktuell benutzten zweiten SecureFile für einen Merge-Vorgang (mit der Endung *.key).
                 */
                QString *MergeFileName2;
                /*! \brief Referenz auf eine QString-Instanz
                 *
                 *  Enthält den Pfad und den Dateinamen des aktuell benutzten dritten SecureFile für einen Merge-Vorgang (mit der Endung *.bin).
                 */
                QString *MergeFileName3;
                /*!
                 * \brief Zeiger auf eine ByteArray-Instanz, die einen OneTimePad-Schlüssel enthält.
                 *
                 * Diese ByteArray-Instanz enthält einen Schlüssel, der genau so lang ist, wie der zu verschlüsselnde Klartext (derzeit 4096 Bytes).
                 */
                ByteArray *OneTimePadKey;
                /*!
                 * \brief Zeiger auf eine ByteArray-Instanz, die einen OneTimePad-Schlüssel enthält (für einen Merge-Store).
                 *
                 * Diese ByteArray-Instanz enthält einen Schlüssel, der genau so lang ist, wie der zu verschlüsselnde Klartext (derzeit 4096 Bytes).
                 */
                ByteArray *OneTimePadKeyM;
                /*! \brief Zeiger auf eine Instanz der Klasse RH::PBKDF2 zur Schlüsselableitung von einem Passwort.
                 *
                 *  Details sind im Projekt HelperClasses unter der Klasse RH::PBKDF2 nachzulesen.
                 */
                PBKDF2 *pbkdf2;
                /*!
                 * \brief Zeiger auf eine ByteArray-Instanz mit einem 256 Byte langen Vertauschungsschlüssel.
                 *
                 * Zeiger auf eine ByteArray-Instanz, die einen 256 Byte langen Schlüssel enthält, der zum Vertauschen eines 256 Byte langen Arrays verwendet wird.
                 * Alle Byte-Werte in diesem Schlüssel sind unterschiedlich und kommen genau einmal vor.
                 */
                ByteArray *PermutationKey;
                /*!
                 * \brief Zeiger auf eine ByteArray-Instanz mit einem 256 Byte langen Vertauschungsschlüssel (für einen Merge-Store).
                 *
                 * Zeiger auf eine ByteArray-Instanz, die einen 256 Byte langen Schlüssel enthält, der zum Vertauschen eines 256 Byte langen Arrays verwendet wird.
                 * Alle Byte-Werte in diesem Schlüssel sind unterschiedlich und kommen genau einmal vor.
                 */
                ByteArray *PermutationKeyM;
                /*! \brief Zeiger auf eine Instanz der Klasse RH::Random zur Generierung von Zufallszahlen.
                 *
                 *  Details sind im Projekt HelperClasses unter der Klasse RH::Random nachzulesen.
                 */
                Random *rand;
                /*! \brief Zeiger auf ein ByteArray, das Zufallsdaten zur Schlüsselwiederherstellung enthält.
                 *
                 *  Derzeit werden ein MegaByte (1.048.576 Bytes) verwendet.
                 */
                ByteArray *RandomData;
                /*! \brief Länge des Datenfeldes mit Zufallszahlen zur Schlüsselwiederherstellung in Bytes.
                 *
                 *  Derzeit werden ein MegaByte (1.048.576 Bytes) verwendet.
                 */
                size_t RandomDataLength;
                /*!
                 * \brief 64 Byte langer Salt-Wert.
                 */
                char Salt1[64] = { 8, 126, -79, 78, -26, 82, -55, -72, 30, 19, 36, -112, -88, -73, 22, 104, -128, 27, -11, -15,
                                  -108, -71, 65, -21, -124, 123, -20, -39, -50, 92, -46, -53, -48, 28, 112, 100, 103, 114, 39, 64,
                                  -118, -5, -126, 116, -93, -24, -124, 126, 114, -52, 116, 34, -126, -115, -61, -13, 38, -123, 35, -64,
                                  9, -127, 80, -81 };

                /*!
                 * \brief 64 Byte langer Salt-Wert.
                 */
                char Salt2[64] = { 107, -67, -38, 74, -27, -90, 1, 3, 95, 2, 58, 45, 41, -30, -87, 51, 13, 12, 90, 55,
                                  -38, 57, -93, -18, 94, 107, 75, 13, 50, 85, -65, -17, -107, 96, 24, -112, -81, -40, 7, 9,
                                  39, -28, -76, -47, -93, -55, 15, -74, 81, -28, 116, 75, -72, -93, -56, 17, 47, 44, 38, 73,
                                  108, 68, 39, -75 };

                /*! \brief Zeiger auf eine Instanz der Klasse Store::SecureStore.
                 *
                 * Die Instanz wird nacheinander für alle drei Dateien eines Secure-Store benutzt. Details sind im Projekt HelperClasses unter der Klasse Store::SecureStore nachzulesen.
                 */
                Store::SecureFile *SecFile;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die erste Ver- oder Entschlüsselung des ersten SecureFile (mit der Endung *.rnd).
                 *  Der erste Initialisierungsvektor für die erste AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __ersten__ eingegebenen
                 *  Passwort, der __ersten__ Hashrunden-Anzahl und einem ersten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1IV1, Merge1IV2, Merge2IV1, Merge2IV2, Merge3IV1, Merge3IV2, SecFile1IV2, SecFile2IV1, SecFile2IV2, SecFile3IV1 und SecFile3IV2
                 */
                ByteArray *SecFile1IV1;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die zweite Ver- oder Entschlüsselung des ersten SecureFile (mit der Endung *.rnd).
                 *  Der zweite Initialisierungsvektor für die zweite AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __ersten__ eingegebenen
                 *  Passwort, der __ersten__ Hashrunden-Anzahl und einem zweiten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1IV1, Merge1IV2, Merge2IV1, Merge2IV2, Merge3IV1, Merge3IV2, SecFile1IV1, SecFile2IV1, SecFile2IV2, SecFile3IV1 und SecFile3IV2
                 */
                ByteArray *SecFile1IV2;
                /*! \brief Referenz auf ein ByteArray, das einen 32 Byte langen Schlüssel zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die erste Ver- oder Entschlüsselung des ersten SecureFile (mit der Endung *.rnd).
                 *  Der erste Schlüssel für die erste AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __ersten__ eingegebenen
                 *  Passwort, der __ersten__ Hashrunden-Anzahl und einem ersten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1Key1, Merge1Key2, Merge2Key1, Merge2Key2, Merge3Key1, Merge3Key2, SecFile1Key2, SecFile2Key1, SecFile2Key2, SecFile3Key1 und SecFile3Key2
                 */
                ByteArray *SecFile1Key1;
                /*! \brief Referenz auf ein ByteArray, das einen 32 Byte langen Schlüssel zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die zweite Ver- oder Entschlüsselung des ersten SecureFile (mit der Endung *.rnd).
                 *  Der zweite Schlüssel für die zweite AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __ersten__ eingegebenen
                 *  Passwort, der __ersten__ Hashrunden-Anzahl und einem zweiten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1Key1, Merge1Key2, Merge2Key1, Merge2Key2, Merge3Key1, Merge3Key2, SecFile1Key1, SecFile2Key1, SecFile2Key2, SecFile3Key1 und SecFile3Key2
                 */
                ByteArray *SecFile1Key2;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die erste Ver- oder Entschlüsselung des zweiten SecureFile (mit der Endung *.key).
                 *  Der erste Initialisierungsvektor für die erste AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __zweiten__ eingegebenen
                 *  Passwort, der __zweiten__ Hashrunden-Anzahl und einem ersten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1IV2, Merge1IV2, Merge2IV1, Merge2IV2, Merge3IV1, Merge3IV2, SecFile1IV1, SecFile1IV2, SecFile2IV2, SecFile3IV1 und SecFile3IV2
                 */
                ByteArray *SecFile2IV1;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die zweite Ver- oder Entschlüsselung des zweiten SecureFile (mit der Endung *.key).
                 *  Der zweite Initialisierungsvektor für die zweite AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __zweiten__ eingegebenen
                 *  Passwort, der __zweiten__ Hashrunden-Anzahl und einem zweiten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1IV1, Merge1IV2, Merge2IV1, Merge2IV2, Merge3IV1, Merge3IV2, SecFile1IV1, SecFile1IV2, SecFile2IV1, SecFile3IV1 und SecFile3IV2
                 */
                ByteArray *SecFile2IV2;
                /*! \brief Referenz auf ein ByteArray, das einen 32 Byte langen Schlüssel zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die erste Ver- oder Entschlüsselung des zweiten SecureFile (mit der Endung *.key).
                 *  Der erste Schlüssel für die erste AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __zweiten__ eingegebenen
                 *  Passwort, der __zweiten__ Hashrunden-Anzahl und einem ersten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1Key1, Merge1Key2, Merge2Key1, Merge2Key2, Merge3Key1, Merge3Key2, SecFile1Key1, SecFile1Key2, SecFile2Key2, SecFile3Key1 und SecFile3Key2
                 */
                ByteArray *SecFile2Key1;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die zweite Ver- oder Entschlüsselung des zweiten SecureFile (mit der Endung *.key).
                 *  Der zweite Schlüssel für die zweite AES-Verschlüsselung dieser Datei wird mit dem PBKDF2-Verfahren vom __zweiten__ eingegebenen
                 *  Passwort, der __zweiten__ Hashrunden-Anzahl und einem zweiten fest im Programm vorgegebenen Salt-Wert gebildet.
                 *  \sa Merge1Key1, Merge1Key2, Merge2Key1, Merge2Key2, Merge3Key1, Merge3Key2, SecFile1Key1, SecFile1Key2, SecFile2Key1, SecFile3Key1 und SecFile3Key2
                 */
                ByteArray *SecFile2Key2;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die erste Ver- oder Entschlüsselung des dritten SecureFile (mit der Endung *.bin).
                 *  Der erste Initialisierungsvektor für die erste AES-Verschlüsselung dieser Datei wurde bei der Erstellung des Secure-Store vom Zufallszahlengenerator generiert.
                 *  Die Daten werden bei der Eröffnung des Secure-Store aus den Zufallsbytes aus der ersten Datei und den Integerwerten aus der zweiten Datei dann richtig zurück berechnet,
                 *  wenn der Startindex und der Differenzwert vom Benutzer korrekt eingegeben wurde.
                 *  \sa Merge1IV1, Merge1IV2, Merge2IV1, Merge2IV2, Merge3IV1, Merge3IV2, SecFile1IV1, SecFile1IV2, SecFile2IV1, SecFile2IV2 und SecFile3IV2
                 */
                ByteArray *SecFile3IV1;
                /*! \brief Referenz auf ein ByteArray, das einen 16 Byte langen Initialisierungsvektor zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 16 Byte langen Initialisierungsvektor für die zweite Ver- oder Entschlüsselung des dritten SecureFile (mit der Endung *.bin).
                 *  Der zweite Initialisierungsvektor für die zweite AES-Verschlüsselung dieser Datei wurde bei der Erstellung des Secure-Store vom Zufallszahlengenerator generiert.
                 *  Die Daten werden bei der Eröffnung des Secure-Store aus den Zufallsbytes aus der ersten Datei und den Integerwerten aus der zweiten Datei dann richtig zurück berechnet,
                 *  wenn der Startindex und der Differenzwert vom Benutzer korrekt eingegeben wurde.
                 *  \sa Merge1IV1, Merge1IV2, Merge2IV1, Merge2IV2, Merge3IV1, Merge3IV2, SecFile1IV1, SecFile1IV2, SecFile2IV1, SecFile2IV2 und SecFile3IV1
                 */
                ByteArray *SecFile3IV2;
                /*! \brief Referenz auf ein ByteArray, das einen 32 Byte langen Schlüssel zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die erste Ver- oder Entschlüsselung des dritten SecureFile (mit der Endung *.bin).
                 *  Der erste Schlüssel für die erste AES-Verschlüsselung dieser Datei wurde bei der Erstellung des Secure-Store vom Zufallszahlengenerator generiert.
                 *  Die Daten werden bei der Eröffnung des Secure-Store aus den Zufallsbytes aus der ersten Datei und den Integerwerten aus der zweiten Datei dann richtig zurück berechnet,
                 *  wenn der Startindex und der Differenzwert vom Benutzer korrekt eingegeben wurde.
                 *  \sa Merge1Key1, Merge1Key2, Merge2Key1, Merge2Key2, Merge3Key1, Merge3Key2, SecFile1Key1, SecFile1Key2, SecFile2Key1, SecFile2Key2 und SecFile3Key2
                 */
                ByteArray *SecFile3Key1;
                /*! \brief Referenz auf ein ByteArray, das einen 32 Byte langen Schlüssel zum Öffnen eines SecureFile enthält.
                 *
                 *  Das ByteArray enthält den 32 Byte langen Schlüssel für die zweite Ver- oder Entschlüsselung des dritten SecureFile (mit der Endung *.bin).
                 *  Der zweite Schlüssel für die zweite AES-Verschlüsselung dieser Datei wurde bei der Erstellung des Secure-Store vom Zufallszahlengenerator generiert.
                 *  Die Daten werden bei der Eröffnung des Secure-Store aus den Zufallsbytes aus der ersten Datei und den Integerwerten aus der zweiten Datei dann richtig zurück berechnet,
                 *  wenn der Startindex und der Differenzwert vom Benutzer korrekt eingegeben wurde.
                 *  \sa Merge1Key1, Merge1Key2, Merge2Key1, Merge2Key2, Merge3Key1, Merge3Key2, SecFile1Key1, SecFile1Key2, SecFile2Key1, SecFile2Key2 und SecFile3Key1
                 */
                ByteArray *SecFile3Key2;
                /*! \brief Referenz auf eine QString-Instanz
                 *
                 *  Enthält den Pfad und den Dateinamen des aktuell benutzten ersten SecureFile (mit der Endung *.rnd).
                 */
                QString *SecFileName1;
                /*! \brief Referenz auf eine QString-Instanz
                 *
                 *  Enthält den Pfad und den Dateinamen des aktuell benutzten zweiten SecureFile (mit der Endung *.key).
                 */
                QString *SecFileName2;
                /*! \brief Referenz auf eine QString-Instanz
                 *
                 *  Enthält den Pfad und den Dateinamen des aktuell benutzten dritten SecureFile (mit der Endung *.bin).
                 */
                QString *SecFileName3;
                /*! \brief Referenz auf eine QString-Instanz
                 *
                 *  Enthält (im Securemode) den Namen des gerade bearbeiteten Elements im Secure-Store.
                 */
                QString *SecItemName;
                /*! \brief Zeiger auf eine Instanz der Klassse RH::Secure, die Methoden zur Ver- und Entschlüsselung bereit hält.
                 *
                 *  Details sind im Projekt HelperClasses unter der Klasse RH::Secure nachzulesen.
                 */
                Secure *secure;
                /*! \brief Zeiger auf eine Instanz der Klassse RH::Secure, die Methoden zur Ver- und Entschlüsselung bereit hält (für einen Merge-Store).
                 *
                 *  Details sind im Projekt HelperClasses unter der Klasse RH::Secure nachzulesen.
                 */
                Secure *secureM;
                /*! \brief Diese (boolsche) Variable kennzeichnet in welchem Betriebsmodus sich das Programm gerade befindet.
                 *
                 *  _false_, das Programm ist im Nomal-Modus.
                 *  _true_,  das Programm ist im Secure-Modus.
                 */
                bool SecureMode;
                /*! \brief Diese (boolsche) Variable kennzeichnet im Secure-Modus ob bereits ein Element aus dem Secure-Store geladen wurde.
                 *
                 *  _false_, es wurde bereits ein Element aus dem Secure-Store geladen.
                 *  _true_,  es wurde noch kein Element aus dem Secure-Store geladen.
                 *
                 *  Diese Information ist wichtig für die beiden Methoden `NextButton_Click()` und `PriorButton_Click()` damit sie das erste zu ladende
                 *  Element korrekt laden.
                 */
                bool SelectFirst;
                /*! \brief Diese (boolsche) Variable kennzeichnet im Secure-Modus welche Elemente im Secure-Store bearbeitet werden sollen.
                 *
                 *  _false_, es werden normale (ungelöschte) Elemente im Secure-Store bearbeitet.
                 *  _true_,  es werden die Elemente bearbeitet, die sich im Mülleimer befinden (also bereits als gelöscht gekennzeichnet sind).
                 */
                bool SelectTrash;
                /*! \brief Zeiger auf eine Instanz der Klasse QSettings.
                 *
                 *  In dieser Instanz der QSettings-Klasse werden alle Einstellungen des Programms verwaltet. Dies sind z.B die ausgewählte Spracheinstellung, die Größe und
                 *  Lage des Programms auf dem Bildschirm oder der letzte augewählte Secure-Store.
                 */
                QSettings *settings;
                /*! \brief Zeiger auf eine Instanz der Klassse RH::SHA384, die Methoden zur Hash-Erstellung bereit hält.
                 *
                 *  Details sind im Projekt HelperClasses unter der Klasse RH::SHA384 nachzulesen.
                 */
                SHA384 *sha384;
                /*! \brief Zeiger auf eine Instanz der Klassse RH::SHA512, die Methoden zur Hash-Erstellung bereit hält.
                 *
                 *  Details sind im Projekt HelperClasses unter der Klasse RH::SHA512 nachzulesen.
                 */
                SHA512 *sha512;
                /*! \brief Die Variable enthält die derzeit benutzte Länge des OTP-Schlüssels.
                 *
                 *  Derzeit sind im Programm fest 4096 Byte eingestellt (diese Größe derzeit ist nicht per Bedienung veränderbar).
                 */
                size_t size = 4096;
                /*! \brief Zeiger auf eine Instanz der Klasse QLabel
                 *
                 * Mit dieser QLabel-Instanz wird das erste Textfeld in der Statuszeile dargestellt.
                 */
                QLabel *sLabel1;
                /*! \brief Zeiger auf eine Instanz der Klasse QLabel
                 *
                 * Mit dieser QLabel-Instanz wird das zweitee Textfeld in der Statuszeile dargestellt.
                 */
                QLabel *sLabel2;
                /*! \brief Zeiger auf eine Instanz der Klasse QLabel
                 *
                 * Mit dieser QLabel-Instanz wird das dritte Textfeld in der Statuszeile dargestellt.
                 */
                QLabel *sLabel3;
                /*! \brief Zeiger auf eine Instanz der Klasse QLabel
                 *
                 * Mit dieser QLabel-Instanz wird das vierte Textfeld in der Statuszeile dargestellt.
                 */
                QLabel *sLabel4;
                /*! \brief Zeiger auf eine Instanz der Klasse QLabel
                 *
                 * Mit dieser QLabel-Instanz wird das Textfeld in der Tool-Bar-Zeile (der letzte ausgewählte Secure-Store) dargestellt.
                 */
                QLabel *sLabel5;
                /*! \brief Die Variable enthält den vom Benutzer gewählten Differenz-Wert.
                 *
                 *  Derzeit kann vom Benutzer (bei der Anlage eines neuen Secure-Store) ein Wert zwischen 1 und 250.000 gewählt werden.
                 */
                int Spread;
                /*! \brief Die Variable enthält den vom Benutzer gewählten Start-Index.
                 *
                 *  Derzeit kann vom Benutzer (bei der Anlage eines neuen Secure-Store) ein Wert zwischen 1 und 900.000 gewählt werden.
                 */
                int StartIndex;
                /*! \brief Referenz auf eine QString-Instanz
                 *
                 *  Enthält (im Normalmode) den Pfad- und Datei-Namen der gerade bearbeiteten Datei.
                 */
                QString *TextFileName;
                /*! \brief Referenz auf eine QToolBar-Instanz
                 *
                 *  In der Tool-Bar-Zeile werden vier Schalter, Separatoren und dein Textfeld dargestellt.
                 */
                QToolBar *toolBar;
        };

} // end of namespace SecureDB


#endif // SECUREDB_MAINWINDOW_H
