/* Copyright (c) 2020-2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "dialogviewcolors.h"

#include "ui_dialogviewcolors.h"

DialogViewColors::DialogViewColors(QWidget *pParent) : XShortcutsDialog(pParent, false), ui(new Ui::DialogViewColors)
{
    ui->setupUi(this);
}

DialogViewColors::~DialogViewColors()
{
    delete ui;
}

void DialogViewColors::adjustView()
{
}

void DialogViewColors::setOptions(XOptions *pOptions, const QList<RECORD> &listRecords, const QString &sTitle)
{
    g_pOptions = pOptions;

    qint32 nNumberOfRecords = listRecords.count();

    ui->tableWidgetColors->clear();

    ui->tableWidgetColors->setColumnCount(6);
    ui->tableWidgetColors->setRowCount(nNumberOfRecords);

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        addRecord(i, listRecords.at(i));
    }

    //    ui->tableWidgetColors->setColumnWidth(COLUMN_TEXT_COLOR,80);
    ui->tableWidgetColors->setColumnWidth(COLUMN_TEXT_COLOR_REMOVE, 20);
    //    ui->tableWidgetColors->setColumnWidth(COLUMN_BACKGROUND_COLOR,80);
    ui->tableWidgetColors->setColumnWidth(COLUMN_BACKGROUND_COLOR_REMOVE, 20);

    setWindowTitle(sTitle);
}

void DialogViewColors::save()
{
    QMapIterator<XOptions::ID, QString> iter(g_mapColors);

    while (iter.hasNext()) {
        iter.next();

        XOptions::ID id = iter.key();
        QString sValue = iter.value();

        g_pOptions->setValue(id, sValue);
    }
}

void DialogViewColors::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogViewColors::addRecord(qint32 nRow, const QString &sGroup, const QString &sText, XOptions::ID id)
{
    QPushButton *pButtonColor = new QPushButton;
    pButtonColor->setText(tr("Color"));
    pButtonColor->setProperty("ROW", nRow);
    pButtonColor->setProperty("COLUMN", COLUMN_TEXT_COLOR);
    pButtonColor->setProperty("ID", id);

    connect(pButtonColor, SIGNAL(clicked(bool)), this, SLOT(pushButtonSlot()));

    ui->tableWidgetColors->setCellWidget(nRow, COLUMN_TEXT_COLOR, pButtonColor);

    QPushButton *pButtonColorRemove = new QPushButton;
    pButtonColorRemove->setText(QString("X"));
    pButtonColorRemove->setProperty("ROW", nRow);
    pButtonColorRemove->setProperty("COLUMN", COLUMN_TEXT_COLOR_REMOVE);
    pButtonColorRemove->setProperty("ID", id);

    connect(pButtonColorRemove, SIGNAL(clicked(bool)), this, SLOT(pushButtonSlot()));

    ui->tableWidgetColors->setCellWidget(nRow, COLUMN_TEXT_COLOR_REMOVE, pButtonColorRemove);

    QPushButton *pButtonBackgroundColor = new QPushButton;
    pButtonBackgroundColor->setText(tr("Background"));
    pButtonBackgroundColor->setProperty("ROW", nRow);
    pButtonBackgroundColor->setProperty("COLUMN", COLUMN_BACKGROUND_COLOR);
    pButtonBackgroundColor->setProperty("ID", id);

    connect(pButtonBackgroundColor, SIGNAL(clicked(bool)), this, SLOT(pushButtonSlot()));

    ui->tableWidgetColors->setCellWidget(nRow, COLUMN_BACKGROUND_COLOR, pButtonBackgroundColor);

    QPushButton *pButtonBackgroundColorRemove = new QPushButton;
    pButtonBackgroundColorRemove->setText(QString("X"));
    pButtonBackgroundColorRemove->setProperty("ROW", nRow);
    pButtonBackgroundColorRemove->setProperty("COLUMN", COLUMN_BACKGROUND_COLOR_REMOVE);
    pButtonBackgroundColorRemove->setProperty("ID", id);

    connect(pButtonBackgroundColorRemove, SIGNAL(clicked(bool)), this, SLOT(pushButtonSlot()));

    ui->tableWidgetColors->setItem(nRow, COLUMN_GROUP, new QTableWidgetItem(sGroup));

    ui->tableWidgetColors->setCellWidget(nRow, COLUMN_BACKGROUND_COLOR_REMOVE, pButtonBackgroundColorRemove);

    QLineEdit *pLineEdit = new QLineEdit;
    pLineEdit->setText(sText);
    pLineEdit->setProperty("ROW", nRow);
    pLineEdit->setProperty("COLUMN", COLUMN_STRING);
    pLineEdit->setProperty("ID", id);
    pLineEdit->setReadOnly(true);

    ui->tableWidgetColors->setCellWidget(nRow, COLUMN_STRING, pLineEdit);

    g_mapColors.insert(id, g_pOptions->getValue(id).toString());

    updateRow(nRow);
}

void DialogViewColors::addRecord(qint32 nRow, const RECORD &record)
{
    addRecord(nRow, record.sGroup, record.sText, record.id);
}

void DialogViewColors::updateRow(qint32 nRow)
{
    XOptions::ID id = (XOptions::ID)(ui->tableWidgetColors->cellWidget(nRow, COLUMN_TEXT_COLOR)->property("ID").toUInt());

    QString sColor = g_mapColors.value(id);
    QString sTextColor = sColor.section("|", 0, 0);
    QString sBackgroundColor = sColor.section("|", 1, 1);

    QLineEdit *pLineEdit = (QLineEdit *)(ui->tableWidgetColors->cellWidget(nRow, COLUMN_STRING));

    pLineEdit->setStyleSheet(QString("color: %1;  background-color: %2").arg(sTextColor, sBackgroundColor));

    ((QPushButton *)(ui->tableWidgetColors->cellWidget(nRow, COLUMN_TEXT_COLOR_REMOVE)))->setEnabled(sTextColor != "");
    ((QPushButton *)(ui->tableWidgetColors->cellWidget(nRow, COLUMN_BACKGROUND_COLOR_REMOVE)))->setEnabled(sBackgroundColor != "");
}

void DialogViewColors::on_pushButtonOK_clicked()
{
    save();

    this->close();
}

void DialogViewColors::pushButtonSlot()
{
    QPushButton *pPushButton = qobject_cast<QPushButton *>(sender());

    if (pPushButton) {
        qint32 nRow = pPushButton->property("ROW").toInt();
        qint32 nColumn = pPushButton->property("COLUMN").toInt();
        XOptions::ID id = (XOptions::ID)(pPushButton->property("ID").toUInt());

        QString sColor = g_mapColors.value(id);
        QString sTextColor = sColor.section("|", 0, 0);
        QString sBackgroundColor = sColor.section("|", 1, 1);

        if (nColumn == COLUMN_TEXT_COLOR) {
            QColor color;
            color.setNamedColor(sTextColor);

            color = XOptions::getColorDialog(this, tr("Color"), color);

            sTextColor = color.name();
        } else if (nColumn == COLUMN_BACKGROUND_COLOR) {
            QColor color;
            color.setNamedColor(sBackgroundColor);

            color = XOptions::getColorDialog(this, tr("Background"), color);

            sBackgroundColor = color.name();
        } else if (nColumn == COLUMN_TEXT_COLOR_REMOVE) {
            sTextColor = "";
        } else if (nColumn == COLUMN_BACKGROUND_COLOR_REMOVE) {
            sBackgroundColor = "";
        }

        if ((sTextColor != "") || (sBackgroundColor != "")) {
            sColor = QString("%1|%2").arg(sTextColor, sBackgroundColor);
        } else {
            sColor = "";
        }

        g_mapColors.insert(id, sColor);

        updateRow(nRow);
    }
}

void DialogViewColors::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
