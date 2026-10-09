/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file Item.cpp
 *  \brief Realisiert die Klasse __Item__.
 *
 *  Die Klasse __Iteme__ ist eine Ableitung von der Klasse __BasicRow__ und enthält Daten zur Verwaltung eines Elementes im Secure-Store.
 */
#include "item.h"
#include <QChar>
#include <QByteArray>

namespace SecureDB {

        /*! \brief Konstruktor: Initialisiert eine neue Instanz der __Item-Klasse__.
         *
         *  Im Konstruktor werden alle in dieser Klasse benötigten Elemente (zwei QString-Instanzen) angelegt.
         *  \param table Die Tabelleninstanz, die diese Instanz verwaltet.
         *  \param isSearchInfo _true_, wenn diese Instanz Suchinformationen enthält; _false_, wenn es sich um eine normale Datensatzinstanz handelt.
         */
        Item::Item(BasicTable &table, const bool isSearchInfo) : BasicRow(table, isSearchInfo) {
                Name = new QString();
                fullName = new QString();
                deleted = 0;
        }

        /*! \brief Destruktor: Zerstört die vorhandene Instanz der __Item-Klasse__.
         *
         *  Alle Instanzen der Unterelemente (zwei QString-Instanzen) werden wieder frei gegeben.
         */
        Item::~Item() {
                delete Name;
                delete fullName;
        }

        /*! \brief Dupliziert die Daten dieser Instanz.
         *
         *  Führt die Arbeit des Duplizieren dieser Instanz tatsächlich durch.
         *  \return Referenz auf eine Kopie dieser Instanz.
         */
        BasicRow *Item::CloneThis() const {
                Item *newRow = new Item(*this->myTable, this->searchInfo);
                CopyToDestination(*newRow);
                return newRow;
        }

        /*! \brief Konvertiert einen Integer-Wert in einen Standard-String.
         *
         *  Diese Methode konvertiert einen Integerwert zwischen 00 - 59 in einen zwei Zeichen langen Standard-String.
         *  \return Ein Standard-String, der aus zwei Zeichen besteht (und eineZahl zwischen 00 - 59 darstellt).
         */
        std::string Item::ConvertToStr(int value, size_t length) {
                std::string s1 = std::to_string(value);
                if (s1.length() < length) {
                        return "0" + s1;
                } else {
                        return s1;
                }
        }

        /*! \brief Dupliziert die Daten dieser Instanz in eine Zielinstanz.
         *
         *  Führt die Arbeit des Kopierens dieser Instanz in eine Zielinstanz tatsächlich durch.
         *  \param ref Referenz auf die Zielinstanz, in die kopiert wird.
         */
        void Item::CopyToDestination(BasicRow &ref) const {
                Item &targetRow = static_cast<Item&>(ref);
                targetRow.Day = this->Day;
                targetRow.Hour = this->Hour;
                targetRow.Minute = this->Minute;
                targetRow.Month = this->Month;
                if (this->Name != nullptr) {
                        targetRow.Name = new QString(*this->Name);
                } else {
                        targetRow.Name = nullptr;
                }
                targetRow.Second = this->Second;
                targetRow.StoreIndex = this->StoreIndex;
                targetRow.Year = this->Year;
                targetRow.deleted = this->deleted;
        }

        /*! \brief Exportiert alle Datenfelder dieser Instanz in ein ByteArray.
         *
         *  Exportiert alle Datenfelder aus dieser Instanz so in ein ByteArray,
         *  dass sie mit der Methode `ImportFromByteArray()` wieder eingelesen werden können.
         *  \param ref referenz auf ein ByteArray, in das exportiert werden soll.
         */
        void Item::ExportToByteArray(ByteArray &ref) const {
                size_t index = 0;
                ref.WriteX209Int64(index, Day);
                ref.WriteX209Int64(index, Hour);
                ref.WriteX209Int64(index, Minute);
                ref.WriteX209Int64(index, Month);
                ref.WriteX209Int64(index, Second);
                ref.WriteX209Int64(index, Year);
                ref.WriteX209Int64(index, deleted);
                ref.WriteX209Int64(index, static_cast<int64_t>(StoreIndex));
                int length;
                QByteArray qba;
                if (Name == nullptr) {
                        length = 0;
                } else {
                        qba = Name->toUtf8();
                        length = qba.size();
                }
                ref.WriteX209Int64(index, length);
                ref.Copy(reinterpret_cast<char*>(qba.data()), 0, static_cast<size_t>(index), static_cast<size_t>(length));
        }

