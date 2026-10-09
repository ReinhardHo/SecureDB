/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file pwdaten2.cpp
 *  \brief Realisiert die Klasse __PWDaten2__.
 *
 *  Die Klasse __PWDaten2__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Eingabe von Passwort-Daten.
 *  Die Klasse PWDaten2 wird benötigt, um einen neuen Secure-Store anzulegen.
 */
#include "pwdaten2.h"

namespace SecureDB {

        /*! \brief Konstruktor: Initialisiert eine neue Instanz der Klasse __PWDaten2__.
         *
         *  Im Konstruktor werden alle im Dialog benötigten Elemente angelegt.
         */
        PWDaten2::PWDaten2() {
                ui.setupUi(this);
                pw1 = new ByteArray(32, true);
                pw12 = new ByteArray(32, true);
                pw2 = new ByteArray(32, true);
                pw22 = new ByteArray(32, true);
                si1 = new ByteArray(32, true);
                si2 = new ByteArray(32, true);
                di1 = new ByteArray(32, true);
                di2 = new ByteArray(32, true);
                hr1 = new ByteArray(32, true);
                hr12 = new ByteArray(32, true);
                hr2 = new ByteArray(32, true);
                hr22 = new ByteArray(32, true);
        }

        /*! \brief Destruktor: Zerstört die vorhandene Instanz der Klasse __PWDaten2__.
         *
         *  Alle Instanzen der Unterelemente werden wieder frei gegeben.
         */
        PWDaten2::~PWDaten2() {
                delete pw1;
                delete pw12;
                delete pw2;
                delete pw22;
                delete si1;
                delete si2;
                delete di1;
                delete di2;
                delete hr1;
                delete hr12;
                delete hr2;
                delete hr22;
        }

        /*! \brief Diese Methode `CancelButton_clicked()` wird aufgerufen, wenn im Dialog auf den Schaltet _Abbrechen_ geklickt wird.
         *
         *  Der geöffnete Dialog wird darauf hin abgebrochen und geschlossen.
         */
        void PWDaten2::CancelButton_clicked() {
                isOK = false;
                pw1->Clear();
                pw12->Clear();
                pw2->Clear();
                pw22->Clear();
                si1->Clear();
                si2->Clear();
                di1->Clear();
                di2->Clear();
                hr1->Clear();
                hr12->Clear();
                hr2->Clear();
                hr22->Clear();
                QDialog::reject();
        }

