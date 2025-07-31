/* Copyright (c) 2020-2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "xoptionswidget.h"
#include "DesktopIntegrationHelper.h"

#include "guimainwindow.h"
#include "ui_xoptionswidget.h"

XOptionsWidget::XOptionsWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XOptionsWidget)
{
    ui->setupUi(this);


    g_pParent = pParent;
    g_pOptions = nullptr;
    g_sApplicationDisplayName = "";


    connect(this, SIGNAL(saveSignal()), this, SLOT(save()), Qt::DirectConnection);
    connect(this, SIGNAL(reloadSignal()), this, SLOT(reload()), Qt::DirectConnection);



#ifdef Q_OS_WIN
    g_userRole = XOptions::USERROLE_NORMAL;
#endif
}


XOptionsWidget::~XOptionsWidget()
{
    delete ui;
}

void XOptionsWidget::adjustView()
{
    getGlobalOptions()->adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);
    XShortcutsWidget::adjustViewChildren(this);
}

void XOptionsWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    XShortcutsWidget::setGlobalChildren(this, pShortcuts, pXOptions);
}

void XOptionsWidget::setOptions(XOptions *pOptions, const QString &sApplicationDisplayName)
{
    g_pOptions = pOptions;
    g_sApplicationDisplayName = sApplicationDisplayName;

    if (g_pOptions->isGroupIDPresent(XOptions::GROUPID_VIEW)) {
        addListRecord(tr("Appearance"), 0);
        ui->pageView->setProperty("GROUPID", XOptions::GROUPID_VIEW);
    }

    if (g_pOptions->isGroupIDPresent(XOptions::GROUPID_FILE)) {
        addListRecord(tr("File"), 1);
        ui->pageFile->setProperty("GROUPID", XOptions::GROUPID_FILE);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_CONTROLS) || g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_TABLEVIEWS) ||
        g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_TREEVIEWS) || g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_TEXTEDITS)) {
        addListRecord(tr("Fonts"), 2);
        ui->pageFile->setProperty("GROUPID", XOptions::GROUPID_FONTS);
    }

    reload();
}

void XOptionsWidget::addListRecord(const QString &sTitle, qint32 nIndex)
{
    QListWidgetItem *pItem = new QListWidgetItem;

    pItem->setText(sTitle);
    pItem->setData(Qt::UserRole, nIndex);

    ui->listWidgetOptions->addItem(pItem);

    XOptions::adjustListWidgetSize(ui->listWidgetOptions);
}

void XOptionsWidget::addPage(QWidget *pWidget, const QString &sTitle)
{
    qint32 nIndex = ui->stackedWidgetOptions->addWidget(pWidget);

    addListRecord(sTitle, nIndex);

    connect(this, SIGNAL(saveSignal()), pWidget, SLOT(save()), Qt::DirectConnection);
    connect(this, SIGNAL(reloadSignal()), pWidget, SLOT(reload()), Qt::DirectConnection);
}

void XOptionsWidget::setCurrentPage(qint32 nPage)
{
    if (nPage < ui->listWidgetOptions->count()) {
        ui->listWidgetOptions->setCurrentRow(nPage);
    }
}

void XOptionsWidget::setCurrentPage(XOptions::GROUPID groupId)
{
    if (groupId != XOptions::GROUPID_UNKNOWN) {
        qint32 nNumberOfPages = ui->stackedWidgetOptions->count();
        qint32 nNumberOfLists = ui->listWidgetOptions->count();

        for (qint32 i = 0; i < nNumberOfPages; i++) {
            if (ui->stackedWidgetOptions->widget(i)->property("GROUPID").toUInt() == groupId) {
                for (qint32 j = 0; j < nNumberOfLists; j++) {
                    qint32 nIndex = ui->listWidgetOptions->item(j)->data(Qt::UserRole).toInt();

                    if (nIndex == i) {
                        ui->listWidgetOptions->setCurrentRow(j);

                        break;
                    }
                }

                break;
            }
        }
    }
}

void XOptionsWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
    reload();
}

void XOptionsWidget::save()
{
    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_STAYONTOP)) {
        g_pOptions->getCheckBox(ui->checkBoxViewStayOnTop, XOptions::ID_VIEW_STAYONTOP);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_SINGLEAPPLICATION)) {
        g_pOptions->getCheckBox(ui->checkBoxViewSingleApplication, XOptions::ID_VIEW_SINGLEAPPLICATION);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_STYLE)) {
        g_pOptions->getComboBox(ui->comboBoxViewStyle, XOptions::ID_VIEW_STYLE);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_QSS)) {
        g_pOptions->getComboBox(ui->comboBoxViewQss, XOptions::ID_VIEW_QSS);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_LANG)) {
        g_pOptions->getComboBox(ui->comboBoxViewLanguage, XOptions::ID_VIEW_LANG);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_SHOWLOGO)) {
        g_pOptions->getCheckBox(ui->checkBoxViewShowLogo, XOptions::ID_VIEW_SHOWLOGO);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_CONTROLS)) {
        g_pOptions->getLineEdit(ui->lineEditViewFontControls, XOptions::ID_VIEW_FONT_CONTROLS);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_TABLEVIEWS)) {
        g_pOptions->getLineEdit(ui->lineEditViewFontTables, XOptions::ID_VIEW_FONT_TABLEVIEWS);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_TREEVIEWS)) {
        g_pOptions->getLineEdit(ui->lineEditViewFontTrees, XOptions::ID_VIEW_FONT_TREEVIEWS);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_TEXTEDITS)) {
        g_pOptions->getLineEdit(ui->lineEditViewFontTextEdits, XOptions::ID_VIEW_FONT_TEXTEDITS);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_HEX_FONT)) {
        g_pOptions->getLineEdit(ui->lineEditHexFont, XOptions::ID_HEX_FONT);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_DISASM_FONT)) {
        g_pOptions->getLineEdit(ui->lineEditDisasmFont, XOptions::ID_DISASM_FONT);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_FILE_SAVELASTDIRECTORY)) {
        g_pOptions->getCheckBox(ui->checkBoxFileSaveLastDirectory, XOptions::ID_FILE_SAVELASTDIRECTORY);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_FILE_SAVEBACKUP)) {
        g_pOptions->getCheckBox(ui->checkBoxFileSaveBackup, XOptions::ID_FILE_SAVEBACKUP);
    }

    if (g_pOptions->isIDPresent(XOptions::ID_FILE_SAVERECENTFILES)) {
        g_pOptions->getCheckBox(ui->checkBoxFileSaveHistory, XOptions::ID_FILE_SAVERECENTFILES);
    }
    if (g_pOptions->isIDPresent(XOptions::ID_FILE_SETENV)) {
        g_pOptions->getCheckBox(ui->checkBoxFileSetEnvVar, XOptions::ID_FILE_SETENV);

        QString appDir = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath();

#ifdef Q_OS_WIN
        QString formattedDir = QDir::toNativeSeparators(appDir);
#else
        QString formattedDir = QDir(appDir).absolutePath();  // normalized for Unix-like systems
#endif

        if (ui->checkBoxFileSetEnvVar->isChecked()) {
            g_pOptions->appendToUserPathVariable(formattedDir);
            qDebug() << "[Save] Appended to user PATH:" << formattedDir;
        } else {
            g_pOptions->removeFromUserPathVariable(formattedDir);
            qDebug() << "[Save] Removed from user PATH:" << formattedDir;
        }
    }

#ifdef Q_OS_WIN
    if (g_pOptions->isIDPresent(XOptions::ID_FILE_ENABLETRAYMONITORING)) {
        g_pOptions->getCheckBox(ui->checkBoxEnableTrayMonitoring, XOptions::ID_FILE_ENABLETRAYMONITORING);
        qDebug() << "[Save] Tray monitoring state saved:" << g_pOptions->getValue(XOptions::ID_FILE_ENABLETRAYMONITORING).toBool();
    }
#endif
    g_pOptions->save();
}

void XOptionsWidget::reload()
{
    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_STAYONTOP)) {
        g_pOptions->setCheckBox(ui->checkBoxViewStayOnTop, XOptions::ID_VIEW_STAYONTOP);
    } else {
        ui->checkBoxViewStayOnTop->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_SINGLEAPPLICATION)) {
        g_pOptions->setCheckBox(ui->checkBoxViewSingleApplication, XOptions::ID_VIEW_SINGLEAPPLICATION);
    } else {
        ui->checkBoxViewSingleApplication->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_STYLE)) {
        g_pOptions->setComboBox(ui->comboBoxViewStyle, XOptions::ID_VIEW_STYLE);
    } else {
        ui->groupBoxViewStyle->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_QSS)) {
        g_pOptions->setComboBox(ui->comboBoxViewQss, XOptions::ID_VIEW_QSS);
    } else {
        ui->groupBoxViewQss->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_LANG)) {
        g_pOptions->setComboBox(ui->comboBoxViewLanguage, XOptions::ID_VIEW_LANG);
    } else {
        ui->groupBoxViewLanguage->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_SHOWLOGO)) {
        g_pOptions->setCheckBox(ui->checkBoxViewShowLogo, XOptions::ID_VIEW_SHOWLOGO);
    } else {
        ui->checkBoxViewShowLogo->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_CONTROLS)) {
        g_pOptions->setLineEdit(ui->lineEditViewFontControls, XOptions::ID_VIEW_FONT_CONTROLS);
    } else {
        ui->groupBoxViewFontControls->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_TEXTEDITS)) {
        g_pOptions->setLineEdit(ui->lineEditViewFontTextEdits, XOptions::ID_VIEW_FONT_TEXTEDITS);
    } else {
        ui->groupBoxViewFontTextEdits->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_TABLEVIEWS)) {
        g_pOptions->setLineEdit(ui->lineEditViewFontTables, XOptions::ID_VIEW_FONT_TABLEVIEWS);
    } else {
        ui->groupBoxViewFontTables->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_VIEW_FONT_TREEVIEWS)) {
        g_pOptions->setLineEdit(ui->lineEditViewFontTrees, XOptions::ID_VIEW_FONT_TREEVIEWS);
    } else {
        ui->groupBoxViewFontTrees->hide();
    }
    if (g_pOptions->isIDPresent(XOptions::ID_HEX_FONT)) {
        g_pOptions->setLineEdit(ui->lineEditHexFont, XOptions::ID_HEX_FONT);
    } else {
        ui->groupBoxHexFont->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_DISASM_FONT)) {
        g_pOptions->setLineEdit(ui->lineEditDisasmFont, XOptions::ID_DISASM_FONT);
    } else {
        ui->groupBoxDisasmFont->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_FILE_SAVELASTDIRECTORY)) {
        g_pOptions->setCheckBox(ui->checkBoxFileSaveLastDirectory, XOptions::ID_FILE_SAVELASTDIRECTORY);
    } else {
        ui->checkBoxFileSaveLastDirectory->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_FILE_SAVEBACKUP)) {
        g_pOptions->setCheckBox(ui->checkBoxFileSaveBackup, XOptions::ID_FILE_SAVEBACKUP);
    } else {
        ui->checkBoxFileSaveBackup->hide();
    }

    if (g_pOptions->isIDPresent(XOptions::ID_FILE_SAVERECENTFILES)) {
        g_pOptions->setCheckBox(ui->checkBoxFileSaveHistory, XOptions::ID_FILE_SAVERECENTFILES);
    } else {
        ui->checkBoxFileSaveHistory->hide();
    }

    QString appDir = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath();

#ifdef Q_OS_WIN
    QString formattedDir = QDir::toNativeSeparators(appDir);
#else
    QString formattedDir = QDir(appDir).absolutePath();
#endif


    ui->checkBoxFileSetEnvVar->setChecked(
        g_pOptions->isPathInUserEnvironment(formattedDir)
        );

    if (g_pOptions->isIDPresent(XOptions::ID_FILE_CONTEXT)) {
#ifdef Q_OS_WIN
        // bool bAdmin = g_pOptions->checkContext(g_sApplicationDisplayName, g_pOptions->getValue(XOptions::ID_FILE_CONTEXT).toString(), XOptions::USERROLE_ADMIN);
        // bool bUser = g_pOptions->checkContext(g_sApplicationDisplayName, g_pOptions->getValue(XOptions::ID_FILE_CONTEXT).toString(), XOptions::USERROLE_NORMAL);

        // if (bAdmin &&
        //     (!bUser)) {
        //     g_userRole = XOptions::USERROLE_ADMIN;
        // }
        ui->checkBoxFileContext->setChecked(
            g_pOptions->checkContext(
                g_sApplicationDisplayName,
                g_pOptions->getValue(XOptions::ID_FILE_CONTEXT).toString(),
                g_userRole
                )
            );
        if (g_pOptions->isIDPresent(XOptions::ID_FILE_ENABLETRAYMONITORING)) {
            bool bTrayStored = g_pOptions->getValue(XOptions::ID_FILE_ENABLETRAYMONITORING).toBool();
            bool bTrayRunning = g_pOptions->isTrayMonitoringActive();

            ui->checkBoxEnableTrayMonitoring->blockSignals(true);
            ui->checkBoxEnableTrayMonitoring->setChecked(bTrayStored);
            ui->checkBoxEnableTrayMonitoring->blockSignals(false);

            if (bTrayStored && !bTrayRunning) {
                qDebug() << "[Reload] Tray was enabled in config but not running — setting up without toast.";
                g_pOptions->setupTrayIconAndDownloadMonitoring(g_pMainWindow, false);
            }

            if (bTrayStored) {
                qDebug() << "[Reload] Rebinding tray callbacks to ensure restore works.";
                g_pOptions->registerTrayCallbacks(false);
            }
        } else {
            ui->checkBoxEnableTrayMonitoring->hide();
            qDebug() << "[Reload] Tray monitoring not configured — hiding checkbox.";
        }
#endif
    } else {
        ui->checkBoxFileContext->hide();
       // ui->checkBoxFileSetEnvVar->hide();
        ui->checkBoxEnableTrayMonitoring->hide();
    }
}

void XOptionsWidget::on_listWidgetOptions_currentRowChanged(int nCurrentRow)
{
    if (nCurrentRow < ui->stackedWidgetOptions->count()) {
        qint32 nIndex = ui->listWidgetOptions->item(nCurrentRow)->data(Qt::UserRole).toInt();
        ui->stackedWidgetOptions->setCurrentIndex(nIndex);
    }
}

void XOptionsWidget::on_checkBoxFileContext_toggled(bool bChecked)
{
    if (g_pOptions->isIDPresent(XOptions::ID_FILE_CONTEXT)) {
#ifdef Q_OS_WIN
        if (g_pOptions->checkContext(g_sApplicationDisplayName, g_pOptions->getValue(XOptions::ID_FILE_CONTEXT).toString(), g_userRole) != bChecked) {
            bool bSuccess = false;

            if (bChecked) {
                bSuccess = g_pOptions->registerContext(g_sApplicationDisplayName, g_pOptions->getValue(XOptions::ID_FILE_CONTEXT).toString(), qApp->applicationFilePath(),
                                                       g_userRole);
            } else {
                bSuccess = g_pOptions->clearContext(g_sApplicationDisplayName, g_pOptions->getValue(XOptions::ID_FILE_CONTEXT).toString(), g_userRole);
            }

            if (!bSuccess) {
                QMessageBox::critical(this, tr("Error"), tr("Please run the program as an administrator"));

                ui->checkBoxFileContext->setChecked(!bChecked);
            }
        }
#else
        Q_UNUSED(bChecked)
#endif
    }
}

void XOptionsWidget::on_checkBoxEnableTrayMonitoring_toggled(bool bChecked)
{
#ifdef Q_OS_WIN
    qDebug() << "[Tray Monitor] Toggled to:" << bChecked;
    g_pOptions->setValue(XOptions::ID_FILE_ENABLETRAYMONITORING, bChecked);
    //g_pOptions->save();

    if (bChecked) {
        if (!g_pOptions->isTrayMonitoringActive()) {
            g_pOptions->setupTrayIconAndDownloadMonitoring(g_pMainWindow, true);
            qDebug() << "[Tray Monitor] Setup triggered.";
        } else {
            qDebug() << "[Tray Monitor] Already active.";
            g_pOptions->registerTrayCallbacks(true);
        }
    } else {
        g_pOptions->cleanupTrayMonitoring();
    }
#else
    Q_UNUSED(bChecked)
#endif
}

void XOptionsWidget::on_checkBoxFileSetEnvVar_toggled(bool bChecked)
{
    QString appDir = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath();

#ifdef Q_OS_WIN
    QString formattedDir = QDir::toNativeSeparators(appDir);
#else
    QString formattedDir = QDir(appDir).absolutePath();
#endif

    qDebug() << "[EnvCheckbox] Toggled:" << (bChecked ? "Checked" : "Unchecked");
    qDebug() << "[EnvCheckbox] Directory:" << formattedDir;

    bool isCurrentlySet = g_pOptions->isPathInUserEnvironment(formattedDir);

    if (bChecked) {
        if (!isCurrentlySet) {
            g_pOptions->appendToUserPathVariable(formattedDir);
            qDebug() << "[EnvPath] Appended to Path:" << formattedDir;
        } else {
            qDebug() << "[EnvPath] Already present. No action taken.";
        }
    } else {
        if (isCurrentlySet) {
            g_pOptions->removeFromUserPathVariable(formattedDir);
            qDebug() << "[EnvPath] Removed from Path:" << formattedDir;
        } else {
            qDebug() << "[EnvPath] Not present. Nothing to remove.";
        }
    }
}

void XOptionsWidget::on_toolButtonViewFontControls_clicked()
{
    XOptions::handleFontButton(this, ui->lineEditViewFontControls);
}

void XOptionsWidget::on_toolButtonViewFontTables_clicked()
{
    XOptions::handleFontButton(this, ui->lineEditViewFontTables);
}

void XOptionsWidget::on_toolButtonViewFontTrees_clicked()
{
    XOptions::handleFontButton(this, ui->lineEditViewFontTrees);
}

void XOptionsWidget::on_toolButtonHexFont_clicked()
{
    XOptions::handleFontButton(this, ui->lineEditHexFont);
}

void XOptionsWidget::on_toolButtonDisasmFont_clicked()
{
    XOptions::handleFontButton(this, ui->lineEditDisasmFont);
}

void XOptionsWidget::on_toolButtonViewFontTextEdits_clicked()
{
    XOptions::handleFontButton(this, ui->lineEditViewFontTextEdits);
}

void XOptionsWidget::on_pushButtonDefault_clicked()
{
    g_pOptions->resetToDefault();

    emit reloadSignal();
}

void XOptionsWidget::on_pushButtonOK_clicked()
{
    emit saveSignal();

    if (g_pOptions->isRestartNeeded()) {
        QMessageBox::information(this, tr("Information"), tr("Please restart the application"));
    }

    g_pParent->close();
}

void XOptionsWidget::on_pushButtonCancel_clicked()
{
    g_pParent->close();
}

void XOptionsWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
