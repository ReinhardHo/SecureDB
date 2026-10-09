/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file ItemName.cpp
 *  \brief Realisiert die Klasse __Item_Name__.
 *
 *  Die Klasse __Item_Name__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Eingabe eines Element-Namens.
 *  Die Klasse Item_Name dient im Secure-Modus des Programms zur Eingabe eines (neuen) Element-Namens.
 */
#include "ItemName.h"

namespace SecureDB {

        /*! \brief Konstruktor: Initialisiert eine neue Instanz der __Item_Name-Klasse__.
         *
         *  Im Konstruktor werden alle im Dialog benötigten Elemente angelegt.
         */
        Item_Name::Item_Name() {
                ui.setupUi(this);
                Name = "";
                connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(accept3()));
                connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(reject3()));
        }

        /*! \brief Destruktor: Zerstört die vorhandene Instanz der __Item_Name-Klasse__.
         *
         *  Alle Instanzen der Unterelemente werden wieder frei gegeben.
         */
        Item_Name::~Item_Name() {
                Name = "";
        }

        /*! \brief Das Ereignis wird ausgelöst, wenn der Button "Ok" angeklickt wird.
         *
         *  Wurde ein Text eingegeben, so wird er in den Stringvariable _Name_ übernommen und die Variable _isOK_ wird auf _true_ gesetzt.
         *  Andernfalls wird die Stringvariable _Name_ gelöscht und die Variable _isOK_ wird auf _false_ gesetzt.
         */
        void Item_Name::accept3() {
                if (ui.NameEdit->text().length() > 0) {
                        isOK = true;
                        Name = ui.NameEdit->text();
                } else {
                        isOK = false;
                        Name = "";
                }
                QDialog::accept();
        }

        /*! \brief Das Ereignis wird ausgelöst, wenn der Button "Abbrechen" angeklickt wird.
         *
         *  Die Stringvariable _Name_ wird gelöscht und die Variable _isOK_ wird auf _false_ gesetzt.
         */
        void Item_Name::reject3() {
                isOK = false;
                Name = "";
                QDialog::reject();
        }

} // end of namespace SecureDB