        /*! \brief Diese Methode `getFullName()` gibt den vollständigen Namen des Elementes in zurück.
         *
         *  Die Methode `getName()` gibt einen Zeiger auf eine QString-Instanz zurück, in der sich der Name dieess Elementes (inclusive Datum und Zeitangabe) befindet.
         *  \return Ein Zeiger auf eine QString-Instanz.
         */
        QString *Item::getFullName() {
                fullName->clear();
                fullName->append(*Name);
                fullName->append(" ");
                fullName->append(ConvertToStr(Day, 2).c_str());
                fullName->append(".");
                fullName->append(ConvertToStr(Month, 2).c_str());
                fullName->append(".");
                fullName->append(ConvertToStr(Year, 4).c_str());
                fullName->append(" ");
                fullName->append(ConvertToStr(Hour, 2).c_str());
                fullName->append(":");
                fullName->append(ConvertToStr(Minute, 2).c_str());
                fullName->append(":");
                fullName->append(ConvertToStr(Second, 2).c_str());
                return fullName;
        }

        /*! \brief Importiert alle Datenfelder dieser Instanz aus einem ByteArray.
         *
         *  Importiert alle Datenfelder in diese Instanz aus einem ByteArray,
         *  die vorher mit der Methode `ExportToByteArray()` dort hinein exportiert worden sind.
         *  \param ref Referenz auf ein ByteArray, aus dem importiert werden soll.
         */
        void Item::ImportFromByteArray(ByteArray &ref) {
                size_t index = 0;
                Day = ref.ReadX209Int64(index);
                Hour = ref.ReadX209Int64(index);
                Minute = ref.ReadX209Int64(index);
                Month = ref.ReadX209Int64(index);
                Second = ref.ReadX209Int64(index);
                Year = ref.ReadX209Int64(index);
                deleted = ref.ReadX209Int64(index);
                StoreIndex = static_cast<size_t>(ref.ReadX209Int64(index));
                int length = ref.ReadX209Int64(index);
                if (length > 0) {
                        delete Name;
                        Name = new QString(QString::fromUtf8(reinterpret_cast<const char*>(&ref.WriteareaReferenz(index, static_cast<size_t>(length))), length));
                }
        }

        /*! \brief Test von Schlüsseln auf Gleichheit.
         *
         *  Die virtuelle Methode `IsEqualKey()` vergleicht Name, Datum und Uhrzeit (Schlüsselindex 0) auf Gleichheit.
         *
         *  Die virtuelle Methode `IsEqualKey()` vergleicht Name, Datum und Uhrzeit (Schlüsselindex 1) auf Gleichheit.
         *
         *  Die virtuelle Methode `IsEqualKey()` vergleicht Datum und Uhrzeit (Schlüsselindex 2) auf Gleichheit.
         *
         *  Die virtuelle Methode `IsEqualKey()` vergleicht Datum und Uhrzeit (Schlüsselindex 3) auf Gleichheit.
         *
         *  Ist der eigene Schlüssel gleich mit dem aus der Vergleichsinstanz wird _true_ zurückgegeben. Sind beide Schlüssel verschieden wird _false_ zurückgegeben.
         *
         *  \param row Instanz, mit der der Vergleich durchgeführt werden soll.
         *  \param keyIndex Index des zu prüfenden Schlüssels (0 .. n).
         *  \return _true_, wenn eigener Schlüssel gleich dem Schlüssel aus der Vergleichsinstanz ist.
         */
        bool Item::IsEqualKey(BasicRow &row, const size_t keyIndex) const {
                Item &test = static_cast<Item&>(row);
                switch (keyIndex) {
                case 0:
                case 1:
                        if (this->Name->compare(*test.Name, Qt::CaseInsensitive) == 0) {
                                if (this->Year == test.Year) {
                                        if (this->Month == test.Month) {
                                                if (this->Day == test.Day) {
                                                        if (this->Hour == test.Hour) {
                                                                if (this->Minute == test.Minute) {
                                                                        if (this->Second == test.Second) {
                                                                                return true;
                                                                        } else {
                                                                                return false;
                                                                        }
                                                                } else {
                                                                        return false;
                                                                }
                                                        } else {
                                                                return false;
                                                        }
                                                } else {
                                                        return false;
                                                }
                                        } else {
                                                return false;
                                        }
                                } else {
                                        return false;
                                }
                        } else {
                                return false;
                        }
                case 2:
                case 3:
                        if (this->Year == test.Year) {
                                if (this->Month == test.Month) {
                                        if (this->Day == test.Day) {
                                                if (this->Hour == test.Hour) {
                                                        if (this->Minute == test.Minute) {
                                                                if (this->Second == test.Second) {
                                                                        return true;
                                                                } else {
                                                                        return false;
                                                                }
                                                        } else {
                                                                return false;
                                                        }
                                                } else {
                                                        return false;
                                                }
                                        } else {
                                                return false;
                                        }
                                } else {
                                        return false;
                                }
                        } else {
                                return false;
                        }
                default:
                        return true;
                }
        }

