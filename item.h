/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file Item.h
 *  \brief Definiert die Klasse __Item__.
 *
 *  Die Klasse __Iteme__ ist eine Ableitung von der Klasse __BasicRow__ und enthält Daten zur Verwaltung eines Elementes im Secure-Store.
 */
#ifndef SECUREDB_SECUREITEM_H
#define SECUREDB_SECUREITEM_H
#include "basicrow.h"
#include "basictable.h"
#include <QString>
using namespace RH;
using namespace Table;

namespace SecureDB {

        /*! \brief Definiert die Klasse __Item__.
         *
         *  Die Klasse __Iteme__ ist eine Ableitung von der Klasse __BasicRow__ und enthält Daten zur Verwaltung eines Elementes im Secure-Store.
         */
        class Item : public BasicRow {
        public:
                Item(BasicTable &table, const bool isSearchInfo);
                virtual ~Item() override;
                QString *getName() const;
                QString *getFullName();
                void setName(const QString &text);
                /*! \brief Enthält eine Teilinformation des Speicherzeitpunktes.
                 *
                 *  Enthält die Tagesinformation des Speicherzeitpunktes.
                 */
                int Day;
                /*! \brief Diese Variable kennzeichnet den Status des Elementes im Secure-Store.
                 *
                 *  _0_, es handelt sich um ein normales (ungelöschte) Elemente im Secure-Store.
                 *  _1_, es handelt sich um ein Element, dass sich im Mülleimer befindet (also bereits als gelöscht gekennzeichnet ist).
                 */
                int deleted;
                /*! \brief Enthält eine Teilinformation des Speicherzeitpunktes.
                 *
                 *  Enthält die Stundeninformation des Speicherzeitpunktes.
                 */
                int Hour;
                /*! \brief Enthält eine Teilinformation des Speicherzeitpunktes.
                 *
                 *  Enthält die Minuteninformation des Speicherzeitpunktes.
                 */
                int Minute;
                /*! \brief Enthält eine Teilinformation des Speicherzeitpunktes.
                 *
                 *  Enthält die Monatsinformation des Speicherzeitpunktes.
                 */
                int Month;
                /*! \brief Enthält eine Teilinformation des Speicherzeitpunktes.
                 *
                 *  Enthält die Sekundeninformation des Speicherzeitpunktes.
                 */
                int Second;
                /*! \brief Enthält den Indexwert unter dem dises Element im SecureStore gespeichert wird.
                 *
                 *  Diese Information ist für den Zugriff auf das Element entscheidend.
                 */
                size_t StoreIndex;
                /*! \brief Enthält eine Teilinformation des Speicherzeitpunktes.
                 *
                 *  Enthält die Jahresinformation des Speicherzeitpunktes.
                 */
                int Year;
        protected:
                virtual BasicRow *CloneThis() const override;
                virtual void CopyToDestination(BasicRow &ref) const override;
                virtual void ExportToByteArray(ByteArray &ref) const override;
                virtual void ImportFromByteArray(ByteArray &ref) override;
                virtual bool IsEqualKey(BasicRow &row, const size_t keyIndex) const override;
                virtual bool IsLessKey(BasicRow &row, const size_t keyIndex) const override;
        private:
                std::string ConvertToStr(int value, size_t length);
                /*! \brief Zeiger auf eine Instanz der Klassse QString
                 *
                 *  Enthält den Namen mit Datum und Uhrzeit.
                 */
                QString *fullName;
                /*! \brief Zeiger auf eine Instanz der Klassse QString
                 *
                 *  Enthält den Namen, unter dem dieses Element dem Bebutzer angezeigt wird.
                 */
                QString *Name;
        };

        //***************** inline Implementierungen *****************

        /*! \brief Diese Methode `getName()` gibt den Namen des Elementes in zurück.
         *
         *  Die Methode `getName()` gibt einen Zeiger auf eine QString-Instanz zurück, in der sich der Name dieess Elementes (ohne Datum und Zeitangabe) befindet.
         *  \return Ein Zeiger auf eine QString-Instanz.
         */
        inline QString *Item::getName() const { return Name; }

} // end of namespace SecureDB

#endif // SECUREDB_SECUREITEM_H
