/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file ItemTable.h
 *  \brief Stellt die API zu der Klasse __ItemTable__ bereit.
 *
 * Die Klasse __ItemTable__ ist eine Ableitung der Klasse __BasicTable__.
 * Eine Instanz dieser Klasse __ItemTable__ verwaltet die im Store gespeicherten SecureItems, die im Securestore gespeichert sind und
 * hält diese sortiert nach Name und Schreibzeitpunkt (Schlüssel 0) vor.
 */

#ifndef SECUREDB_SECITEMTABLE_H
#define SECUREDB_SECITEMTABLE_H
#include "basicrow.h"
#include "basictable.h"
using namespace RH;
using namespace Table;

namespace SecureDB {

        class Item;         // Vorwärtsdeklaration

        /*! \brief Eine Instanz dieser von BasicTable abgeleiteten Klasse verwaltet die gespeicherten SecureItems im SecureStore.
         *
         * Die Klasse __ItemTable__ ist eine Ableitung der Klasse __BasicTable__.
         * Eine Instanz dieser Klasse __ItemTable__ verwaltet die gespeicherten SecureItems im Store und hält diese sortiert nach Name und Schreibzeitpunkt (Schlüssel 0) vor.
         */
        class ItemTable : public BasicTable {
        public:
                ItemTable();
                virtual ~ItemTable() override;
                Item *Row();
                Item *SearchRow();
        protected:
                virtual void NewRow() override;
        };

} // end of namespace SecureDB

#endif // SECUREDB_SECITEMTABLE_H
