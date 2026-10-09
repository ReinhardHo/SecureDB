/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Itemtable.h"
#include "item.h"

/*! \file ItemTable.cpp
 *  \brief In dieser Datei wird die Klasse __ItemTable__ realisiert.
 */

namespace SecureDB {

        /*! \brief Konstruktor: Initialisiert eine neue Instanz der __ItemTable-Klasse__.
         *
         *  Die Instanz verwaltet vier Schlüssel.
         *  Die Variable _searchInfo_ wird mit einer neuen Suchinstanz der Klasse StoreItem geladen.
         */
        ItemTable::ItemTable() : BasicTable(4) {
                searchInfo = new Item(*this, true);
        }

        /*! \brief Destruktor: Zerstört die vorhandene Instanz der __SecItemTable-Klasse__.
         *
         *   Der Destruktor zerstört die in der Variablen _searchInfo_ referenzierte Instanz der Klasse StoreItem.
         */
        ItemTable::~ItemTable() {
                delete searchInfo;
        }

        /*! \brief Die Methode `Row()` gewährt Zugriff auf die Datensatzinstanz, die gerade in Bearbeitung ist.
         *
         *  \return Zeiger auf eine Instanz der Klasse SecureDB::Item.
         */
        Item *ItemTable::Row() {
                return static_cast<Item*>(currentRow);
        }

        /*! \brief Die Methode `SearchRow()` gewährt Zugriff auf die Datensatzinstanz, die zu Suchzwecken auf andere Datensatzinstanzen benutzt wird.
         *
         *  \return Zeiger auf eine Instanz der Klasse SecureDB::Item.
         */
        Item *ItemTable::SearchRow() {
                return static_cast<Item*>(searchInfo);
        }

        /*! \brief Erzeugt eine neue Datenzeile (Instanz der Item-Klasse).
         *
         *  Erstellt in der Variablen _currentRow_ eine neue Datensatzinstanz der Klasse Item, wenn nicht bereits eine nicht einsortierte Instanz besteht.
         */
        void ItemTable::NewRow() {
                if (!newRowFlag) {
                        currentRow = new Item(*this, false);
                        newRowFlag = true;
                }
        }

} // end of namespace SecureDB
