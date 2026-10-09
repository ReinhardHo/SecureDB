/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file ItemSelect.h
 *  \brief Definiert die Klasse __Item_Selection__.
 *
 *  Die Klasse __Item_Selection__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Auswahl eines Elements aus dem Secure-Store.
 */
#ifndef SECUREDB_ITEMSELECT_H
#define SECUREDB_ITEMSELECT_H
#include <QDialog>
#include <QtGui>
#include <ui_ItemSelection.h>
#include "Itemtable.h"

namespace SecureDB {

        /*! \brief Definiert die Klasse __Item_Selection__.
         *
         *  Die Klasse __Item_Selection__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Auswahl eines Elements aus dem Secure-Store.
         */
        class Item_Selection : public QDialog {
                Q_OBJECT
        public:
                /*! \brief Diese Variable enthält eine Nummer, unter der Ein bestimmtes Element im Secure-Store gespeichert ist.
                 *
                 *  Wird in der Combo-Box des Dialogs ein bestimmtes Element ausgewählt (angeklickt), so wird dessen StorIndex in dieser Variablen gespeichert.
                 */
                size_t Index;
                /*! \brief Diese (boolsche) Variable kennzeichnet, ob ein Element-Name korrekt eingegeben wurde.
                 *
                 *  _false_, kein gültiger Element-Name vorhanden.
                 *  _true_,  ein gültiger Element-Name ist vorhanden.
                 */
                bool isOK;
                /*! \brief Diese (boolsche) Variable kennzeichnet, ob ein Element als gelöscht gekennzeichnet ist.
                 *
                 *  _false_, reguläres Element.
                 *  _true_,  gelöschtes Element (Element ist im Papierkorb).
                 */
                bool selectTrash;
                Item_Selection(ItemTable *Table);
                virtual ~Item_Selection();
        private slots:
                void accept4();
                void ChangeIndex(int value);
                void reject4();
                void sort();
                void trash();
        private:
                /*! \brief Diese Variable enthält den gerade benutzten Tabellenschlüssel.
                 *
                 *  Da in der Tabelle vier verschiedene Schlüssel verendet werden, kann die Variable Werte von 0 bis 3 enthalten.
                 */
                size_t keyIndex;
                /*! \brief Zeiger auf eine Instanz der Klasse SecureDB::ItemTable.
                 *
                 *  In der Tabelle werden die Elemente des Secure-Store verwaltet.
                 */
                ItemTable *tab;
                /*! \brief Graphisches User-Interface.
                 *
                 *  Zeiger auf das graphische User-Interface für diesen Item_Selection-Dialog.
                 */
                Ui::ItemSelect ui;
                void LoadItems();
        };

} // end of namespace SecureDB

#endif // SECUREDB_ITEMSELECT_H
