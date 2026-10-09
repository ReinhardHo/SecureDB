/*
MailWork.exe, a mail client concerning the X.400 standard.
Copyright (C) 2024 - 2024    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


/*! \file languagedialog.h
 *  \brief Definiert die Klasse __languageDialog__.
 *
 *  Die Klasse __languageDialog__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Auswahl einer Spracheinstellung.
 */
#ifndef FILEDUMP_LANGUAGEDIALOG_H
#define FILEDUMP_LANGUAGEDIALOG_H

#include <QDialog>

 namespace Ui {
         class languageDialog;
 }

namespace SecureDB {

        /*! \brief Definiert die Klasse __languageDialog__.
         *
         *  Die Klasse __languageDialog__ ist eine Ableitung von der Qt-Klasse __QDialog__ und dient zur Auswahl einer Spracheinstellung.
         */
        class languageDialog : public QDialog {
                Q_OBJECT
        public:
                explicit languageDialog(QWidget *parent = nullptr);
                ~languageDialog();
                void AddLanguages();
                /*! \brief Enthält den Indexwert für eine Sprachdarstellung.
                 *
                 *  Derzeit sind nur Englisch (Indexwert = 1) und Deutsch (Indexwert = 2) realisiert.
                 */
                int lang;
        private slots:
                void accept1();
                void ChangeIndex(int value);
                void reject1();
        private:
                /*! \brief Graphisches User-Interface.
                 *
                 *  Zeiger auf das graphische User-Interface für diesen languageDialog.
                 */
                Ui::languageDialog *ui;
                /*! \brief Enthält den Indexwert für eine Sprachdarstellung.
                 *
                 *  Derzeit sind nur Englisch (Indexwert = 1) und Deutsch (Indexwert = 2) realisiert.
                 */
                int index;
        };

} // end of namespace FileDump

#endif // FILEDUMP_LANGUAGEDIALOG_H
