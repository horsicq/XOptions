// Copyright (c) 2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef XOPTIONS_H
#define XOPTIONS_H

#include <QObject>
#include <QMap>
#include <QDir>
#include <QSettings>
#include <QWidget>

class XOptions : public QObject
{
    Q_OBJECT
public:
    enum ID
    {
        ID_STAYONTOP=0,
        ID_SCANAFTEROPEN,
        ID_SAVELASTDIRECTORY,
        ID_LASTDIRECTORY,
        ID_SAVEBACKUP
    };

    explicit XOptions(QObject *parent=nullptr);
    void setFilePath(QString sFilePath);
    void setValueIDs(QList<ID> listValueIDs);
    void load();
    void save();
    QVariant getValue(ID id);
    void setValue(ID id,QVariant value);
    static QString idToString(ID id);
    QString getLastDirectory();
    void setLastDirectory(QString sValue);
    void adjustStayOnTop(QWidget *pWidget); // TODO def GUI

private:
    QString sFilePath;
    QList<ID> listValueIDs;
    QMap<ID,QVariant> mapValues;
};

#endif // XOPTIONS_H
