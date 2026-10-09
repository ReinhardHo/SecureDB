/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file ItemSelect.cpp
 *  \brief Realisiert die Klasse __Item_Selection__.
 *
 *  Die Klasse __Item_Selection__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Auswahl eines Elements aus dem Secure-Store.
 */
#include "ItemSelect.h"
#include "item.h"

namespace SecureDB {

        /*! \brief Konstruktor: Initialisiert eine neue Instanz der Klasse __Item_Selection__.
         *
         *  Im Konstruktor werden alle in dieser Klasse benötigten Elemente angelegt.
         *  \param Table Zeiger auf eine Instanz der Klasse SecureDB::ItemTable zur Veralltung der Elemente im Secure-Store.
         */
        Item_Selection::Item_Selection(ItemTable *Table) {
                ui.setupUi(this);
                connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(accept4()));
                connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(reject4()));
                connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(sort()));
                connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(trash()));
                connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeIndex(int)));
                ui.pushButton_3->setCheckable(true);
                ui.pushButton_4->setCheckable(true);
                Index = 0;
                keyIndex = 0;
                tab = Table;
                selectTrash = false;
                LoadItems();
        }

        /*! \brief Destruktor: Zerstört die vorhandene Instanz der Klasse __Item_Selection__.
         *
         *  Alle Instanzen der Unterelemente werden wieder frei gegeben.
         */
        Item_Selection::~Item_Selection() {
        }

        /*! \brief Diese Methode `accept4()` wird aufgerufen, wenn im Dialog auf den Schaltet _OK_ geklickt wird.
         *
         *  Die Variable _isOK_ wird auf _true_ gesetzt.
         */
        void Item_Selection::accept4() {
                isOK = true;
                QDialog::accept();
        }

        /*! \brief Diese Methode `ChangeIndex()` wird aufgerufen, wenn im Dialog ein Element in der Combo-Box ausgewählt wird.
         *
         *  Die Variable _Index_ wird darauf hin mit dem Store_Index des entsprechenden Elementes geladen.
         */
        void Item_Selection::ChangeIndex(int value) {
                QVariant i = ui.comboBox->currentData();
                Index = i.toLongLong();
        }

        /*! \brief Diese Methode `LoadItems()` lädt ausgewälte Element-Namen in den Dialog.
         *
         *  In Abhängigkeit von der eingestellten Sortierung (Variable _keyIndex_) und der Variable _selectTrash_ wird eine Auswahl der Element-Namen
         *  in den Dialog geladen und zur Anzeige gebracht. Zum Schluss wird noch die Anzahl der geladenen Elemnt-Namen angezeigt.
         */
        void Item_Selection::LoadItems() {
                tab->SetKeyIndex(keyIndex);
                tab->First();
                ui.comboBox->clear();
                while(tab->Row() != nullptr) {
                        if (selectTrash) {
                                if (tab->Row()->deleted == 1) {
                                        QString *s = tab->Row()->getFullName();
                                        size_t i = tab->Row()->StoreIndex;
                                        ui.comboBox->addItem(*s, i);
                                }
                        } else {
                                if (tab->Row()->deleted == 0) {
                                        QString *s = tab->Row()->getFullName();
                                        size_t i = tab->Row()->StoreIndex;
                                        ui.comboBox->addItem(*s, i);
                                }
                        }
                        tab->Next();
                }
                ui.label_2->setNum(ui.comboBox->count());
        }

        /*! \brief Diese Methode `reject4()` wird aufgerufen, wenn im Dialog auf den Schaltet _Abbrechen_ geklickt wird.
         *
         *  Die Variable _isOK_ wird auf _false_ gesetzt.
         */
        void Item_Selection::reject4() {
                isOK = false;
                QDialog::reject();
        }

        /*! \brief Diese Methode `sort()` wird aufgerufen, wenn im Dialog auf den Schaltet _Umsortieren_ geklickt wird.
         *
         *  Die Variable _keyIndex_ wird verändert und die Element-Namen werden in einer anderen Reihenfolge in die Combo-Box des Dialogs geladen.
         */
        void Item_Selection::sort() {
                keyIndex++;
                if (keyIndex > 3) {
                        keyIndex = 0;
                }
                LoadItems();
        }

        /*! \brief Diese Methode `trash()` wird aufgerufen, wenn im Dialog auf den Schaltet _Mülleimer_ geklickt wird.
         *
         *  Es wird zwischen den ungelöschten und den gelöschten Elementen hin und her geschaltet. Die jeweilige Gruppe derElement-Namen
         *  Wird in den Dialog geladen und dort angezeigt.
         */
        void Item_Selection::trash() {
                if (!selectTrash) {
                        selectTrash = true;
                } else {
                        selectTrash = false;
                }
                LoadItems();
        }

} // end of namespace SecureDB

