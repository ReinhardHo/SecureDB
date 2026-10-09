/*
MailWork.exe, a mail client concerning the X.400 standard.
Copyright (C) 2024 - 2024    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file languagedialog.cpp
 *  \brief Realisiert die Klasse __languageDialog__.
 *
 *  Die Klasse __languageDialog__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Auswahl einer Spracheinstellung.
 */
#include "languagedialog.h"
#include "ui_languagedialog.h"

namespace SecureDB {

        /*! \brief Konstruktor: Initialisiert eine neue Instanz der Klasse __languageDialog__.
         *
         *  Im Konstruktor werden alle im Dialog benötigten Elemente angelegt.
         */
        languageDialog::languageDialog(QWidget *parent) : QDialog(parent), ui(new Ui::languageDialog) {
                ui->setupUi(this);
                ui->comboBox->clear();
                ui->comboBox->addItem("===================");
                index = 0;
        }

        /*! \brief Destruktor: Zerstört die vorhandene Instanz der Klasse __languageDialog__.
         *
         *  Alle Instanzen der Unterelemente werden wieder frei gegeben.
         */
        languageDialog::~languageDialog() {
                delete ui;
        }

        /*! \brief Diese Methode `accept1()` wird aufgerufen, wenn im Dialog auf den Schaltet _OK_ geklickt wird.
         *
         *  Die Variable _lang_ wird ggf. mit einem neuen Sprachindex geladen.
         */
        void languageDialog::accept1() {
                if (index != lang) {
                        lang = index;
                }
                QDialog::accept();
        }

        /*! \brief Diese Methode `AddLanguages()` wird im Hauptprogramm aufgerufen, wenn der Dialog zur Sprachauswahl angezeigt wird.
         *
         *  Diese Methode lädt Bezeichnungen der bisher vorhandenen Sprachen in die Combo-Box, so dass sie dort angezeigt werden.
         */
        void languageDialog::AddLanguages() {
                if (lang == 1) {
                        ui->comboBox->addItem("English");
                        ui->comboBox->addItem("German");
                }
                if (lang == 2) {
                        ui->comboBox->addItem("Englisch");
                        ui->comboBox->addItem("Deutsch");
                }
        }

        /*! \brief Diese Methode `ChangeIndex()` wird aufgerufen, wenn im Dialog ein Element in der Combo-Box ausgewählt wird.
         *
         *  Die Variable _index_ wird darauf hin mit dem Index des entsprechenden Elementes aus der Combo-Box geladen.
         */
        void languageDialog::ChangeIndex(int value) {
                index = value;
        }

        /*! \brief Diese Methode `reject1()` wird aufgerufen, wenn im Dialog auf den Schaltet _Abbrechen_ geklickt wird.
         *
         *  Der geöffnete Dialog wird darauf hin abgebrochen und geschlossen.
         */
        void languageDialog::reject1() {
                QDialog::reject();
        }

} // end of namespace FileDump
