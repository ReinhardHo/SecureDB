/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file pwdaten1.cpp
 *  \brief Realisiert die Klasse __PWDaten1__.
 *
 *  Die Klasse __PWDaten1__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Eingabe von Passwort-Daten.
 *  Die Klasse PWDaten1 wird benötigt, um vom Normalmodus in den Secure-Modus des Programms zu gelangen.
 */
#include "pwdaten1.h"

namespace SecureDB {

        /*! \brief Konstruktor: Initialisiert eine neue Instanz der Klasse __PWDaten1__.
         *
         *  Im Konstruktor werden alle im Dialog benötigten Elemente angelegt.
         */
        PWDaten1::PWDaten1() {
                ui.setupUi(this);
                pw1 = new ByteArray(32, true);
                pw2 = new ByteArray(32, true);
                si1 = new ByteArray(32, true);
                di1 = new ByteArray(32, true);
                hr1 = new ByteArray(32, true);
                hr2 = new ByteArray(32, true);
        }

        /*! \brief Destruktor: Zerstört die vorhandene Instanz der Klasse __PWDaten1__.
         *
         *  Alle Instanzen der Unterelemente werden wieder frei gegeben.
         */
        PWDaten1::~PWDaten1() {
                delete pw1;
                delete pw2;
                delete si1;
                delete di1;
                delete hr1;
                delete hr2;
        }

        /*! \brief Diese Methode `CancelButton_clicked()` wird aufgerufen, wenn im Dialog auf den Schaltet _Abbrechen_ geklickt wird.
         *
         *  Der geöffnete Dialog wird darauf hin abgebrochen und geschlossen.
         */
        void PWDaten1::CancelButton_clicked() {
                isOK = false;
                pw1->Clear();
                pw2->Clear();
                si1->Clear();
                di1->Clear();
                hr1->Clear();
                hr2->Clear();
                QDialog::reject();
        }

        /*! \brief Diese Methode `D1_textEdited()` wird aufgerufen, wenn im Eingabefeld für den Differenz eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _di1_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _di1_ entfernt.
         */
        void PWDaten1::D1_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = di1->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        di1->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.D1->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        di1->RemoveAt(length2 - 1);
                }
        }

        /*! \brief Diese Methode `H1_textEdited()` wird aufgerufen, wenn im Eingabefeld für den ersten Hashwert eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _hr1_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _hr1_ entfernt.
         */
        void PWDaten1::H1_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = hr1->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        hr1->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.H1->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        hr1->RemoveAt(length2 - 1);
                }
        }

        /*! \brief Diese Methode `H2_textEdited()` wird aufgerufen, wenn im Eingabefeld für den zweitenn Hashwert eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _hr2_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _hr2_ entfernt.
         */
        void PWDaten1::H2_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = hr2->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        hr2->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.H2->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        hr2->RemoveAt(length2 - 1);
                }
        }

        /*! \brief Diese Methode `OKButton_clicked()` wird aufgerufen, wenn im Dialog auf den Schaltet _OK_ geklickt wird.
         *
         *  Die Variable _isOK_ wird auf _true_ gesetzt.
         */
        void PWDaten1::OKButton_clicked() {
                isOK = true;
                QDialog::accept();
        }

        /*! \brief Diese Methode `PW1_textEdited()` wird aufgerufen, wenn im Eingabefeld für das erste Passwort eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _pw1_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _pw1_ entfernt.
         */
        void PWDaten1::PW1_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = pw1->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        pw1->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.PW1->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        pw1->RemoveAt(length2 - 1);
                }
        }

        /*! \brief Diese Methode `PW2_textEdited()` wird aufgerufen, wenn im Eingabefeld für das zweite Passwort eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _pw2_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _pw2_ entfernt.
         */
        void PWDaten1::PW2_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = pw2->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        pw2->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.PW2->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        pw2->RemoveAt(length2 - 1);
                }
        }

        /*! \brief Diese Methode `S1_textEdited()` wird aufgerufen, wenn im Eingabefeld für den Startindex eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _si1_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _si1_ entfernt.
         */
        void PWDaten1::S1_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = si1->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        si1->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.S1->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        si1->RemoveAt(length2 - 1);
                }
        }

} // end of namespace SecureDB


