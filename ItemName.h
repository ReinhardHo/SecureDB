/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file ItemName.h
 *  \brief Definiert die Klasse __Item_Name__.
 *
 *  Die Klasse __Item_Name__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Eingabe eines Element-Namens.
 *  Die Klasse Item_Name dient im Secure-Modus des Programms zur Eingabe eines (neuen) Element-Namens.
 */
#ifndef SECUREDB_ITEMNAME_H
#define SECUREDB_ITEMNAME_H
#include <QDialog>
#include <QtGui>
#include <ui_ItemName.h>

namespace SecureDB {

        /*! \brief Die Klasse __Item_Name__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Eingabe eines Element-Namens.
         *
         *  Die Klasse Item_Name dient im Secure-Modus des Programms zur Eingabe eines (neuen) Element-Namens.
         */
        class Item_Name : public QDialog {
                Q_OBJECT
        public:
                /*! \brief Diese (boolsche) Variable kennzeichnet, ob ein Element-Name korrekt eingegeben wurde.
                 *
                 *  _false_, kein gültiger Element-Name vorhanden.
                 *  _true_,  ein gültiger Element-Name ist vorhanden.
                 */
                bool isOK;
                /*!
                 * \brief QString Name
                 *
                 * Dieser String nimmt den eingegebenen Element-Namen auf.
                 */
                QString Name;
                Item_Name();
                virtual ~Item_Name();
        private slots:
                void accept3();
                void reject3();
        private:
                /*! \brief Graphisches User-Interface.
                 *
                 *  Zeiger auf das graphische User-Interface für diesen Item_Name-Dialog.
                 */
                Ui::ItemNameDialog ui;
        };


} // end of namespace SecureDB

#endif // SECUREDB_ITEMNAME_H