        /*! \brief Test, ob eigener Schlüssel kleiner als in der Vergleichsinstanz.
         *
         *  Die virtuelle Methode `IsLessKey()` prüft, ob der eigene Name, das Datum und die Uhrzeit (Schlüsselindex 0) kleiner als die aus einem Vergleichsdatensatz ist.
         *
         *  Die virtuelle Methode `IsLessKey()` prüft, ob der eigene Name, das Datum und die Uhrzeit (Schlüsselindex 1) größer als die aus einem Vergleichsdatensatz ist.
         *
         *  Die virtuelle Methode `IsLessKey()` prüft, ob das Datum und die Uhrzeit (Schlüsselindex 2) kleiner als die aus einem Vergleichsdatensatz ist.
         *
         *  Die virtuelle Methode `IsLessKey()` prüft, ob das Datum und die Uhrzeit (Schlüsselindex 3) größer als die aus einem Vergleichsdatensatz ist.
         *
         *  Ist der eigene Schlüssel kleiner als der der Vergleichsinstanz wird _true_ zurückgegeben. Sind beide Schlüssel gleich
         *  oder ist der eigene Schlüssel größer wird _false_ zurückgegeben.
         *
         *  \param row Instanz, mit der der Vergleich durchgeführt werden soll.
         *  \param keyIndex Index des zu prüfenden Schlüssels (0 .. n).
         *  \return _true_, wenn eigener Schlüssel kleiner als der Schlüssel aus der Vergleichsinstanz ist.
         */
        bool Item::IsLessKey(BasicRow &row, const size_t keyIndex) const {
                Item &test = static_cast<Item&>(row);
                switch (keyIndex) {
                case 0:
                        if (this->Name->compare(*test.Name, Qt::CaseInsensitive) < 0) {
                                return true;
                        } else {
                                if (this->Name->compare(*test.Name, Qt::CaseSensitive) == 0) {
                                        if (this->Year < test.Year) {
                                                return true;
                                        } else {
                                                if (this->Year == test.Year) {
                                                        if (this->Month < test.Month) {
                                                                return true;
                                                        } else {
                                                                if (this->Month == test.Month) {
                                                                        if (this->Day < test.Day) {
                                                                                return true;
                                                                        } else {
                                                                                if (this->Day == test.Day) {
                                                                                        if (this->Hour < test.Hour) {
                                                                                                return true;
                                                                                        } else {
                                                                                                if (this->Hour == test.Hour) {
                                                                                                        if (this->Minute < test.Minute) {
                                                                                                                return true;
                                                                                                        } else {
                                                                                                                if (this->Minute == test.Minute) {
                                                                                                                        if (this->Second < test.Second) {
                                                                                                                                return true;
                                                                                                                        } else {
                                                                                                                                return false;
                                                                                                                        }
                                                                                                                } else {
                                                                                                                        return false;
                                                                                                                }
                                                                                                        }
                                                                                                } else {
                                                                                                        return false;
                                                                                                }
                                                                                        }
                                                                                } else {
                                                                                        return false;
                                                                                }
                                                                        }
                                                                } else {
                                                                        return false;
                                                                }
                                                        }
                                                } else {
                                                        return false;
                                                }
                                        }
                                } else {
                                        return false;
                                }
                        }
                case 1:
                        if (this->Name->compare(*test.Name, Qt::CaseInsensitive) > 0) {
                                return true;
                        } else {
                                if (this->Name->compare(*test.Name, Qt::CaseSensitive) == 0) {
                                        if (this->Year > test.Year) {
                                                return true;
                                        } else {
                                                if (this->Year == test.Year) {
                                                        if (this->Month > test.Month) {
                                                                return true;
                                                        } else {
                                                                if (this->Month == test.Month) {
                                                                        if (this->Day > test.Day) {
                                                                                return true;
                                                                        } else {
                                                                                if (this->Day == test.Day) {
                                                                                        if (this->Hour > test.Hour) {
                                                                                                return true;
                                                                                        } else {
                                                                                                if (this->Hour == test.Hour) {
                                                                                                        if (this->Minute > test.Minute) {
                                                                                                                return true;
                                                                                                        } else {
                                                                                                                if (this->Minute == test.Minute) {
                                                                                                                        if (this->Second > test.Second) {
                                                                                                                                return true;
                                                                                                                        } else {
                                                                                                                                return false;
                                                                                                                        }
                                                                                                                } else {
                                                                                                                        return false;
                                                                                                                }
                                                                                                        }
                                                                                                } else {
                                                                                                        return false;
                                                                                                }
                                                                                        }
                                                                                } else {
                                                                                        return false;
                                                                                }
                                                                        }
                                                                } else {
                                                                        return false;
                                                                }
                                                        }
                                                } else {
                                                        return false;
                                                }
                                        }
                                } else {
                                        return false;
                                }
                        }
                case 2:
                        if (this->Year < test.Year) {
                                return true;
                        } else {
                                if (this->Year == test.Year) {
                                        if (this->Month < test.Month) {
                                                return true;
                                        } else {
                                                if (this->Month == test.Month) {
                                                        if (this->Day < test.Day) {
                                                                return true;
                                                        } else {
                                                                if (this->Day == test.Day) {
                                                                        if (this->Hour < test.Hour) {
                                                                                return true;
                                                                        } else {
                                                                                if (this->Hour == test.Hour) {
                                                                                        if (this->Minute < test.Minute) {
                                                                                                return true;
                                                                                        } else {
                                                                                                if (this->Minute == test.Minute) {
                                                                                                        if (this->Second < test.Second) {
                                                                                                                return true;
                                                                                                        } else {
                                                                                                                return false;
                                                                                                        }
                                                                                                } else {
                                                                                                        return false;
                                                                                                }
                                                                                        }
                                                                                } else {
                                                                                        return false;
                                                                                }
                                                                        }
                                                                } else {
                                                                        return false;
                                                                }
                                                        }
                                                } else {
                                                        return false;
                                                }
                                        }
                                } else {
                                        return false;
                                }
                        }
                case 3:
                        if (this->Year > test.Year) {
                                return true;
                        } else {
                                if (this->Year == test.Year) {
                                        if (this->Month > test.Month) {
                                                return true;
                                        } else {
                                                if (this->Month == test.Month) {
                                                        if (this->Day > test.Day) {
                                                                return true;
                                                        } else {
                                                                if (this->Day == test.Day) {
                                                                        if (this->Hour > test.Hour) {
                                                                                return true;
                                                                        } else {
                                                                                if (this->Hour == test.Hour) {
                                                                                        if (this->Minute > test.Minute) {
                                                                                                return true;
                                                                                        } else {
                                                                                                if (this->Minute == test.Minute) {
                                                                                                        if (this->Second > test.Second) {
                                                                                                                return true;
                                                                                                        } else {
                                                                                                                return false;
                                                                                                        }
                                                                                                } else {
                                                                                                        return false;
                                                                                                }
                                                                                        }
                                                                                } else {
                                                                                        return false;
                                                                                }
                                                                        }
                                                                } else {
                                                                        return false;
                                                                }
                                                        }
                                                } else {
                                                        return false;
                                                }
                                        }
                                } else {
                                        return false;
                                }
                        }
                default:
                        return true;
                }
        }

        /*! \brief Setzt den Namen dieses Elementes.
         *
         *  Kopiert den Namen aus einen übergebenen QString-Instanz in die eigene QString-Instanz _Name_.
         *  \param text Referenz auf eine QString-Instanz, in der sich der Name befindet.
         */
        void Item::setName(const QString &text) {
                Name->clear();
                Name->append(text);
        }

} // end of namespace SecureDB



