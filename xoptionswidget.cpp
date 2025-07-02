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
    const QList<QPair<QObject *, XOptions::ID>> optionMappings = {
        { ui->checkBoxViewStayOnTop, XOptions::ID_VIEW_STAYONTOP },
        { ui->checkBoxViewSingleApplication, XOptions::ID_VIEW_SINGLEAPPLICATION },
        { ui->comboBoxViewStyle, XOptions::ID_VIEW_STYLE },
        { ui->comboBoxViewQss, XOptions::ID_VIEW_QSS },
        { ui->comboBoxViewLanguage, XOptions::ID_VIEW_LANG },
        { ui->checkBoxViewShowLogo, XOptions::ID_VIEW_SHOWLOGO },
        { ui->lineEditViewFontControls, XOptions::ID_VIEW_FONT_CONTROLS },
        { ui->lineEditViewFontTables, XOptions::ID_VIEW_FONT_TABLEVIEWS },
        { ui->lineEditViewFontTrees, XOptions::ID_VIEW_FONT_TREEVIEWS },
        { ui->lineEditViewFontTextEdits, XOptions::ID_VIEW_FONT_TEXTEDITS },
        { ui->lineEditHexFont, XOptions::ID_HEX_FONT },
        { ui->lineEditDisasmFont, XOptions::ID_DISASM_FONT },
        { ui->checkBoxFileSaveLastDirectory, XOptions::ID_FILE_SAVELASTDIRECTORY },
        { ui->checkBoxFileSaveBackup, XOptions::ID_FILE_SAVEBACKUP },
        { ui->checkBoxFileSaveHistory, XOptions::ID_FILE_SAVERECENTFILES }
    };

    for (const auto &mapping : optionMappings) {
        if (g_pOptions->isIDPresent(mapping.second)) {
            if (auto *checkBox = qobject_cast<QCheckBox *>(mapping.first)) {
                g_pOptions->getCheckBox(checkBox, mapping.second);
            } else if (auto *comboBox = qobject_cast<QComboBox *>(mapping.first)) {
                g_pOptions->getComboBox(comboBox, mapping.second);
            } else if (auto *lineEdit = qobject_cast<QLineEdit *>(mapping.first)) {
                g_pOptions->getLineEdit(lineEdit, mapping.second);
            }
        }
    }

    g_pOptions->save();
}

void XOptionsWidget::reload()
{
    const QList<QPair<QPair<QObject *, XOptions::ID>, QObject *>> optionMappings = {
        { { ui->checkBoxViewStayOnTop, XOptions::ID_VIEW_STAYONTOP }, ui->checkBoxViewStayOnTop },
        { { ui->checkBoxViewSingleApplication, XOptions::ID_VIEW_SINGLEAPPLICATION }, ui->checkBoxViewSingleApplication },
        { { ui->comboBoxViewStyle, XOptions::ID_VIEW_STYLE }, ui->groupBoxViewStyle },
        { { ui->comboBoxViewQss, XOptions::ID_VIEW_QSS }, ui->groupBoxViewQss },
        { { ui->comboBoxViewLanguage, XOptions::ID_VIEW_LANG }, ui->groupBoxViewLanguage },
        { { ui->checkBoxViewShowLogo, XOptions::ID_VIEW_SHOWLOGO }, ui->checkBoxViewShowLogo },
        { { ui->lineEditViewFontControls, XOptions::ID_VIEW_FONT_CONTROLS }, ui->groupBoxViewFontControls },
        { { ui->lineEditViewFontTextEdits, XOptions::ID_VIEW_FONT_TEXTEDITS }, ui->groupBoxViewFontTextEdits },
        { { ui->lineEditViewFontTables, XOptions::ID_VIEW_FONT_TABLEVIEWS }, ui->groupBoxViewFontTables },
        { { ui->lineEditViewFontTrees, XOptions::ID_VIEW_FONT_TREEVIEWS }, ui->groupBoxViewFontTrees },
        { { ui->lineEditHexFont, XOptions::ID_HEX_FONT }, ui->groupBoxHexFont },
        { { ui->lineEditDisasmFont, XOptions::ID_DISASM_FONT }, ui->groupBoxDisasmFont },
        { { ui->checkBoxFileSaveLastDirectory, XOptions::ID_FILE_SAVELASTDIRECTORY }, ui->checkBoxFileSaveLastDirectory },
        { { ui->checkBoxFileSaveBackup, XOptions::ID_FILE_SAVEBACKUP }, ui->checkBoxFileSaveBackup },
        { { ui->checkBoxFileSaveHistory, XOptions::ID_FILE_SAVERECENTFILES }, ui->checkBoxFileSaveHistory }
    };

    for (const auto &mapping : optionMappings) {
        QObject *control = mapping.first.first;
        XOptions::ID optionId = mapping.first.second;
        QObject *hideableWidget = mapping.second;

        if (g_pOptions->isIDPresent(optionId)) {
            if (auto *checkBox = qobject_cast<QCheckBox *>(control)) {
                g_pOptions->setCheckBox(checkBox, optionId);
            } else if (auto *comboBox = qobject_cast<QComboBox *>(control)) {
                g_pOptions->setComboBox(comboBox, optionId);
            } else if (auto *lineEdit = qobject_cast<QLineEdit *>(control)) {
                g_pOptions->setLineEdit(lineEdit, optionId);
            }
        } else {
            if (hideableWidget) {
                hideableWidget->setProperty("visible", false);
            }
        }
    }

#ifdef Q_OS_WIN
    if (g_pOptions->isIDPresent(XOptions::ID_FILE_CONTEXT)) {
        ui->checkBoxFileContext->setChecked(
            g_pOptions->checkContext(g_sApplicationDisplayName,
                                     g_pOptions->getValue(XOptions::ID_FILE_CONTEXT).toString(),
                                     g_userRole));
    } else {
        ui->checkBoxFileContext->hide();
    }
#endif
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
