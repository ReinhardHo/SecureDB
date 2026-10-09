/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file pwdaten1.h
 *  \brief Definiert die Klasse __PWDaten1__.
 *
 *  Die Klasse __PWDaten1__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Eingabe von Passwort-Daten.
 *  Die Klasse PWDaten1 wird benötigt, um vom Normalmodus in den Secure-Modus des Programms zu gelangen.
 */
#ifndef SECUREDB_PWDATEN1_H
#define SECUREDB_PWDATEN1_H
#include <QDialog>
#include <QtGui>
#include <ui_pwdaten1.h>
#include <bytearray.h>
using namespace RH;

namespace SecureDB {

        /*! \brief Die Klasse __PWDaten1__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Eingabe von Passwort-Daten.
         *
         *  Die Klasse PWDaten1 wird benötigt, um vom Normalmodus in den Secure-Modus des Programms zu gelangen.
         */
        class PWDaten1 : public QDialog {
                Q_OBJECT
        public:
                /*! \brief Zeiger auf eine Instanz der Klasse ByteArray.
                 *
                 *  Die ByteArray-Instanz enthält den eingegebenen Differenzindex.
                 */
                ByteArray *di1;
                /*! \brief Diese (boolsche) Variable kennzeichnet, ob die Daten korrekt eingegeben wurde.
                 *
                 *  _false_, keine gültigen Daten vorhanden.
                 *  _true_,  gültige Daten vorhanden.
                 */
                bool isOK;
                /*! \brief Zeiger auf eine Instanz der Klasse ByteArray.
                 *
                 *  Die ByteArray-Instanz enthält die erste eingegebene Hashrundenanzahl.
                 */
                ByteArray *hr1;
                /*! \brief Zeiger auf eine Instanz der Klasse ByteArray.
                 *
                 *  Die ByteArray-Instanz enthält die zweite eingegebene Hashrundenanzahl.
                 */
                ByteArray *hr2;
                /*! \brief Zeiger auf eine Instanz der Klasse ByteArray.
                 *
                 *  Die ByteArray-Instanz enthält das erste eingegebenen Passwort.
                 */
                ByteArray *pw1;
                /*! \brief Zeiger auf eine Instanz der Klasse ByteArray.
                 *
                 *  Die ByteArray-Instanz enthält das zweite eingegebenen Passwort.
                 */
                ByteArray *pw2;
                /*! \brief Zeiger auf eine Instanz der Klasse ByteArray.
                 *
                 *  Die ByteArray-Instanz enthält den eingegebenen Startindex.
                 */
                ByteArray *si1;
                PWDaten1();
                virtual ~PWDaten1();
        private slots:
                void CancelButton_clicked();
                void D1_textEdited(const QString &text);
                void H1_textEdited(const QString &text);
                void H2_textEdited(const QString &text);
                void OKButton_clicked();
                void PW1_textEdited(const QString &text);
                void PW2_textEdited(const QString &text);
                void S1_textEdited(const QString &text);
        private:
                /*! \brief Graphisches User-Interface.
                 *
                 *  Zeiger auf das graphische User-Interface für diesen PWDaten1-Dialog.
                 */
                Ui::Dialog1 ui;
        };

} // end of namespace SecureDB

#endif // SECUREDB_PWDATEN1_H
