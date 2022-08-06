/* Copyright (c) 2020-2022 hors<horsicq@gmail.com>
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
#ifndef XOPTIONSWIDGET_H
#define XOPTIONSWIDGET_H

#include <QFontDialog>
#include <QWidget>
#include "xoptions.h"

namespace Ui {
class XOptionsWidget;
}

class XOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XOptionsWidget(QWidget *pParent=nullptr);
    ~XOptionsWidget();

    void setOptions(XOptions *pOptions,QString sApplicationDisplayName);
    void addListRecord(QString sTitle,qint32 nIndex);
    void addPage(QWidget *pWidget,QString sTitle);
    void setCurrentPage(qint32 nPage); // TODO remove
    void setCurrentPage(XOptions::GROUPID groupId);

public slots:
    void save();
    void reload();

private slots:
    void on_listWidgetOptions_currentRowChanged(int nCurrentRow);
    void on_checkBoxFileContext_toggled(bool bChecked);
    void on_toolButtonViewFont_clicked();
    void on_pushButtonDefault_clicked();
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();

signals:
    void saveSignal();
    void reloadSignal();

private:
    Ui::XOptionsWidget *ui;
    QWidget *g_pParent;
    XOptions *g_pOptions;
    QString g_sApplicationDisplayName;
};

#endif // XOPTIONSWIDGET_H
