/*
SecureDB.exe, a password container.
Copyright (C) 2023 - 2026    Reinhard Hölscher

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*! \file mainwindow.cpp
 *  \brief Realisiert die Klasse __MainWindow__.
 *
 *  Die Klasse MainWindow ist eine Ableitung der Qt-Klasse QMainWindow. Sie definiert alle graphischen Elemente des Hauptfensters und stellt alle Methoden bereit,
 *  die der Benutzer durch die Auswahl von Menüpunkten oder Buttons auslösen kann.
 */
#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QIcon>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include "pwdaten1.h"
#include "pwdaten2.h"
#include <QDateTime>
#include "item.h"
#include "ItemName.h"
#include "ItemSelect.h"
#include "languagedialog.h"

/*! \file Mainwindow.cpp
 *  \brief In dieser Datei wird die Klasse __MainWindow__ (das Hauptprogramm) realisiert.
 */

using namespace RH;

namespace SecureDB {

        /*! \brief Konstruktor: Initialisiert eine neue Instanz der __MainWindow-Klasse__.
         *
         *  Im Konstruktor werden alle im Programm benötigte Instanzen (Menüs, Buttons, usw.) der Elemente der Oberfläche
         *  und sonstige benötigte Elemente angelegt. Die Einstellungen des letzten Programmlaufs (Lage und Größe des Programms auf dem Desktop)
         *  und der letzte benutzte Secure-Store werden geladen und das Programm wird so angezeigt, wie es beim letzten Mal beendet wurde.
         */
        MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
                SecFileName1 = new QString();
                SecFileName2 = new QString();
                SecFileName3 = new QString();
                settings = new QSettings("RH", "SecureDB");
                SecFileName1->append(settings->value("SecFileLocation1").toString());
                SecFileName2->append(settings->value("SecFileLocation2").toString());
                SecFileName3->append(settings->value("SecFileLocation3").toString());
                setGeometry(settings->value("LocationX").toInt(),
                            settings->value("LocationY").toInt(),
                            settings->value("MainWidth").toInt(),
                            settings->value("MainHight").toInt());
                setWindowIcon(QIcon(":d/images/SecureDB.png"));
                setWindowTitle(tr("SecureDB version x.x.x as of yyyy-MM-dd"));
                editor = new QTextEdit(this);
                QFont *newC = new QFont("Courier New ", 12, QFont::Bold);
                editor->setFont(*newC);
                delete newC;
                connect(editor, SIGNAL(textChanged()), this, SLOT(SetBackgroundColor()));
                SetupMenuBar();
                SetupToolBar();
                SetupStatusBar();
                setCentralWidget(editor);
                SetupSecureItems();
                SetBackgroundColor();
                TextFileName = new QString();
                SecItemName = new QString();
                MergeFileName1 = nullptr;
                MergeFileName2 = nullptr;
                MergeFileName3 = nullptr;
                SelectTrash = false;
        }

        /*! \brief Destruktor: Zerstört die vorhandene Instanz der __MainWindow-Klasse__.
         *
         *  Die aktuellen Einstellungen des Programms (Lage, Größe und Secure-Store) werden gespeichert und alle Instanzen der Unterelemente wieder frei gegeben.
         */
        MainWindow::~MainWindow() {
                delete SecItemName;
                delete TextFileName;
                ClearSecureItems();
                QRect r = geometry();
                settings->setValue("LocationX", r.left());
                settings->setValue("LocationY", r.top());
                settings->setValue("MainWidth", r.width());
                settings->setValue("MainHight", r.height());
                delete settings;
                delete editor;
                delete SecFileName3;
                delete SecFileName2;
                delete SecFileName1;
        }

        /*! \brief Die Methode `ClearMergeItems()` zerstört Instanzen von verschiedenen Objekten auf dem Heap die zum Mergen benutzt wurden.
         *
         *  So wird sichergestellt, dass kein sicherheitskritisches Schlüsselmaterial im Heap-Speicher verbleibt.
         */
        void MainWindow::ClearMergeItems() {
                delete MergeFileName1;
                delete MergeFileName2;
                delete MergeFileName3;
                delete Merge1Key1;
                delete Merge1IV1;
                delete Merge1Key2;
                delete Merge1IV2;
                delete Merge2Key1;
                delete Merge2IV1;
                delete Merge2Key2;
                delete Merge2IV2;
                delete Merge3Key1;
                delete Merge3IV1;
                delete Merge3Key2;
                delete Merge3IV2;
                delete MergeFile;
                delete OneTimePadKeyM;
                delete secureM;
                delete EndLabelKeyM;
                delete PermutationKeyM;
        }

        /*! \brief Die Methode `ClearSecureItems()` zerstört Instanzen von verschiedenen Objekten auf dem Heap die zur Verschlüsselung benutzt wurden.
         *
         *  So wird sichergestellt, dass kein sicherheitskritisches Schlüsselmaterial im Heap-Speicher verbleibt.
         */
        void MainWindow::ClearSecureItems() {
                delete ItemTable;
                delete RandomData;
                delete sha384;
                delete sha512;
                delete SecFile;
                delete SecFile1Key1;
                delete SecFile1IV1;
                delete SecFile1Key2;
                delete SecFile1IV2;
                delete SecFile2Key1;
                delete SecFile2IV1;
                delete SecFile2Key2;
                delete SecFile2IV2;
                delete SecFile3Key1;
                delete SecFile3IV1;
                delete SecFile3Key2;
                delete SecFile3IV2;
                delete KeyGenerationArray;
                delete rand;
                delete pbkdf2;
                delete secure;
                delete PermutationKey;
                delete EndLabelKey;
                delete OneTimePadKey;
        }

        /*! \brief Das Ereignis wird (im Secure-Mode) ausgelöst, wenn der Button "Delete" angeklickt wird.
         *
         *  Wurde der Richtext-Eingabebereich verändert, wird der Benutzer zunächst in einem Meldungsfernster gefragt, ob der Text gesichert werden soll.
         *  Bei _nein_ wird der Richtext-Eingabebereich gelöscht und dann fortgesetzt. Bei _ja_ erfogt return.
         *
         *  War ein SecureElement geladen, so wird dieses nun als gelöscht markiert. War es bereits als gelöscht markiert, so wird es endgültig aus dem Secure-Store gelöscht.
         *
         *  Nun wird versucht auf das nächst folgende Element zu positionieren. Funktioniert dies,
         *  so wird das Element geladen, anderfalls wird das Ende der Tabelle angezeigt.
         */
        void MainWindow::DeleteButton_Click() {
                int ret = 0;
                QString elementName;
                if (editor->document()->isModified()) {
                        ret = QMessageBox::warning(this, tr("richtext_editor"), tr("should this text be saved first?"), QMessageBox::Yes | QMessageBox::No);
                        if (ret == QMessageBox::No) {
                                editor->document()->clear();
                                editor->document()->setModified(false);
                                SetBackgroundColor();
                        } else {
                                return;
                        }
                }
                if ((ItemTable->Row() != nullptr) && !SelectFirst) {
                        elementName = *ItemTable->Row()->getName();
                        if (ItemTable->Row()->deleted == 0) {
                                ItemTable->Row()->deleted = 1;
                        } else {
                                size_t error = 0;
                                error = SecFile->Delete(ItemTable->Row()->StoreIndex);
                                if (error > 0) {
                                        sLabel1->setText(tr("error: (%1)").arg(3));
                                        sLabel2->setText(tr("delete-error securestore!"));
                                        return;
                                }
                                ItemTable->Delete(SetAfter);
                        }
                        SaveItemTable();
                        ItemTable->SearchRow()->setName(elementName);
                        ItemTable->SeekNearest(true);
                        while (ItemTable->Row() != nullptr) {
                                if (SelectTrash){
                                        if (ItemTable->Row()->deleted == 1) {
                                                break;
                                        }
                                } else{
                                        if (ItemTable->Row()->deleted == 0) {
                                                break;
                                        }
                                }
                                ItemTable->Next();
                        }
                        if (ItemTable->Row() != nullptr) {
                                if (UnsecureItem(ItemTable->Row()->StoreIndex)) {
                                        sLabel2->setText(*ItemTable->Row()->getFullName());
                                        SecItemName->clear();
                                        SecItemName->append(*ItemTable->Row()->getName());
                                } else {
                                        QMessageBox::critical(this, tr("secure_item_name"),
                                                              tr("a secureitem with the name \"%1\" could not be loaded without errors!").arg(*ItemTable->Row()->getName()),
                                                              QMessageBox::Ok);
                                }
                        } else {
                                SecItemName->clear();
                                editor->document()->clear();
                                editor->document()->setModified(false);
                                sLabel2->setText("");
                                sLabel3->setText("");
                                sLabel4->setText("");
                                SetBackgroundColor();
                                QMessageBox::critical(this, tr("secure_item_name"), tr("the bottom of the table was reached!"), QMessageBox::Ok);
                        }
                }
        }

        /*! \brief Löscht Hilfsdaten die zur Schlüsselgenerierung benötigt wurden.
         *
         *  So wird sichergestellt, dass kein sicherheitskritisches Schlüsselmaterial im Heap-Speicher verbleibt.
         */
        void MainWindow::DeleteKeyGenerationData1() {
                RandomData->Clear();
                StartIndex = 0;
                Spread = 0;
                SecFile1Key1->Clear();
                SecFile1IV1->Clear();
                SecFile1Key2->Clear();
                SecFile1IV2->Clear();
                SecFile2Key1->Clear();
                SecFile2IV1->Clear();
                SecFile2Key2->Clear();
                SecFile2IV2->Clear();
                SecFile3Key1->Clear();
                SecFile3IV1->Clear();
                SecFile3Key2->Clear();
                SecFile3IV2->Clear();
                KeyGenerationArray->Clear();
                OneTimePadKey->Clear();
                EndLabelKey->Clear();
                PermutationKey->Clear();
        }

        /*! \brief Löscht Hilfsdaten die zur Schlüsselgenerierung benötigt wurden.
         *
         *  So wird sichergestellt, dass kein sicherheitskritisches Schlüsselmaterial im Heap-Speicher verbleibt.
         */
        void MainWindow::DeleteKeyGenerationData2() {
                RandomData->Clear();
                StartIndex = 0;
                Spread = 0;
                KeyGenerationArray->Clear();
        }

        /*! \brief Dieses Ereignis tritt ein wenn der Benutzer den Button __Benden__ in der Tool-Bar drückt oder im Menü __Datei->Beenden__ anklickt.
         *
         *  Wurde der Richtext-Eingabebereich verändert, wird der Benutzer zunächst in einem Meldungsfernster gefragt, ob der Text gesichert werden soll.
         *  Wenn nein, wird das Hauptfenster und damit das Programm beendet.
         */
        void MainWindow::ExitButton_Click() {
                int ret = 0;
                if (editor->document()->isModified()) {
                        ret = QMessageBox::warning(this, tr("richtext_editor"), tr("should this text be saved first?"),
                                                   QMessageBox::Yes | QMessageBox::No);
                        if (ret == QMessageBox::No) {
                                editor->document()->clear();
                                close();
                        }
                } else {
                        editor->document()->clear();
                        close();
                }
        }

        /*! \brief Die Methode `FindByteDown()` sucht das übergebene Byte in den Zufallsdaten zur Schlüsselgenerierung.
         *
         *  Die Methode sucht ab dem übergebenen Index (oder dem Datenfeldende) einen Bytewert in Richtung Datenfeldanfang. Wird der Datenfeldanfang erreicht, ohne dass der Bytewert
         *  gefunden wurde, wird 0 zurück gegeben. Ansonsten wird der Differenzindex (Auffindeposition minus übergebener Index) als negativer Integerwert zurück gegeben.
         *
         *  \param index Index in den Zufallsdaten, ab dem gesucht werden soll.
         *  \param c Zu suchenden Bytewert.
         *  \return Der negative Byteabstand in Richtung Datenfeldanfang (oder 0 im Fehlerfalle).
         */
        int MainWindow::FindByteDown(int index, unsigned char c) {
                int i = index;
                if (i > static_cast<int>(RandomDataLength)) {
                        i = static_cast<int>(RandomDataLength);
                }
                bool searchflag = true;
                while (searchflag) {
                        i--;
                        if (i < 1) {
                                return 0;
                        }
                        unsigned char cin = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(i)]);
                        if (cin == c) {
                                searchflag = false;
                        }
                }
                return (i - index);
        }

        /*! \brief Die Methode `FindByteUp()` sucht das übergebene Byte in den Zufallsdaten zur Schlüsselgenerierung.
         *
         *  Die Methode sucht ab dem übergebenen Index (oder dem Datenfeldanfang) einen Bytewert in Richtung Datenfeldende. Wird das Datenfeldende erreicht, ohne dass der Bytewert
         *  gefunden wurde, wird -1 zurück gegeben. Ansonsten wird der Differenzindex (Auffindeposition minus übergebener Index) als positiver Integerwert zurück gegeben.
         *
         *  \param index Index in der Zufallsdaten, ab dem gesucht werden soll.
         *  \param c Zu suchenden Bytewert.
         *  \return Der positive Byteabstand in Richtung Datenfeldende (oder -1 im Fehlerfalle).
         */
        int MainWindow::FindByteUp(int index, unsigned char c) {
                int i = index;
                if (i < 0) {
                        i = -1;
                }
                bool searchflag = true;
                while (searchflag) {
                        i++;
                        if (i >= static_cast<int>(RandomDataLength)) {
                                return -1;
                        }
                        unsigned char cin = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(i)]);
                        if (cin == c) {
                                searchflag = false;
                        }
                }
                return (i - index);
        }

        /*! \brief Die Methode `IsByteInArray()` sucht das übergebene Byte in einem Array.
         *
         *  Die Methode sucht ab dem Index 0 einen Bytewert in einem Array. Dabei gibt es maximal __Count__ Versuche. Wird der gesuchte Bytewert gefunden, wird die Suche sofort abgebrochen
         *  und _true_ zurück gegeben. Wird der gesuchte Bytewert nicht gefunden, so wird _false_ zurück gegeben.
         *
         *  \param ref Referenz auf eine ByteArray-Instanz.
         *  \param c Zu suchenden Bytewert.
         *  \param count Anzahl der Suchversuche.
         *  \return _true_ bei einem Treffer, ansonsten _false_.
         */
        bool MainWindow::IsByteInArray(ByteArray &ref, char c, size_t count) {
                for (size_t i = 0; i < count; i++) {
                        if (ref[i] == c) {
                                return true;
                        }
                }
                return false;
        }

        /*! \brief Die Methode `languages_Clicked()` zeigt einen Dialog zur Sprachauswahl für die Programmoberfläche.
         *
         * Diese Methode wird aufgerufen, wenn der Benutzer den Menüpunkt: _Hilfe->Sprachauswahl ..._ anklickt.
         * Daraufhin wird ein Dialog angezeigt, in dem der Benutzer eine der vorhandenen Sprachen für die Programmoberfläche auswählen kann.
         * Hat der Benutzer die Sprachauswahl für die Programmoberfläche geändert, so wird dies in den Programmeinstellungen vermerkt und
         * das Programm beendet. Wurde die Sprachauswahl vom Benutzer nicht verändert, so wird das Programm normal fortgesetzt.
         */
        void MainWindow::languages_Clicked() {
                int l = settings->value("Language").toInt();
                languageDialog* ld = new languageDialog(this);
                ld->lang = l;
                ld->AddLanguages();
                ld->exec();
                if ((ld->lang > 0) && (ld->lang != l)) {
                        l = ld->lang;
                        delete ld;
                        settings->setValue("Language", l);
                        ExitButton_Click();
                } else {
                        delete ld;
                }
        }

        /*! \brief Die Methode `LoadItemTable()` lädt ein Element aus dem Secure-Store.
         *
         *  Das Element ist unter dem Index __258__ im Secure-Store hinterlegt. Es handelt sich dabei um eine Liste aller im Secure-Store
         *  hunterlegten Elemente (egal ob zum Löschen markiert oder nicht). Diese Daten werden die Instanz einer Tabelle vom
         *  Typ __SecItemTable__ geladen und dort verwaltet.
         */
        void MainWindow::LoadItemTable() {
                size_t error = 0;
                ByteArray a(8192);
                ItemTable->ClearAll();
                SecFile->Get(a, 258, error);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(14));
                        sLabel2->setText(tr("read-error securestore!"));
                        return;
                }
                ItemTable->ImportFromByteArray(a, true);
        }

        /*! \brief Die Methode `NewStore_Click()` erzeugt einen nenen (leeren) Secure-Store auf einem Laufwerk.
         *
         * Diese Metode wird ausgeführt, wenn der Benutzer im Menü __Extras->Neuen Secure-Store anlegen ...__ anklickt. Bei Erfolg werden drei Securedateien
         * (mit den Endungen *.rnd, *.key und +.bin) angelegt und benötigten Schlüssel vom Zufallszahlengenerator erzeugt.
         */
        void MainWindow::NewStore_Click() {
                QString s;
                s.append(*SecFileName1);
                StoreSelect_Click(true);
                if (s.compare(*SecFileName1) != 0) {
                        QString *s1 = new QString();
                        s1->append("1.048.576 DatenBytes");
                        sLabel3->setText(*s1);
                        delete s1;
                        rand->GetRandomBytes(*RandomData, 1048576);
                        RandomDataLength = RandomData->Size();
                        size_t error = 0;
                        sLabel3->setText("0 Byte");
                        int count1;
                        int count2;
                        PWDaten2 pw2;
                        pw2.exec();
                        if (pw2.isOK) {
                                StartIndex = pw2.si1->ToInteger();
                                if (StartIndex == -1) {
                                        pw2.isOK = false;
                                }
                                if (StartIndex < 1) {
                                        pw2.isOK = false;
                                }
                                if (StartIndex > 900000) {
                                        pw2.isOK = false;
                                }
                                Spread = pw2.di1->ToInteger();
                                if (Spread == -1) {
                                        pw2.isOK = false;
                                }
                                if (Spread < 1) {
                                        pw2.isOK = false;
                                }
                                if (Spread > 250000) {
                                        pw2.isOK = false;
                                }
                                count1 = pw2.hr1->ToInteger();
                                pw2.hr1->Clear();
                                if (count1 == -1) {
                                        pw2.isOK = false;
                                }
                                if (count1 < 10) {
                                        pw2.isOK = false;
                                }
                                count2 = pw2.hr2->ToInteger();
                                pw2.hr2->Clear();
                                if (count2 == -1) {
                                        pw2.isOK = false;
                                }
                                if (count2 < 10) {
                                        pw2.isOK = false;
                                }
                                pw2.si1->Clear();
                                pw2.di1->Clear();
                        }
                        if (!pw2.isOK) {
                                sLabel1->setText(tr("error: (%1)").arg(4));
                                sLabel2->setText(tr("the data entered are not plausible!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        pbkdf2->SetIteration(count1);
                        pbkdf2->SetPW(*(pw2.pw1));
                        ByteArray *salt1 = new ByteArray(64);
                        salt1->Copy(Salt1, 0, 0, 64);
                        pbkdf2->SetSalt(*salt1);
                        ByteArray *ivKey = pbkdf2->GetKeys();
                        if (ivKey != nullptr) {
                                SecFile1IV1->Copy(*ivKey, 0, 0, 16);
                                SecFile1Key1->Copy(*ivKey, 16, 0, 32);
                        }
                        salt1->Copy(Salt2, 0, 0, 64);
                        pbkdf2->SetSalt(*salt1);
                        ivKey = nullptr;
                        ivKey = pbkdf2->GetKeys();
                        if (ivKey != nullptr) {
                                SecFile1IV2->Copy(*ivKey, 0, 0, 16);
                                SecFile1Key2->Copy(*ivKey, 16, 0, 32);
                        }
                        error = SecFile->NewStore(SecFileName1, SecFile1IV1, SecFile1Key1, SecFile1IV2, SecFile1Key2);
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(5));
                                sLabel2->setText(tr("create-error securestore!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        error = SecFile->Open(SecFileName1, SecFile1IV1, SecFile1Key1, SecFile1IV2, SecFile1Key2);
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(8));
                                sLabel2->setText(tr("open-error securestore!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        SecFile->Put(*RandomData, 260, error);
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(9));
                                sLabel2->setText(tr("write-error securestore!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        error = SecFile->Close();
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(10));
                                sLabel2->setText(tr("close-error securestore!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        pbkdf2->SetIteration(count2);
                        pbkdf2->SetPW(*(pw2.pw2));
                        salt1->Copy(Salt1, 0, 0, 64);
                        pbkdf2->SetSalt(*salt1);
                        ivKey = pbkdf2->GetKeys();
                        if (ivKey != nullptr) {
                                SecFile2IV1->Copy(*ivKey, 0, 0, 16);
                                SecFile2Key1->Copy(*ivKey, 16, 0, 32);
                        }
                        salt1->Copy(Salt2, 0, 0, 64);
                        pbkdf2->SetSalt(*salt1);
                        ivKey = nullptr;
                        ivKey = pbkdf2->GetKeys();
                        if (ivKey != nullptr) {
                                SecFile2IV2->Copy(*ivKey, 0, 0, 16);
                                SecFile2Key2->Copy(*ivKey, 16, 0, 32);
                        }
                        delete salt1;
                        error = SecFile->NewStore(SecFileName2, SecFile2IV1, SecFile2Key1, SecFile2IV2, SecFile2Key2);
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(6));
                                sLabel2->setText(tr("create-error securestore!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        ByteArray hash(64, true);
                        sha512->GetHash(*RandomData, hash);
                        KeyGenerationArray->Copy(hash, 0, 0, 64);
                        sha384->GetHash(*RandomData, hash);
                        KeyGenerationArray->Copy(hash, 0, 64, 48);
                        rand->GetRandomBytes(*OneTimePadKey, size);
                        bool searchup = true;
                        size_t writeindex = 112;
                        int index = StartIndex + Spread;
                        int bytegab = 0;
                        for (size_t i = 0; i < size; i++) {
                                unsigned char c = static_cast<unsigned char>((*OneTimePadKey)[i]);
                                if (searchup) {
                                        bytegab = FindByteUp(index, c);
                                        if (bytegab == -1) {
                                                searchup = false;
                                                bytegab = FindByteDown(index, c);
                                        }
                                } else {
                                        bytegab = FindByteDown(index, c);
                                        if (bytegab == 0) {
                                                searchup = true;
                                                bytegab = FindByteUp(index, c);
                                        }
                                }
                                KeyGenerationArray->WriteX209Int64(writeindex, bytegab);
                                if (bytegab > 0) {
                                        index = index + c + bytegab + Spread;
                                } else {
                                        index = index + c + bytegab - Spread;
                                }
                        }
                        bool ready = false;
                        while (!ready) {
                                rand->GetRandomBytes(*OneTimePadKey, size);
                                EndLabelKey->Clear();
                                size_t keyindex = 0;
                                for (size_t i = 0; i < size; i++) {
                                        char c = (*OneTimePadKey)[i];
                                        if (!IsByteInArray(*EndLabelKey, c, keyindex)) {
                                                EndLabelKey->Append(c);
                                                keyindex++;
                                        }
                                        if (keyindex == 256) {
                                                ready = true;
                                                break;
                                        }
                                }
                        }
                        for (size_t i = 0; i < 256; i++) {
                                unsigned char c = static_cast<unsigned char>((*EndLabelKey)[i]);
                                if (searchup) {
                                        bytegab = FindByteUp(index, c);
                                        if (bytegab == -1) {
                                                searchup = false;
                                                bytegab = FindByteDown(index, c);
                                        }
                                } else {
                                        bytegab = FindByteDown(index, c);
                                        if (bytegab == 0) {
                                                searchup = true;
                                                bytegab = FindByteUp(index, c);
                                        }
                                }
                                KeyGenerationArray->WriteX209Int64(writeindex, bytegab);
                                if (bytegab > 0) {
                                        index = index + c + bytegab + Spread;
                                } else {
                                        index = index + c + bytegab - Spread;
                                }
                        }
                        ready = false;
                        while (!ready) {
                                rand->GetRandomBytes(*OneTimePadKey, size);
                                PermutationKey->Clear();
                                size_t keyindex = 0;
                                for (size_t i = 0; i < size; i++) {
                                        char c = (*OneTimePadKey)[i];
                                        if (!IsByteInArray(*PermutationKey, c, keyindex)) {
                                                PermutationKey->Append(c);
                                                keyindex++;
                                        }
                                        if (keyindex == 256) {
                                                ready = true;
                                                break;
                                        }
                                }
                        }
                        for (size_t i = 0; i < 256; i++) {
                                unsigned char c = static_cast<unsigned char>((*PermutationKey)[i]);
                                if (searchup) {
                                        bytegab = FindByteUp(index, c);
                                        if (bytegab == -1) {
                                                searchup = false;
                                                bytegab = FindByteDown(index, c);
                                        }
                                } else {
                                        bytegab = FindByteDown(index, c);
                                        if (bytegab == 0) {
                                                searchup = true;
                                                bytegab = FindByteUp(index, c);
                                        }
                                }
                                KeyGenerationArray->WriteX209Int64(writeindex, bytegab);
                                if (bytegab > 0) {
                                        index = index + c + bytegab + Spread;
                                } else {
                                        index = index + c + bytegab - Spread;
                                }
                        }
                        rand->GetRandomBytes(*OneTimePadKey, 48);
                        SecFile3Key1->SetSize(32);
                        SecFile3Key1->Copy(*OneTimePadKey, 0, 0, 32);
                        for (size_t i = 0; i < 32; i++) {
                                unsigned char c = static_cast<unsigned char>((*SecFile3Key1)[i]);
                                if (searchup) {
                                        bytegab = FindByteUp(index, c);
                                        if (bytegab == -1) {
                                                searchup = false;
                                                bytegab = FindByteDown(index, c);
                                        }
                                } else {
                                        bytegab = FindByteDown(index, c);
                                        if (bytegab == 0) {
                                                searchup = true;
                                                bytegab = FindByteUp(index, c);
                                        }
                                }
                                KeyGenerationArray->WriteX209Int64(writeindex, bytegab);
                                if (bytegab > 0) {
                                        index = index + c + bytegab + Spread;
                                } else {
                                        index = index + c + bytegab - Spread;
                                }
                        }
                        SecFile3IV1->SetSize(16);
                        SecFile3IV1->Copy(*OneTimePadKey, 32, 0, 16);
                        for (size_t i = 0; i < 16; i++) {
                                unsigned char c = static_cast<unsigned char>((*SecFile3IV1)[i]);
                                if (searchup) {
                                        bytegab = FindByteUp(index, c);
                                        if (bytegab == -1) {
                                                searchup = false;
                                                bytegab = FindByteDown(index, c);
                                        }
                                } else {
                                        bytegab = FindByteDown(index, c);
                                        if (bytegab == 0) {
                                                searchup = true;
                                                bytegab = FindByteUp(index, c);
                                        }
                                }
                                KeyGenerationArray->WriteX209Int64(writeindex, bytegab);
                                if (bytegab > 0) {
                                        index = index + c + bytegab + Spread;
                                } else {
                                        index = index + c + bytegab - Spread;
                                }
                        }
                        rand->GetRandomBytes(*OneTimePadKey, 48);
                        SecFile3Key2->SetSize(32);
                        SecFile3Key2->Copy(*OneTimePadKey, 0, 0, 32);
                        for (size_t i = 0; i < 32; i++) {
                                unsigned char c = static_cast<unsigned char>((*SecFile3Key2)[i]);
                                if (searchup) {
                                        bytegab = FindByteUp(index, c);
                                        if (bytegab == -1) {
                                                searchup = false;
                                                bytegab = FindByteDown(index, c);
                                        }
                                } else {
                                        bytegab = FindByteDown(index, c);
                                        if (bytegab == 0) {
                                                searchup = true;
                                                bytegab = FindByteUp(index, c);
                                        }
                                }
                                KeyGenerationArray->WriteX209Int64(writeindex, bytegab);
                                if (bytegab > 0) {
                                        index = index + c + bytegab + Spread;
                                } else {
                                        index = index + c + bytegab - Spread;
                                }
                        }
                        SecFile3IV2->SetSize(16);
                        SecFile3IV2->Copy(*OneTimePadKey, 32, 0, 16);
                        for (size_t i = 0; i < 16; i++) {
                                unsigned char c = static_cast<unsigned char>((*SecFile3IV2)[i]);
                                if (searchup) {
                                        bytegab = FindByteUp(index, c);
                                        if (bytegab == -1) {
                                                searchup = false;
                                                bytegab = FindByteDown(index, c);
                                        }
                                } else {
                                        bytegab = FindByteDown(index, c);
                                        if (bytegab == 0) {
                                                searchup = true;
                                                bytegab = FindByteUp(index, c);
                                        }
                                }
                                KeyGenerationArray->WriteX209Int64(writeindex, bytegab);
                                if (bytegab > 0) {
                                        index = index + c + bytegab + Spread;
                                } else {
                                        index = index + c + bytegab - Spread;
                                }
                        }
                        error = SecFile->Open(SecFileName2, SecFile2IV1, SecFile2Key1, SecFile2IV2, SecFile2Key2);
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(11));
                                sLabel2->setText(tr("open-error securestore!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        SecFile->Put(*KeyGenerationArray, 270, error);
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(12));
                                sLabel2->setText(tr("write-error securestore!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        error = SecFile->Close();
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(13));
                                sLabel2->setText(tr("close-error securestore!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        error = SecFile->NewStore(SecFileName3, SecFile3IV1, SecFile3Key1, SecFile3IV2, SecFile3Key2);
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(7));
                                sLabel2->setText(tr("create-error securestore!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        secure->SetOneTimePadKey(*OneTimePadKey);
                        secure->SetLabelKey(*EndLabelKey);
                        secure->SetPermutationKey(*PermutationKey);
                        error = SecFile->Open(SecFileName3, SecFile3IV1, SecFile3Key1, SecFile3IV2, SecFile3Key2);
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(11));
                                sLabel2->setText(tr("open-error securestore!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        ItemTable->ClearAll();
                        SaveItemTable();
                        error = SecFile->Close();
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(13));
                                sLabel2->setText(tr("close-error securestore!"));
                                DeleteKeyGenerationData1();
                                return;
                        }
                        DeleteKeyGenerationData1();
                        sLabel1->setText(tr("message:"));
                        sLabel2->setText(tr("generation successfully completed!"));
                } else {
                        sLabel1->setText(tr("message:"));
                        sLabel2->setText(tr("securestore already exists!"));
                }
        }

        /*! \brief Das Ereignis wird ausgelöst, wenn (im Secure-Mode) der Button __Next__ angeklickt wird.
         *
         *  Wurde der Eingabebereich verändert, wird der Benutzer zunächst in einem Meldungsfernster gefragt, ob der Text gesichert werden soll.
         *  Bei _nein_ wird der Eingabebereich gelöscht und dann fortgesetzt. Bei _ja_ erfogt return.
         *
         *  In der Elemente-Tabelle wird versucht auf das nächste Element zu positionieren. Ist dies vorhanden wird es geladen und angezeigt. Ist kein Element mehr
         *  vorhanden wird das Ende der Tabelle angezeigt.
         */
        void MainWindow::NextButton_Click() {
                int ret = 0;
                if (editor->document()->isModified()) {
                        ret = QMessageBox::warning(this, tr("richtext_editor"), tr("should this text be saved first?"), QMessageBox::Yes | QMessageBox::No);
                        if (ret == QMessageBox::No) {
                                editor->document()->clear();
                                editor->document()->setModified(false);
                                SetBackgroundColor();
                        } else {
                                return;
                        }
                }
                if (SelectFirst) {
                        ItemTable->First();
                } else {
                        ItemTable->Next();
                }
                while (ItemTable->Row() != nullptr) {
                        SelectFirst = false;
                        if (SelectTrash){
                                if (ItemTable->Row()->deleted == 1) {
                                        break;
                                }
                        } else{
                                if (ItemTable->Row()->deleted == 0) {
                                        break;
                                }
                        }
                        ItemTable->Next();
                }
                if (ItemTable->Row() != nullptr) {
                        if (UnsecureItem(ItemTable->Row()->StoreIndex)) {
                                sLabel2->setText(*ItemTable->Row()->getFullName());
                                SecItemName->clear();
                                SecItemName->append(*ItemTable->Row()->getName());
                        } else {
                                QMessageBox::critical(this, tr("secure_item_name"),
                                                      tr("a secureitem with the name \"%1\" could not be loaded without errors!").arg(*ItemTable->Row()->getName()),
                                                      QMessageBox::Ok);
                        }
                } else {
                        QMessageBox::critical(this, tr("secure_item_name"), tr("the bottom of the table was reached!"), QMessageBox::Ok);
                }
        }

        /*! \brief Dieses Ereignis tritt ein wenn der Benutzer den Button __Öffnen__ in der ToolBar drückt oder im Menü __Datei->Öffnen ...__ anklickt.
         *
         *  Wurde der Eingabebereich verändert, wird der Benutzer zunächst in einem Meldungsfernster gefragt, ob der Text gesichert werden soll.
         *  Bei _nein_ wird der Eingabebereich gelöscht und dann fortgesetzt. Bei _ja_ erfogt return.
         *
         *  Im SecureMode wird ein Dialog geöffnet, mit dem der Benutzer ein Element aus dem Secure-Store auswählen und laden kann.
         *
         *  Ohne SecureMode wird der FileOpenDialog angezeigt und der Benutzer kann eine Datei zum Einlesen auswählen.
         */
        void MainWindow::OpenButton_Click() {
                int ret = 0;
                if (editor->document()->isModified()) {
                        ret = QMessageBox::warning(this, tr("richtext_editor"), tr("should this text be saved first?"), QMessageBox::Yes | QMessageBox::No);
                        if (ret == QMessageBox::No) {
                                editor->document()->clear();
                                editor->document()->setModified(false);
                                SetBackgroundColor();
                        }
                }
                if (SecureMode) {
                        Item_Selection sis(ItemTable);
                        sis.exec();
                        if (sis.isOK) {
                                SelectTrash = sis.selectTrash;
                                ItemTable->First();
                                while (ItemTable->Row() != nullptr) {
                                        if (ItemTable->Row()->StoreIndex == sis.Index) {
                                                break;
                                        }
                                        ItemTable->Next();
                                }
                                if (ItemTable->Row() != nullptr) {
                                        SelectFirst = false;
                                        if (UnsecureItem(ItemTable->Row()->StoreIndex)) {
                                                sLabel2->setText(*ItemTable->Row()->getFullName());
                                                SecItemName->clear();
                                                SecItemName->append(*ItemTable->Row()->getName());
                                        } else {
                                                QMessageBox::critical(this, tr("secure_item_name"),
                                                                      tr("a secureitem with the name \"%1\" could not be loaded without errors!").arg(*ItemTable->Row()->getName()),
                                                                      QMessageBox::Ok);
                                        }
                                }
                        }
                } else {
                        TextFileName->clear();
                        TextFileName->append(QFileDialog::getOpenFileName(this, tr("openfile"), QDir::homePath(), tr("all files (*.*)")));
                        if (!TextFileName->isNull()) {
                                sLabel2->setText(*TextFileName);
                                QFileInfo fi(*TextFileName);
                                size_t length = static_cast<size_t>(fi.size());
                                ByteArray b;
                                size_t index = 0;
                                b.IntegerToText(index, static_cast<int>(length), 15, true);
                                QString *s1 = b.ToQString();
                                s1->append(" Bytes");
                                sLabel3->setText(*s1);
                                delete s1;
                                QFile fn(*TextFileName);
                                if (fn.open(QIODevice::ReadOnly | QIODevice::Text)) {
                                        editor->setPlainText(fn.readAll());
                                        editor->document()->setModified(false);
                                        fn.close();
                                        SetBackgroundColor();
                                }
                        }
                }
        }

        /*! \brief Das Ereignis wird ausgelöst, wenn der Button __Prior__ angeklickt wird.
         *
         *  Wurde der Eingabebereich verändert, wird der Benutzer zunächst in einem Meldungsfernster gefragt, ob der Text gesichert werden soll.
         *  Bei _nein_ wird der Eingabebereich gelöscht und dann fortgesetzt. Bei _ja_ erfogt return.
         *
         *  In der Elemente-Tabelle wird versucht auf das vorherige Element zu positionieren. Ist dies vorhanden wird es geladen und angezeigt. Ist kein Element mehr
         *  vorhanden wird der Anfang der Tabelle angezeigt.
         */
        void MainWindow::PriorButton_Click() {
                int ret = 0;
                if (editor->document()->isModified()) {
                        ret = QMessageBox::warning(this, tr("richtext_editor"), tr("should this text be saved first?"), QMessageBox::Yes | QMessageBox::No);
                        if (ret == QMessageBox::No) {
                                editor->document()->clear();
                                editor->document()->setModified(false);
                                SetBackgroundColor();
                        } else {
                                return;
                        }
                }
                if (SelectFirst) {
                        ItemTable->Last();
                } else {
                        ItemTable->Prior();
                }
                while (ItemTable->Row() != nullptr) {
                        SelectFirst = false;
                        if (SelectTrash){
                                if (ItemTable->Row()->deleted == 1) {
                                        break;
                                }
                        } else{
                                if (ItemTable->Row()->deleted == 0) {
                                        break;
                                }
                        }
                        ItemTable->Prior();
                }
                if (ItemTable->Row() != nullptr) {
                        if (UnsecureItem(ItemTable->Row()->StoreIndex)) {
                                sLabel2->setText(*ItemTable->Row()->getFullName());
                                SecItemName->clear();
                                SecItemName->append(*ItemTable->Row()->getName());
                        } else {
                                QMessageBox::critical(this, tr("secure_item_name"),
                                                      tr("a secureitem with the name \"%1\" could not be loaded without errors!").arg(*ItemTable->Row()->getName()),
                                                      QMessageBox::Ok);
                        }
                } else {
                        QMessageBox::critical(this, tr("secure_item_name"), tr("the top of the table was reached!"), QMessageBox::Ok);
                }
        }

        /*! \brief Dieses Ereignis tritt ein wenn der Benutzer den Button __RedWhite__ in der ToolBar drückt.
         *
         *  War das Programm im SecureMode wird der SecureStore geschlossen und der SecureMode verlassen. Die Button __Next__, __Prior__ und __Delete__ werden unsichtbar.
         *  Andernfalls wird versucht in den SecureMode zu wechseln (der Benutzer muss seine Passwort-Daten eingeben). Bei Erfolg werden die Button __Next__, __Prior__ und __Delete__
         *  angezeigt und eine Liste der Secure-Elemente aus dem Store geladen.
         */
        void MainWindow::RedWhite_Click() {
                sLabel1->setText("");
                sLabel2->setText("");
                if (SecureMode) {
                        SecureMode = false;
                        SelectFirst = false;
                        actNext->setVisible(false);
                        actPrior->setVisible(false);
                        actDelete->setVisible(false);
                        actSelectStore->setVisible(true);
                        actMergeStore->setVisible(false);
                        actMergeStoreTrash->setVisible(false);
                        editor->document()->clear();
                        editor->document()->setModified(false);
                        SetBackgroundColor();
                        OneTimePadKey->Clear();
                        EndLabelKey->Clear();
                        PermutationKey->Clear();
                        size_t error = 0;
                        error = SecFile->Close();
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(15));
                                sLabel2->setText(tr("close-error secure store!"));
                                return;
                        }
                        sLabel1->setText(tr("unsecure"));
                        sLabel2->setText("");
                } else {
                        if (StartSecureMode()) {
                                SecureMode = true;
                                SelectFirst = true;
                                actNext->setVisible(true);
                                actPrior->setVisible(true);
                                actDelete->setVisible(true);
                                actSelectStore->setVisible(false);
                                actMergeStore->setVisible(true);
                                actMergeStoreTrash->setVisible(true);
                                LoadItemTable();
                                sLabel1->setText(tr("secure"));
                                sLabel2->setText("");
                        }
                }
                sLabel3->setText(" ");
                sLabel4->setText(" ");
                SetBackgroundColor();
        }

        /*! \brief Das Ereignis wird ausgelöst, wenn im Menü __Datei->Speichern unter ...__ angeklickt wird.
         *
         *  Im SecureMode wird dem Benutzer ein Dialogfeld angezeit, in den er einen Elemente-Namen eingeben kann. Die Eingabe wird in die Variable _SecItemName_ übernommen.
         *
         *  Ohne Securemode wird ein SaveFileDialog angezeigt, indem der Benutzer einen Namen auswählen oder eingeben kann. Der Name wird in die Variable _TextFileName_ übernommen.
         */
        void MainWindow::SaveAs_Click() {
                if (SecureMode) {
                        Item_Name sin;
                        sin.exec();
                        if (sin.isOK) {
                                SecItemName->clear();
                                SecItemName->append(sin.Name);
                        } else {
                                return;
                        }
                        SaveButton_Click();
                } else {
                        TextFileName->clear();
                        TextFileName->append(QFileDialog::getSaveFileName(this, tr("filesave"), QDir::homePath(), tr("all files (*.*)")));
                        if (!TextFileName->isNull()) {
                                SaveButton_Click();
                        }
                }
        }

        /*! \brief Das Ereignis wird ausgelöst, wenn im Menü __Datei->Speichern__ angeklickt oder der Button __Speichern__ in der ToolBar gedrückt wird.
         *
         *  Ist im SecureMode kein Element-Name vorhanden wird zunächst die Methode `SaveAs_Click()` aufgerufen (vgl. dort). Wird dadurch ein Element-Name verfügbar und wurde der
         *  Eingabebereich verändert, so wird das Element im Secure-Store gespeichert. Ist im Secure-Mode ein Elemnet-Name vorhanden wird das Element gespeichert wenn der Eingabebereich
         *  verändert wurde. Anderfalls erscheint ein Meldungsfenster mit einem Hinweistext.
         *
         *  Ist ohne Secure-Mode kein Dateiname vorhanden wird zunächst die Methode `SaveAs_Click()` aufgerufen (vgl. dort). Wird dadurch ein Dateiname verfügbar und wurde der
         *  Eingabebereich verändert, so wird die Datei gespeichert. Ist ohne Secure-Mode ein Dateiname vorhanden wird die Datei gespeichert wenn der Eingabebereich
         *  verändert wurde. Anderfalls erscheint ein Meldungsfenster mit einem Hinweistext.
         */
        void MainWindow::SaveButton_Click() {
                if (SecureMode) {
                        if (SecItemName->isNull()) {
                                SaveAs_Click();
                                return;
                        }
                        if (!SecItemName->isNull()) {
                                if (editor->document()->isModified()) {
                                        SecureItem(*SecItemName);
                                } else {
                                        int ret = 0;
                                        ret = QMessageBox::warning(this, tr("richtext_editor"),
                                                                   tr("the text has not been changed!\r\nshould this text really be saved?"),
                                                                   QMessageBox::Yes | QMessageBox::No);
                                        if (ret == QMessageBox::Yes) {
                                                SecureItem(*SecItemName);
                                        }
                                }
                        }
                } else {
                        if (TextFileName->isNull()) {
                                SaveAs_Click();
                                return;
                        } else {
                                if (!TextFileName->isNull()) {
                                        if (editor->document()->isModified()) {
                                                QFile fn(*TextFileName);
                                                if (fn.open(QIODevice::WriteOnly | QIODevice::Text)) {
                                                        fn.write(editor->toPlainText().toUtf8());
                                                        sLabel2->setText(*TextFileName);
                                                        fn.close();
                                                        QFileInfo fi(*TextFileName);
                                                        size_t length = static_cast<size_t>(fi.size());
                                                        ByteArray b;
                                                        size_t index = 0;
                                                        b.IntegerToText(index, static_cast<int>(length), 15, true);
                                                        QString *s1 = b.ToQString();
                                                        s1->append(" Bytes");
                                                        sLabel3->setText(*s1);
                                                        delete s1;
                                                        editor->document()->setModified(false);
                                                        SetBackgroundColor();
                                                } else {
                                                        sLabel1->setText(tr("error: (%1)").arg(16));
                                                        sLabel2->setText(tr("the file \"%1\" could not be saved!").arg(*TextFileName));
                                                        sLabel3->setText(" ");
                                                }
                                        } else {
                                                QMessageBox::information(this, tr("richtext_editor"), tr("the document \"%1\" has not been modified!").arg(*TextFileName), QMessageBox::Ok);
                                        }
                                }
                        }
                }
        }

        /*! \brief Die Methode `SaveItemTable()` schreibt alle Tabellen-Elemente unter der IndexNr. 258 in den Secue-Store.
         *
         *  \return _true_ wenn die Methode erfolgreich war (ansonsten _false_).
         */
        bool MainWindow::SaveItemTable() {
                size_t error = 0;
                ByteArray a(8192);
                ItemTable->ExportToByteArray(a);
                SecFile->Put(a, 258, error);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(17));
                        sLabel2->setText(tr("write-error securestore!"));
                        return false;
                }
                return true;
        }

        /*! \brief Die Methode `SecureItem()` speichert Text aus dem Eingabebereich verschlüsselt im Secure-Store.
         *
         *  Hierbei werden folgende Arbeitsschritte durchgeführt:
         *
         *  Der Text aus dem Eingabebereich wird in ein (utf8) Bytearray geschrieben. Ist das entstandene Bytearray zu groß? wenn ja, Fehlermeldung und return.
         *
         *  Ein neues (Konstante: _size_ großes) _text-Array_ wird mit Zufallszahlen gefüllt. Dann wird am Ende dieses Arrays die Länge des
         *  utf8-Bytearrays vermerkt und vor dieses Arrayende wird der Inhalt des utf8-Bytearray
         *  kopiert. Das utf8-Bytearray wird nun gelöscht. Im so gebildeten Array befinden sich am Anfang Zufallszahlen, dann die Nutzdaten
         *  und (maximal zwei Bytes) mit der Längenangabe der Nutzdaten.
         *
         *  Die Daten aus dem _text-Array_ werden nun byteweise mit einem (ebenfalls _size_ großem) OneTimePadKey verschlüsselt.
         *
         *  Die Daten aus dem _text-Array_ werden nun mit Redundanzbytes versehen. Dazu werden mehrere durchnummerierte 256 Byte große Blöcke zunächst teilweise mit
         *  Nutzdaten, einem Byte Endemarke und dem Rest Redundanzbytes gefüllt (vgl. Methode `CreateRedundancy()`). In diesen 256 Byte großen Blöcken kommt jeder möglich
         *  Bytewert [0..255] genau einmal vor. Die Bytes in den gebildeten Blöcke werden anschließend mit dem Schlüssel _PermutationKey_ vertauscht und die Blöcke dann
         *  zu einem großen Block (mit Namen _secureelement_) verkettet.
         *
         *  Der neue Eintrag wird in der ItemTable verwaltet und in der StatusBar werden die Daten angezeigt.
         *
         *  \param ref Referenz auf den Namen des Elementes.
         */
        void MainWindow::SecureItem(QString &ref) {
                size_t error = 0;
                ByteArray tin(editor->toPlainText(), true);
                ByteArray b;
                size_t index = 0;
                int length = static_cast<int>(tin.Size());
                b.IntegerToText(index, length, 10, true);
                QString *s1 = new QString(tr("txtlength:"));
                QString *s2 = b.ToQString();
                s1->append(*s2);
                s1->append(" Bytes");
                sLabel3->setText(*s1);
                delete s1;
                delete s2;
                ByteArray tsecure(65536);
                ByteArray tredundanz(size, true);
                rand->GetRandomBytes(tredundanz, size);
                secure->SetRedundanz(tredundanz);
                if (secure->Encrypt(tin, tsecure)) {
                        b.Clear();
                        index = 0;
                        b.IntegerToText(index, static_cast<int>(tsecure.Size()), 10, true);
                        QString *s1 = new QString(tr("secured:"));
                        QString *s2 = b.ToQString();
                        s1->append(*s2);
                        s1->append(" Bytes");
                        sLabel4->setText(*s1);
                        delete s1;
                        delete s2;
                        size_t storeindex;
                        storeindex = SecFile->Put(tsecure, 0, error);
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(46));
                                sLabel2->setText(tr("write-error securestore!"));
                                return;
                        }
                        editor->document()->setModified(false);
                        SetBackgroundColor();
                        QDateTime dt = QDateTime::currentDateTime();
                        ItemTable->CreateNewRow();
                        ItemTable->Row()->Day = dt.date().day();
                        ItemTable->Row()->Hour = dt.time().hour();
                        ItemTable->Row()->Minute = dt.time().minute();
                        ItemTable->Row()->Month = dt.date().month();
                        ItemTable->Row()->setName(ref);
                        ItemTable->Row()->Second = dt.time().second();
                        ItemTable->Row()->StoreIndex = storeindex;
                        ItemTable->Row()->Year = dt.date().year();
                        ItemTable->Insert(false);
                        sLabel1->setText(tr("secure"));
                        sLabel2->setText(ref + "  " + dt.toString("dd.MM.yyyy hh:mm"));
                        SaveItemTable();
                } else {
                        sLabel1->setText(tr("error: (%1)").arg(47));
                        sLabel2->setText(tr("a encryption error occurred!"));
                }
        }

        /*! \brief Die Methode `SetBackgroundColor()` steuert die Hintergrundfarbe des Eingabebereiches.
         *
         * Im Normalmodus wird der Hintergrund im Editor _rot_ dargestellt.
         * Im Securemodus wird der Hintergrund im Editor _weiss_ dargestellt.
         * Wird der im Editor vorhandene Text geändert, so wird der Hintergrund _gelb_ dargestellt.
         * Wird der text im Editor gespeichert, so wechselt die Hintergrundfarbe je nach Modus wieder zu _rot_ oder _weiss_.
         */
        void MainWindow::SetBackgroundColor() {
                if (editor->document()->isModified()) {
                        qApp->setStyleSheet("QTextEdit { background-color: yellow }");
                } else {
                        if (SecureMode) {
                                qApp->setStyleSheet("QTextEdit { background-color: white }");
                        } else {
                                qApp->setStyleSheet("QTextEdit { background-color: red }");
                        }
                }
        }

        /*! \brief Die Methode `SetupMenuBar()` erstellt alle Menü-Elemente des Programms.
         *
         *  Diese Methode wird im Konstruktor dieses Programms aufgerufen.
         */
        void MainWindow::SetupMenuBar() {
                QMenu *fileMenu = new QMenu(tr("file"), this);
                menuBar()->addMenu(fileMenu);
                QMenu *extraMenu = new QMenu(tr("extras"), this);
                menuBar()->addMenu(extraMenu);
                QMenu *helpMenu = new QMenu(tr("help"), this);
                menuBar()->addMenu(helpMenu);
                actOpen = fileMenu->addAction(QIcon(":/d/images/Open.png"), tr("open ..."), QKeySequence(tr("ctrl+O", "File|Open ...")), this, SLOT(OpenButton_Click()));
                actSave = fileMenu->addAction(QIcon(":/d/images/Save.png"), tr("save"), QKeySequence(tr("ctrl+S", "File|Save")), this, SLOT(SaveButton_Click()));
                actSaveAs = fileMenu->addAction(tr("save as ..."), QKeySequence(tr("ctrl+clt+S", "File|Save as ...")), this, SLOT(SaveAs_Click()));
                fileMenu->addSeparator();
                actQuit = fileMenu->addAction(QIcon(":/d/images/Exit.png"), tr("exit"), QKeySequence(tr("ctrl+Q", "File|Exit")), this, SLOT(ExitButton_Click()));
                extraMenu->addAction(tr("new store ..."), this, SLOT(NewStore_Click()));
                extraMenu->addSeparator();
                actSelectStore = extraMenu->addAction(tr("select store ..."), this, SLOT(StoreSelect_Click()));
                actMergeStore = extraMenu->addAction(tr("merge store ..."), this, SLOT(StoreMerge_Click()));
                actMergeStore->setVisible(false);
                actMergeStoreTrash = extraMenu->addAction(tr("merge store trash ..."), this, SLOT(StoreMerge_Trash_Click()));
                actMergeStoreTrash->setVisible(false);
                helpMenu->addAction(tr("languages ..."), this, SLOT(languages_Clicked()));
        }

        /*! \brief Die Methode `SetupMergeItems()` erstellt auf dem Heap alle zum Mergen benötigten Objektinstanzen.
         *
         *  Diese Methode wird in den Methoden `StoreMerge_Click()` und `StoreMerge_Trash_Click()` aufgerufen.
         */
        void MainWindow::SetupMergeItems() {
                MergeFileName1 = new QString();
                MergeFileName2 = new QString();
                MergeFileName3 = new QString();
                Merge1IV1 = new ByteArray(16, true);
                Merge1Key1 = new ByteArray(32, true);
                Merge1IV2 = new ByteArray(16, true);
                Merge1Key2 = new ByteArray(32, true);
                Merge2IV1 = new ByteArray(16, true);
                Merge2Key1 = new ByteArray(32, true);
                Merge2IV2 = new ByteArray(16, true);
                Merge2Key2 = new ByteArray(32, true);
                Merge3IV1 = new ByteArray(16, true);
                Merge3Key1 = new ByteArray(32, true);
                Merge3IV2 = new ByteArray(16, true);
                Merge3Key2 = new ByteArray(32, true);
                MergeFile = new Store::SecureFile();
                OneTimePadKeyM = new ByteArray(size, true);
                OneTimePadKeyM->SetSize(size);
                secureM = new Secure();
                EndLabelKeyM = new ByteArray(256, true);
                PermutationKeyM = new ByteArray(256, true);
        }

        /*! \brief Die Methode `SetupSecureItems()` erstellt auf dem Heap alle zur Verschlüsselung benötigten Objektinstanzen.
         *
         *  Diese Methode wird im Konstruktor dieses Programms aufgerufen.
         */
        void MainWindow::SetupSecureItems() {
                SecureMode = false;
                RandomData = new ByteArray(1049088, true);
                sha384 = new SHA384();
                sha512 = new SHA512();
                SecFile1IV1 = new ByteArray(16, true);
                SecFile1Key1 = new ByteArray(32, true);
                SecFile1IV2 = new ByteArray(16, true);
                SecFile1Key2 = new ByteArray(32, true);
                SecFile2IV1 = new ByteArray(16, true);
                SecFile2Key1 = new ByteArray(32, true);
                SecFile2IV2 = new ByteArray(16, true);
                SecFile2Key2 = new ByteArray(32, true);
                SecFile3IV1 = new ByteArray(16, true);
                SecFile3Key1 = new ByteArray(32, true);
                SecFile3IV2 = new ByteArray(16, true);
                SecFile3Key2 = new ByteArray(32, true);
                SecFile = new Store::SecureFile();
                rand = new Random();
                pbkdf2 = new PBKDF2();
                secure = new Secure();
                size_t length = (size << 1) + 1256;
                KeyGenerationArray = new ByteArray(length, true);
                OneTimePadKey = new ByteArray(size, true);
                OneTimePadKey->SetSize(size);
                EndLabelKey = new ByteArray(256, true);
                PermutationKey = new ByteArray(256, true);
                ItemTable = new SecureDB::ItemTable();
        }

        /*! \brief Die Methode `SetupStatusBar()` erstellt alle Elemente der Statuszeile des Programms und wird im Konstruktor aufgerufen.
         *
         *  Diese Methode wird im Konstruktor dieses Programms aufgerufen.
         */
        void MainWindow::SetupStatusBar() {
                sLabel1 = new QLabel(this);
                sLabel1->setText(tr("unsecure"));
                statusBar()->addPermanentWidget(sLabel1, 0);
                sLabel2 = new QLabel(this);
                sLabel2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
                statusBar()->addPermanentWidget(sLabel2, 4);
                sLabel3 = new QLabel(this);
                sLabel3->setFrameStyle(QFrame::Panel | QFrame::Sunken);
                sLabel3->setAlignment(Qt::AlignRight);
                statusBar()->addPermanentWidget(sLabel3, 1);
                sLabel4 = new QLabel(this);
                sLabel4->setFrameStyle(QFrame::Panel | QFrame::Sunken);
                sLabel4->setAlignment(Qt::AlignRight);
                statusBar()->addPermanentWidget(sLabel4, 1);
        }

        /*! \brief Die Methode `SetupToolBar()` erstellt alle Elemente der Werkzeugleiste des Programms und wird im Konstruktor aufgerufen.
         *
         *  Diese Methode wird im Konstruktor dieses Programms aufgerufen.
         */
        void MainWindow::SetupToolBar() {
                toolBar = addToolBar(tr("file"));
                toolBar->addAction(actQuit);
                toolBar->addAction(actOpen);
                toolBar->addAction(actSave);
                toolBar->addSeparator();
                toolBar->addAction(QIcon(":/d/images/RedWhite.png"), tr("redWhite"), this, SLOT(RedWhite_Click()));
                toolBar->addSeparator();
                actNext = toolBar->addAction(QIcon(":/d/images/Nunten.png"), tr("next"), this, SLOT(NextButton_Click()));
                actPrior = toolBar->addAction(QIcon(":/d/images/Noben.png"), tr("prior"), this, SLOT(PriorButton_Click()));
                actDelete = toolBar->addAction(QIcon(":/d/images/Minus.png"), tr("delete"), this, SLOT(DeleteButton_Click()));
                toolBar->setMovable(false);
                actNext->setVisible(false);
                actPrior->setVisible(false);
                actDelete->setVisible(false);
                toolBar->addSeparator();
                sLabel5 = new QLabel(this);
                sLabel5->setFrameStyle(QFrame::Panel | QFrame::Sunken);
                sLabel5->setText(*SecFileName3);
                toolBar->addWidget(sLabel5);
        }

        /*! \brief Die Methode `StartSecureMerge()` versucht einen weiteren Secure-Store zu öffnen.
         *
         *  Ist dies erfolgreich wird true zurück gegeben. Tritt ein Fehler auf, wird eine Fehlermeldung
         *  in der Statusbar ausgegeben und die Funktion bricht mit false ab. (nachfolgend die Einzelschritte:)
         *
         *  Genügend lange Passworte (mind. 8 Zeichen) vorhanden? wenn nein, return false.
         *  Läßt sich der angegebene Startindex in einen Integerwert wandeln? wenn nein, return false.
         *  Läßt sich der angegebene Differenzwert in einen Integerwert wandeln? wenn nein, return false.
         *  Lassen sich die beiden angegebene Hashrundenzwerte in Integerwerte wandeln? wenn nein, return false.
         *  Die sechs Eingabefelder werden gelöscht und aus den Daten werden vier 32 Byte langer StoreKeys und vier 16 Byte langer Initialisierungsvektoren
         *  gebildet. Können die beiden ersten Store-Dateien (mit den gebildeten Keys/IVs) geöffnet werden?
         *  wenn nein, Fehlermeldung und return false.
         *
         *  Können der Datenblock zur Schlüsselgenerierung und die Zufallsdaten aus den beiden ersten Store-Dateien gelesen werden? wenn nein, return false.
         *
         *  Nun werden die Schlüssel _OneTimePadKey_, _EndLabelKey_, _PermutationKey_, _StoreBKey1_, _StoreBIV1_, _StoreBKey2_ und _StoreBIV2_ in dieser
         *  Reihenfolge aus den Zufallsdaten und dem Datenblock zur Schlüsselgenerierung gebildet.
         *
         *  Nun wird versucht die dritte Store-Datei zu öffnen. Ist dies nicht möglich, Fehlermeldung und return false.
         *
         *  Zum Schluss werden alle Hilfsdaten gelöscht und _true_ zurück gegeben.
         *
         *  \return _true_ wenn die Methode erfolgreich war (ansonsten _false_).
         *  \sa StartSecureMode()
         */
        bool MainWindow::StartSecureMerge() {
                size_t error = 0;
                PWDaten1 pw1;
                pw1.exec();
                bool ok;
                int count1;
                int count2;
                if (pw1.isOK) {
                        StartIndex = pw1.si1->ToInteger(&ok);
                        if (!ok) {
                                pw1.isOK = false;
                        }
                        Spread = pw1.di1->ToInteger(&ok);
                        if (!ok) {
                                pw1.isOK = false;
                        }
                        count1 = pw1.hr1->ToInteger(&ok);
                        pw1.hr1->Clear();
                        if (!ok) {
                                pw1.isOK = false;
                        }
                        count2 = pw1.hr2->ToInteger(&ok);
                        pw1.hr2->Clear();
                        if (!ok) {
                                pw1.isOK = false;
                        }
                        pw1.si1->Clear();
                        pw1.di1->Clear();
                }
                if (!pw1.isOK) {
                        sLabel1->setText(tr("error: (%1)").arg(30));
                        sLabel2->setText(tr("the data entered are not plausible!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                pbkdf2->SetIteration(count1);
                pbkdf2->SetPW(*(pw1.pw1));
                ByteArray *salt1 = new ByteArray(64);
                salt1->Copy(Salt1, 0, 0, 64);
                pbkdf2->SetSalt(*salt1);
                ByteArray *ivKey = pbkdf2->GetKeys();
                if (ivKey != nullptr) {
                        Merge1IV1->Copy(*ivKey, 0, 0, 16);
                        Merge1Key1->Copy(*ivKey, 16, 0, 32);
                }
                salt1->Copy(Salt2, 0, 0, 64);
                pbkdf2->SetSalt(*salt1);
                ivKey = nullptr;
                ivKey = pbkdf2->GetKeys();
                if (ivKey != nullptr) {
                        Merge1IV2->Copy(*ivKey, 0, 0, 16);
                        Merge1Key2->Copy(*ivKey, 16, 0, 32);
                }
                error = MergeFile->Open(MergeFileName1, Merge1IV1, Merge1Key1, Merge1IV2, Merge1Key2);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(31));
                        sLabel2->setText(tr("open-error mergestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                MergeFile->Get(*RandomData, 260, error);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(32));
                        sLabel2->setText(tr("read-error mergestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                RandomDataLength = RandomData->Size();
                error = MergeFile->Close();
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(33));
                        sLabel2->setText(tr("close-error mergestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                pbkdf2->SetIteration(count2);
                pbkdf2->SetPW(*(pw1.pw2));
                salt1->Copy(Salt1, 0, 0, 64);
                pbkdf2->SetSalt(*salt1);
                ivKey = pbkdf2->GetKeys();
                if (ivKey != nullptr) {
                        Merge2IV1->Copy(*ivKey, 0, 0, 16);
                        Merge2Key1->Copy(*ivKey, 16, 0, 32);
                }
                salt1->Copy(Salt2, 0, 0, 64);
                pbkdf2->SetSalt(*salt1);
                ivKey = nullptr;
                ivKey = pbkdf2->GetKeys();
                if (ivKey != nullptr) {
                        Merge2IV2->Copy(*ivKey, 0, 0, 16);
                        Merge2Key2->Copy(*ivKey, 16, 0, 32);
                }
                delete salt1;
                error = MergeFile->Open(MergeFileName2, Merge2IV1, Merge2Key1, Merge2IV2, Merge2Key2);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(34));
                        sLabel2->setText(tr("open-error mergestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                MergeFile->Get(*KeyGenerationArray, 270, error);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(35));
                        sLabel2->setText(tr("read-error mergestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                error = MergeFile->Close();
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(36));
                        sLabel2->setText(tr("close-error mergestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                ByteArray hash(64, true);
                hash.Copy(*KeyGenerationArray, 0, 0, 64);
                if (!sha512->TestHash(*RandomData, hash)) {
                        sLabel1->setText(tr("error: (%1)").arg(37));
                        DeleteKeyGenerationData2();
                        return false;
                }
                hash.Copy(*KeyGenerationArray, 64, 0, 48);
                if (!sha384->TestHash(*RandomData, hash)) {
                        sLabel1->setText(tr("error: (%1)").arg(38));
                        DeleteKeyGenerationData2();
                        return false;
                }
                size_t readindex = 112;
                int index = StartIndex + Spread;
                int bytegab = 0;
                unsigned char c = 0;
                OneTimePadKeyM->SetSize(size);
                for (size_t i = 0; i < size; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*OneTimePadKeyM)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                secureM->SetOneTimePadKey(*OneTimePadKeyM);
                EndLabelKeyM->SetSize(256);
                for (size_t i = 0; i < 256; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*EndLabelKeyM)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                secureM->SetLabelKey(*EndLabelKeyM);
                PermutationKeyM->SetSize(256);
                for (size_t i = 0; i < 256; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*PermutationKeyM)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                secureM->SetPermutationKey(*PermutationKeyM);
                Merge3Key1->SetSize(32);
                for (size_t i = 0; i < 32; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*Merge3Key1)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                Merge3IV1->SetSize(16);
                for (size_t i = 0; i < 16; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*Merge3IV1)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                Merge3Key2->SetSize(32);
                for (size_t i = 0; i < 32; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*Merge3Key2)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                Merge3IV2->SetSize(16);
                for (size_t i = 0; i < 16; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*Merge3IV2)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                error = MergeFile->Open(MergeFileName3, Merge3IV1, Merge3Key1, Merge3IV2, Merge3Key2);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(39));
                        sLabel2->setText(tr("open-error mergestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                DeleteKeyGenerationData2();
                return true;
        }

        /*! \brief Die Methode `StartSecureMode()` versucht in den Secure-Mode zu wechseln.
         *
         *  Die Funktion versucht in den Secure-Mode zu wechseln. Ist dies erfolgreich wird true zurück gegeben. Tritt ein Fehler auf, wird eine Fehlermeldung
         *  in der Statusbar ausgegeben und die Funktion bricht mit false ab. (nachfolgend die Einzelschritte:)
         *
         *  Genügend lange Passworte (mind. 8 Zeichen) vorhanden? wenn nein, return false.
         *  Läßt sich der angegebene Startindex in einen Integerwert wandeln? wenn nein, return false.
         *  Läßt sich der angegebene Differenzwert in einen Integerwert wandeln? wenn nein, return false.
         *  Lassen sich die beiden angegebene Hashrundenzwerte in Integerwerte wandeln? wenn nein, return false.
         *  Die sechs Eingabefelder werden gelöscht und aus den Daten werden vier 32 Byte langer StoreKeys und vier 16 Byte langer Initialisierungsvektoren
         *  gebildet. Können die beiden ersten Store-Dateien (mit den gebildeten Keys/IVs) geöffnet werden?
         *  wenn nein, Fehlermeldung und return false.
         *
         *  Können der Datenblock zur Schlüsselgenerierung und die Zufallsdaten aus den beiden ersten Store-Dateien gelesen werden? wenn nein, return false.
         *
         *  Nun werden die Schlüssel _OneTimePadKey_, _EndLabelKey_, _PermutationKey_, _StoreBKey1_, _StoreBIV1_, _StoreBKey2_ und _StoreBIV2_ in dieser
         *  Reihenfolge aus den Zufallsdaten und dem Datenblock zur Schlüsselgenerierung gebildet.
         *
         *  Nun wird versucht die dritte Store-Datei zu öffnen. Ist dies nicht möglich, Fehlermeldung und return false.
         *
         *  Zum Schluss werden alle Hilfsdaten gelöscht und _true_ zurück gegeben.
         *
         *  \return _true_ wenn die Methode erfolgreich war (ansonsten _false_).
         *  \sa StartSecureMerge()
         */
        bool MainWindow::StartSecureMode() {
                size_t error = 0;
                PWDaten1 pw1;
                pw1.exec();
                bool ok;
                int count1;
                int count2;
                if (pw1.isOK) {
                        if (pw1.pw1->Size() < 8) {
                                pw1.isOK = false;
                        }
                        if (pw1.pw2->Size() < 8) {
                                pw1.isOK = false;
                        }
                        StartIndex = pw1.si1->ToInteger(&ok);
                        if (!ok) {
                                pw1.isOK = false;
                        }
                        Spread = pw1.di1->ToInteger(&ok);
                        if (!ok) {
                                pw1.isOK = false;
                        }
                        count1 = pw1.hr1->ToInteger(&ok);
                        pw1.hr1->Clear();
                        if (!ok) {
                                pw1.isOK = false;
                        }
                        count2 = pw1.hr2->ToInteger(&ok);
                        pw1.hr2->Clear();
                        if (!ok) {
                                pw1.isOK = false;
                        }
                        pw1.si1->Clear();
                        pw1.di1->Clear();
                }
                if (!pw1.isOK) {
                        sLabel1->setText(tr("error: (%1)").arg(20));
                        sLabel2->setText(tr("the data entered are not plausible!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                pbkdf2->SetIteration(count1);
                pbkdf2->SetPW(*(pw1.pw1));
                ByteArray *salt1 = new ByteArray(64);
                salt1->Copy(Salt1, 0, 0, 64);
                pbkdf2->SetSalt(*salt1);
                ByteArray *ivKey = pbkdf2->GetKeys();
                if (ivKey != nullptr) {
                        SecFile1IV1->Copy(*ivKey, 0, 0, 16);
                        SecFile1Key1->Copy(*ivKey, 16, 0, 32);
                }
                salt1->Copy(Salt2, 0, 0, 64);
                pbkdf2->SetSalt(*salt1);
                ivKey = nullptr;
                ivKey = pbkdf2->GetKeys();
                if (ivKey != nullptr) {
                        SecFile1IV2->Copy(*ivKey, 0, 0, 16);
                        SecFile1Key2->Copy(*ivKey, 16, 0, 32);
                }
                error = SecFile->Open(SecFileName1, SecFile1IV1, SecFile1Key1, SecFile1IV2, SecFile1Key2);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(21));
                        sLabel2->setText(tr("open-error securestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                SecFile->Get(*RandomData, 260, error);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(22));
                        sLabel2->setText(tr("read-error securestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                RandomDataLength = RandomData->Size();
                error = SecFile->Close();
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(23));
                        sLabel2->setText(tr("close-error securestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                pbkdf2->SetIteration(count2);
                pbkdf2->SetPW(*(pw1.pw2));
                salt1->Copy(Salt1, 0, 0, 64);
                pbkdf2->SetSalt(*salt1);
                ivKey = pbkdf2->GetKeys();
                if (ivKey != nullptr) {
                        SecFile2IV1->Copy(*ivKey, 0, 0, 16);
                        SecFile2Key1->Copy(*ivKey, 16, 0, 32);
                }
                salt1->Copy(Salt2, 0, 0, 64);
                pbkdf2->SetSalt(*salt1);
                ivKey = nullptr;
                ivKey = pbkdf2->GetKeys();
                if (ivKey != nullptr) {
                        SecFile2IV2->Copy(*ivKey, 0, 0, 16);
                        SecFile2Key2->Copy(*ivKey, 16, 0, 32);
                }
                delete salt1;
                error = SecFile->Open(SecFileName2, SecFile2IV1, SecFile2Key1, SecFile2IV2, SecFile2Key2);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(24));
                        sLabel2->setText(tr("open-error securestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                SecFile->Get(*KeyGenerationArray, 270, error);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(25));
                        sLabel2->setText(tr("read-error securestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                error = SecFile->Close();
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(26));
                        sLabel2->setText(tr("close-error securestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                ByteArray hash(64, true);
                hash.Copy(*KeyGenerationArray, 0, 0, 64);
                if (!sha512->TestHash(*RandomData, hash)) {
                        sLabel1->setText(tr("error: (%1)").arg(27));
                        DeleteKeyGenerationData2();
                        return false;
                }
                hash.Copy(*KeyGenerationArray, 64, 0, 48);
                if (!sha384->TestHash(*RandomData, hash)) {
                        sLabel1->setText(tr("error: (%1)").arg(28));
                        DeleteKeyGenerationData2();
                        return false;
                }
                size_t readindex = 112;
                int index = StartIndex + Spread;
                int bytegab = 0;
                unsigned char c = 0;
                OneTimePadKey->SetSize(size);
                for (size_t i = 0; i < size; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*OneTimePadKey)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                secure->SetOneTimePadKey(*OneTimePadKey);
                EndLabelKey->SetSize(256);
                for (size_t i = 0; i < 256; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*EndLabelKey)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                secure->SetLabelKey(*EndLabelKey);
                PermutationKey->SetSize(256);
                for (size_t i = 0; i < 256; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*PermutationKey)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                secure->SetPermutationKey(*PermutationKey);
                SecFile3Key1->SetSize(32);
                for (size_t i = 0; i < 32; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*SecFile3Key1)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                SecFile3IV1->SetSize(16);
                for (size_t i = 0; i < 16; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*SecFile3IV1)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                SecFile3Key2->SetSize(32);
                for (size_t i = 0; i < 32; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*SecFile3Key2)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                SecFile3IV2->SetSize(16);
                for (size_t i = 0; i < 16; i++) {
                        bytegab = KeyGenerationArray->ReadX209Int64(readindex);
                        index = index + bytegab;
                        c = static_cast<unsigned char>((*RandomData)[static_cast<size_t>(index)]);
                        (*SecFile3IV2)[i] = static_cast<char>(c);
                        if (bytegab > 0) {
                                index = index + c + Spread;
                        } else {
                                index = index + c - Spread;
                        }
                }
                error = SecFile->Open(SecFileName3, SecFile3IV1, SecFile3Key1, SecFile3IV2, SecFile3Key2);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(29));
                        sLabel2->setText(tr("open-error securestore!"));
                        DeleteKeyGenerationData2();
                        return false;
                }
                DeleteKeyGenerationData2();
                return true;
        }


        /*! \brief Aus einem weiteren Secure-Store können noch nicht vorhandene Elemente in den aktuellen Secure-Store übernommen werden.
         *
         *  Dieses Methode wird aufgerufen wenn im Menü __Extras->Store zusammen führen ...__ angeklickt wird (diese Menüpunkt ist nur im Securemodus vorhanden).
         *  Im Standard-Datei-Dialog des Betriebssystems kann ein weiterer Secure-Store ausgewählt werden. Danach öffnet sich ein Dialog zur Eingabe der
         *  Passwortdaten für diesen (weiteren) Secure-Store. Wenn die Daten korrekt eingegeben wurden, werden die Elemente aus diesem Secure-Store, die noch
         *  nicht im aktuellen Secure-Store vorhanden sind, in den aktuellen Secure-Store übernommen.
         */
        void MainWindow::StoreMerge_Click() {
                size_t error = 0;
                int ret = 0;
                if (editor->document()->isModified()) {
                        ret = QMessageBox::warning(this, tr("richtext_editor"), tr("should this text be saved first?"),
                                                   QMessageBox::Yes | QMessageBox::No);
                        if (ret == QMessageBox::No) {
                                editor->document()->clear();
                                editor->document()->setModified(false);
                                SetBackgroundColor();
                        } else {
                                return;
                        }
                }
                SetupMergeItems();
                QString s;
                s.append(QFileDialog::getOpenFileName(this, tr("set merge store"), QDir::homePath(), tr("all files (*.*)")));
                if (!s.isEmpty()) {
                        int index = s.lastIndexOf('.');
                        if (index > -1) {
                                s.remove(index + 1, 3);
                                s.append("rnd");
                                MergeFileName1->clear();
                                MergeFileName1->append(s);
                                s.remove(index + 1, 3);
                                s.append("key");
                                MergeFileName2->clear();
                                MergeFileName2->append(s);
                                s.remove(index + 1, 3);
                                s.append("bin");
                                MergeFileName3->clear();
                                MergeFileName3->append(s);
                        }
                } else {
                        sLabel1->setText(tr("error: (%1)").arg(45));
                        sLabel2->setText(tr("mergestorename missing!"));
                        ClearMergeItems();
                        return;
                }
                if (StartSecureMerge()) {
                        ByteArray tredundanz(size, true);
                        ByteArray a(8192, true);
                        MergeFile->Get(a, 258, error);
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(40));
                                sLabel2->setText(tr("read-error mergestore!"));
                                ClearMergeItems();
                                return;
                        }
                        SecureDB::ItemTable itemTable;
                        ByteArray plaintext(size, true);
                        itemTable.ClearAll();
                        itemTable.ImportFromByteArray(a, true);
                        itemTable.SetKeyIndex(ItemTable->GetKeyIndex());
                        editor->append(tr("begin store merge:\r\n"));
                        itemTable.First();
                        while (itemTable.Row() != nullptr) {
                                QString *Name = itemTable.Row()->getName();
                                int year = itemTable.Row()->Year;
                                int month = itemTable.Row()->Month;
                                int day = itemTable.Row()->Day;
                                int hour = itemTable.Row()->Hour;
                                int minute = itemTable.Row()->Minute;
                                int second = itemTable.Row()->Second;
                                ItemTable->SearchRow()->setName(*Name);
                                ItemTable->SearchRow()->Year = year;
                                ItemTable->SearchRow()->Month = month;
                                ItemTable->SearchRow()->Day = day;
                                ItemTable->SearchRow()->Hour = hour;
                                ItemTable->SearchRow()->Minute = minute;
                                ItemTable->SearchRow()->Second = second;
                                if (!ItemTable->Seek(false)) {
                                        editor->append(*itemTable.Row()->getFullName());
                                        size_t storeindex = itemTable.Row()->StoreIndex;
                                        MergeFile->Get(a, storeindex, error);
                                        if (error > 0) {
                                                sLabel1->setText(tr("error: (%1)").arg(41));
                                                sLabel2->setText(tr("read-error mergestore!"));
                                                ClearMergeItems();
                                                return;
                                        }
                                        if (!secureM->Decrypt(a, plaintext)) {
                                                sLabel1->setText(tr("error: (%1)").arg(42));
                                                sLabel2->setText(tr("a decryption error occurred!"));
                                                ClearMergeItems();
                                                return;
                                        }
                                        rand->GetRandomBytes(tredundanz, size);
                                        secure->SetRedundanz(tredundanz);
                                        if (!secure->Encrypt(plaintext, a)) {
                                                sLabel1->setText(tr("error: (%1)").arg(43));
                                                sLabel2->setText(tr("a encryption error occurred!"));
                                                ClearMergeItems();
                                                return;
                                        }
                                        storeindex = SecFile->Put(a, 0, error);
                                        if (error > 0) {
                                                sLabel1->setText(tr("error: (%1)").arg(44));
                                                sLabel2->setText(tr("write-error securestore!"));
                                                ClearMergeItems();
                                                return;
                                        }
                                        if (storeindex > 1023) {
                                                editor->append(tr("successfully written\r\n"));
                                                ItemTable->CreateNewRow();
                                                ItemTable->Row()->Day = day;
                                                ItemTable->Row()->Hour = hour;
                                                ItemTable->Row()->Minute = minute;
                                                ItemTable->Row()->Month = month;
                                                ItemTable->Row()->setName(*Name);
                                                ItemTable->Row()->Second = second;
                                                ItemTable->Row()->StoreIndex = storeindex;
                                                ItemTable->Row()->Year = year;
                                                ItemTable->Insert(false);
                                                SaveItemTable();
                                        }
                                        plaintext.SetSize(0);
                                }
                                itemTable.Next();
                        }
                        editor->append(tr("end store merge:\r\n"));
                        error = MergeFile->Close();
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(45));
                                sLabel2->setText(tr("close-error mergestore!"));
                        }
                }
                ClearMergeItems();
        }

        /*! \brief Aus dem aktuellen Secure-Store können gelöschte Elemente in einen weiteren Secure-Store übernommen werden.
         *
         *  Dieses Methode wird aufgerufen wenn im Menü __Extras->Gelöschte Elemente in einen anderen Secure-Store kopieren ...__ angeklickt wird
         *  (diese Menüpunkt ist nur im Securemodus vorhanden).
         *  Im Standard-Datei-Dialog des Betriebssystems kann ein weiterer Secure-Store ausgewählt werden. Danach öffnet sich ein Dialog zur Eingabe der
         *  Passwortdaten für diesen (weiteren) Secure-Store. Wenn die Daten korrekt eingegeben wurden, werden die gelöschten Elemente aus dem aktuellen
         *  Secure-Store, die noch nicht im weiteren Secure-Store vorhanden sind, in den weiteren Secure-Store übernommen.
         */
        void MainWindow::StoreMerge_Trash_Click() {
                size_t error = 0;
                int ret = 0;
                if (editor->document()->isModified()) {
                        ret = QMessageBox::warning(this, tr("richtext_editor"), tr("should this text be saved first?"),
                                                   QMessageBox::Yes | QMessageBox::No);
                        if (ret == QMessageBox::No) {
                                editor->document()->clear();
                                editor->document()->setModified(false);
                                SetBackgroundColor();
                        } else {
                                return;
                        }
                }
                SetupMergeItems();
                QString s;
                s.append(QFileDialog::getOpenFileName(this, tr("set merge store"), QDir::homePath(), tr("all files (*.*)")));
                if (!s.isEmpty()) {
                        int index = s.lastIndexOf('.');
                        if (index > -1) {
                                s.remove(index + 1, 3);
                                s.append("rnd");
                                MergeFileName1->clear();
                                MergeFileName1->append(s);
                                s.remove(index + 1, 3);
                                s.append("key");
                                MergeFileName2->clear();
                                MergeFileName2->append(s);
                                s.remove(index + 1, 3);
                                s.append("bin");
                                MergeFileName3->clear();
                                MergeFileName3->append(s);
                        }
                } else {
                        sLabel1->setText(tr("error: (%1)").arg(45));
                        sLabel2->setText(tr("mergestorename missing!"));
                        ClearMergeItems();
                        return;
                }
                if (StartSecureMerge()) {
                        ByteArray tredundanz(size, true);
                        ByteArray a(8192, true);
                        MergeFile->Get(a, 258, error);
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(40));
                                sLabel2->setText(tr("read-error mergestore!"));
                                ClearMergeItems();
                                return;
                        }
                        SecureDB::ItemTable itemTable;
                        ByteArray plaintext(size, true);
                        itemTable.ClearAll();
                        itemTable.ImportFromByteArray(a, true);
                        itemTable.SetKeyIndex(ItemTable->GetKeyIndex());
                        editor->append(tr("begin store merge:\r\n"));
                        ItemTable->First();
                        while (ItemTable->Row() != nullptr) {
                                if (ItemTable->Row()->deleted == 1) {
                                        QString *Name = ItemTable->Row()->getName();
                                        int year = ItemTable->Row()->Year;
                                        int month = ItemTable->Row()->Month;
                                        int day = ItemTable->Row()->Day;
                                        int hour = ItemTable->Row()->Hour;
                                        int minute = ItemTable->Row()->Minute;
                                        int second = ItemTable->Row()->Second;
                                        itemTable.SearchRow()->setName(*Name);
                                        itemTable.SearchRow()->Year = year;
                                        itemTable.SearchRow()->Month = month;
                                        itemTable.SearchRow()->Day = day;
                                        itemTable.SearchRow()->Hour = hour;
                                        itemTable.SearchRow()->Minute = minute;
                                        itemTable.SearchRow()->Second = second;
                                        if (!itemTable.Seek(false)) {
                                                editor->append(*ItemTable->Row()->getFullName());
                                                size_t storeindex = ItemTable->Row()->StoreIndex;
                                                SecFile->Get(a, storeindex, error);
                                                if (error > 0) {
                                                        sLabel1->setText(tr("error: (%1)").arg(41));
                                                        sLabel2->setText(tr("read-error store!"));
                                                        ClearMergeItems();
                                                        return;
                                                }
                                                if (!secure->Decrypt(a, plaintext)) {
                                                        sLabel1->setText(tr("error: (%1)").arg(42));
                                                        sLabel2->setText(tr("a decryption error occurred!"));
                                                        ClearMergeItems();
                                                        return;
                                                }
                                                rand->GetRandomBytes(tredundanz, size);
                                                secureM->SetRedundanz(tredundanz);
                                                if (!secureM->Encrypt(plaintext, a)) {
                                                        sLabel1->setText(tr("error: (%1)").arg(43));
                                                        sLabel2->setText(tr("a encryption error occurred!"));
                                                        ClearMergeItems();
                                                        return;
                                                }
                                                storeindex = MergeFile->Put(a, 0, error);
                                                if (error > 0) {
                                                        sLabel1->setText(tr("error: (%1)").arg(44));
                                                        sLabel2->setText(tr("write-error mergestore!"));
                                                        ClearMergeItems();
                                                        return;
                                                }
                                                if (storeindex > 1023) {
                                                        editor->append(tr("successfully written\r\n"));
                                                        itemTable.CreateNewRow();
                                                        itemTable.Row()->Day = day;
                                                        itemTable.Row()->Hour = hour;
                                                        itemTable.Row()->Minute = minute;
                                                        itemTable.Row()->Month = month;
                                                        itemTable.Row()->setName(*Name);
                                                        itemTable.Row()->Second = second;
                                                        itemTable.Row()->StoreIndex = storeindex;
                                                        itemTable.Row()->Year = year;
                                                        itemTable.Insert(false);
                                                        ByteArray b(8192);
                                                        itemTable.ExportToByteArray(b);
                                                        MergeFile->Put(b, 258, error);
                                                        if (error > 0) {
                                                                sLabel1->setText(tr("error: (%1)").arg(17));
                                                                sLabel2->setText(tr("write-error mergestore!"));
                                                                return;
                                                        }
                                                }
                                                plaintext.SetSize(0);
                                        }
                                }
                                ItemTable->Next();
                        }
                        editor->append(tr("end store merge:\r\n"));
                        error = MergeFile->Close();
                        if (error > 0) {
                                sLabel1->setText(tr("error: (%1)").arg(45));
                                sLabel2->setText(tr("close-error mergestore!"));
                        }
                }
                ClearMergeItems();
        }

        /*! \brief Die Methode `StoreSelect_Click()` legt den Pfad und die Namen des benutzten Secure-Store fest.
         *
         *  Dieses Methode wird aufgerufen wenn im Menü __Extras->Secure-Store auswählen ...__ angeklickt wird.
         */
        void MainWindow::StoreSelect_Click(bool newFile) {
                QString s;
                if (newFile) {
                        s.append(QFileDialog::getSaveFileName(this, tr("set secure store"), "", tr("all files (*.*)")));
                } else {
                        s.append(QFileDialog::getOpenFileName(this, tr("set secure store"), "", tr("all files (*.*)")));
                }
                if (!s.isEmpty()) {
                        int index = s.lastIndexOf('.');
                        if (index > -1) {
                                s.remove(index + 1, 3);
                                s.append("rnd");
                                settings->setValue("SecFileLocation1", s);
                                SecFileName1->clear();
                                SecFileName1->append(s);
                                s.remove(index + 1, 3);
                                s.append("key");
                                settings->setValue("SecFileLocation2", s);
                                SecFileName2->clear();
                                SecFileName2->append(s);
                                s.remove(index + 1, 3);
                                s.append("bin");
                                settings->setValue("SecFileLocation3", s);
                                SecFileName3->clear();
                                SecFileName3->append(s);
                                sLabel5->setText(*SecFileName3);
                        }
                }
        }

        /*! \brief Die Methode `UnsecureItem()` liest ein Element aus dem Secure-Store und stellt es im Eingabebereich im Klartext dar.
         *
         *  Hierbei werden folgende  Arbeitsschritte durchgeführt:
         *
         *  Verschlüsseltes Element aus dem Store lesen. Erfolgreich? wenn nein, return false.
         *
         *  Den erhaltenen Block in 256 Byte große Blöcke aufteilen. Für jeden Block die folgenden Aktionen durchführen:
         *
         *  Überprüfen ob jeder Bytewert [0..255] genau einmal vorkommt. wenn nein, return false. Byte-Vertauschung mit dem Schlüssel _PermutationKey_ rückgängig machen.
         *  Redundanz-Bytes entfernen und Nutzdaten aufsammeln (vgl. Methode `TestAndDeleteRedundancy()`).
         *
         *  Als nächstes wird die OneTimePad-Verschlüsselung rückgängig gemacht.
         *
         *  Nutzdaten im Eingabebereich anzeigen und Informationen in der StatusBar anzeigen.
         *
         *  \return _true_ wenn die Methode erfolgreich war (ansonsten _false_).
         */
        bool MainWindow::UnsecureItem(size_t storeIndex) {
                size_t error = 0;
                ByteArray securein;
                SecFile->Get(securein, storeIndex, error);
                if (error > 0) {
                        sLabel1->setText(tr("error: (%1)").arg(18));
                        sLabel2->setText(tr("read-error securestore!"));
                        return false;
                }
                ByteArray b;
                size_t index = 0;
                b.IntegerToText(index, static_cast<int>(securein.Size()), 10, true);
                QString *s1 = new QString(tr("secured:"));
                QString *s2 = b.ToQString();
                s1->append(*s2);
                s1->append(" Bytes");
                sLabel4->setText(*s1);
                delete s1;
                delete s2;
                ByteArray plaintext(size, true);
                if (secure->Decrypt(securein, plaintext)) {
                        b.Clear();
                        index = 0;
                        int length = static_cast<int>(plaintext.Size());
                        b.IntegerToText(index, length, 15, true);
                        QString *s1 = new QString(tr("txtlength:"));
                        s2 = b.ToQString();
                        s1->append(*s2);
                        s1->append(" Bytes");
                        sLabel3->setText(*s1);
                        delete s1;
                        delete s2;
                        editor->document()->setPlainText(QString::fromUtf8(reinterpret_cast<const char*>(&plaintext.WriteareaReferenz(0, static_cast<size_t>(length))), length));
                        editor->document()->setModified(false);
                        SetBackgroundColor();
                        return true;
                } else {
                        sLabel1->setText(tr("error: (%1)").arg(19));
                        sLabel2->setText(tr("a decryption error occurred!"));
                        return false;
                }
        }

} // end of namespace SecureDB