        /*! \brief Diese Methode `D1_textEdited()` wird aufgerufen, wenn im Eingabefeld für den Differenz eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _di1_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _di1_ entfernt.
         */
        void PWDaten2::D1_textEdited(const QString &text) {
                ui.D1->setEchoMode(QLineEdit::Password);
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

        /*! \brief Diese Methode `D2_textEdited()` wird aufgerufen, wenn im Eingabefeld für den Differenz der Eingabewiederholung eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _di2_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _di2_ entfernt.
         */
        void PWDaten2::D2_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = di2->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        di2->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.D2->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        di2->RemoveAt(length2 - 1);
                }
        }

        /*! \brief Diese Methode `H1_textEdited()` wird aufgerufen, wenn im Eingabefeld für den ersten Hashwert eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _hr1_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _hr1_ entfernt.
         */
        void PWDaten2::H1_textEdited(const QString &text) {
                ui.H1->setEchoMode(QLineEdit::Password);
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

        /*! \brief Diese Methode `H12_textEdited()` wird aufgerufen, wenn im Eingabefeld für den ersten Hashwert der Eingabewiederholung eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _hr12_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _hr12_ entfernt.
         */
        void PWDaten2::H12_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = hr12->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        hr12->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.H12->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        hr12->RemoveAt(length2 - 1);
                }
        }

        /*! \brief Diese Methode `H2_textEdited()` wird aufgerufen, wenn im Eingabefeld für den zweitenn Hashwert eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _hr2_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _hr2_ entfernt.
         */
        void PWDaten2::H2_textEdited(const QString &text) {
                ui.H2->setEchoMode(QLineEdit::Password);
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

        /*! \brief Diese Methode `H22_textEdited()` wird aufgerufen, wenn im Eingabefeld für den zweitenn Hashwert der Eingabewiederholung eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _hr22_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _hr22_ entfernt.
         */
        void PWDaten2::H22_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = hr22->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        hr22->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.H22->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        hr22->RemoveAt(length2 - 1);
                }
        }

        /*! \brief Diese Methode `OKButton_clicked()` wird aufgerufen, wenn im Dialog auf den Schaltet _OK_ geklickt wird.
         *
         *  Die Variable _isOK_ wird auf _true_ gesetzt.
         */
        void PWDaten2::OKButton_clicked() {
                isOK = true;
                size_t length = pw1->Size();
                if (length != pw12->Size()) {
                        isOK = false;
                }
                if (length < 8) {
                        isOK = false;
                }
                if (isOK) {
                        for (size_t i = 0; i < length; i++) {
                                if ((*pw1)[i] != (*pw12)[i]) {
                                        isOK = false;
                                        break;
                                }
                        }
                }
                if (isOK) {
                        length = pw2->Size();
                        if (length != pw22->Size()) {
                                isOK = false;
                        }
                }
                if (length < 8) {
                        isOK = false;
                }
                if (isOK) {
                        for (size_t i = 0; i < length; i++) {
                                if ((*pw2)[i] != (*pw22)[i]) {
                                        isOK = false;
                                        break;
                                }
                        }
                }
                if (isOK) {
                        length = si1->Size();
                        if (length != si2->Size()) {
                                isOK = false;
                        }
                }
                if (isOK) {
                        for (size_t i = 0; i < length; i++) {
                                if ((*si1)[i] != (*si2)[i]) {
                                        isOK = false;
                                        break;
                                }
                        }
                }
                if (isOK) {
                        length = di1->Size();
                        if (length != di2->Size()) {
                                isOK = false;
                        }
                }
                if (isOK) {
                        for (size_t i = 0; i < length; i++) {
                                if ((*di1)[i] != (*di2)[i]) {
                                        isOK = false;
                                        break;
                                }
                        }
                }
                if (isOK) {
                        length = hr1->Size();
                        if (length != hr12->Size()) {
                                isOK = false;
                        }
                }
                if (isOK) {
                        for (size_t i = 0; i < length; i++) {
                                if ((*hr1)[i] != (*hr12)[i]) {
                                        isOK = false;
                                        break;
                                }
                        }
                }
                if (isOK) {
                        length = hr2->Size();
                        if (length != hr22->Size()) {
                                isOK = false;
                        }
                }
                if (isOK) {
                        for (size_t i = 0; i < length; i++) {
                                if ((*hr2)[i] != (*hr22)[i]) {
                                        isOK = false;
                                        break;
                                }
                        }
                }
                if (isOK) {
                        pw12->Clear();
                        pw22->Clear();
                        si2->Clear();
                        di2->Clear();
                        hr12->Clear();
                        hr22->Clear();
                } else {
                        pw1->Clear();
                        pw12->Clear();
                        pw2->Clear();
                        pw22->Clear();
                        si1->Clear();
                        si2->Clear();
                        di1->Clear();
                        di2->Clear();
                        hr1->Clear();
                        hr12->Clear();
                        hr2->Clear();
                        hr22->Clear();
                }
                QDialog::accept();
        }

        /*! \brief Diese Methode `PW1_textEdited()` wird aufgerufen, wenn im Eingabefeld für das erste Passwort eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _pw1_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _pw1_ entfernt.
         */
        void PWDaten2::PW1_textEdited(const QString &text) {
                ui.PW1->setEchoMode(QLineEdit::Password);
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

        /*! \brief Diese Methode `PW12_textEdited()` wird aufgerufen, wenn im Eingabefeld für das erste Passwort der Eingabewiederholung eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _pw12_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _pw12_ entfernt.
         */
        void PWDaten2::PW12_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = pw12->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        pw12->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.PW12->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        pw12->RemoveAt(length2 - 1);
                }
        }

        /*! \brief Diese Methode `PW2_textEdited()` wird aufgerufen, wenn im Eingabefeld für das zweite Passwort eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _pw2_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _pw2_ entfernt.
         */
        void PWDaten2::PW2_textEdited(const QString &text) {
                ui.PW2->setEchoMode(QLineEdit::Password);
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

        /*! \brief Diese Methode `PW22_textEdited()` wird aufgerufen, wenn im Eingabefeld für das zweite Passwort der Eingabewiederholung eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _pw22_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _pw22_ entfernt.
         */
        void PWDaten2::PW22_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = pw22->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        pw22->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.PW22->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        pw22->RemoveAt(length2 - 1);
                }
        }

        /*! \brief Diese Methode `S1_textEdited()` wird aufgerufen, wenn im Eingabefeld für den Startindex eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _si1_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _si1_ entfernt.
         */
        void PWDaten2::S1_textEdited(const QString &text) {
                ui.S1->setEchoMode(QLineEdit::Password);
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

        /*! \brief Diese Methode `S2_textEdited()` wird aufgerufen, wenn im Eingabefeld für den Startindex der Eingabewiederholung eine Veränderung aufgetreten ist.
         *
         *  Wurde ein Zeichen eingegeben, so wird es in die ByteArray-Instanz unter der Variablen _si2_ übernommen. Wurde ein Zeichen aus dem Eingabefeld
         *  gelöscht, so wird das letzte Zeichen aus der ByteArray-Instanz unter der Variablen _si2_ entfernt.
         */
        void PWDaten2::S2_textEdited(const QString &text) {
                int length1 = text.length();
                size_t length2 = si2->Size();
                if (length1 == (static_cast<int>(length2) + 1)) {
                        QChar c1 = text.at(length1 - 1);
                        char c2 = static_cast<char>(c1.cell());
                        si2->Append(c2);
                        QString s = "*";
                        s.fill('*', length1);
                        ui.S2->setText(s);
                }
                if (length1 == (static_cast<int>(length2) - 1)) {
                        si2->RemoveAt(length2 - 1);
                }
        }

} // end of namespace SecureDB


