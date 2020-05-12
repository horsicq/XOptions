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
#include "xoptions.h"

XOptions::XOptions(QObject *parent) : QObject(parent)
{

}

void XOptions::setName(QString sName)
{
    this->sName=sName;
    this->sFilePath=QApplication::applicationDirPath()+QDir::separator()+QString("%1.ini").arg(sName);
}

void XOptions::setValueIDs(QList<ID> listVariantIDs)
{
    this->listValueIDs=listVariantIDs;
}

void XOptions::load()
{
    QSettings settings(sFilePath,QSettings::IniFormat);

    int nCount=listValueIDs.count();

    for(int i=0;i<nCount;i++)
    {
        ID id=listValueIDs.at(i);
        QString sName=idToString(id);
        QVariant varDefault;

        switch(id)
        {
            case ID_STAYONTOP:              varDefault=false;       break;
            case ID_SCANAFTEROPEN:          varDefault=true;        break;
            case ID_SAVELASTDIRECTORY:      varDefault=true;        break;
            case ID_LASTDIRECTORY:          varDefault="";          break;
            case ID_SAVEBACKUP:             varDefault=true;        break;
        }

        mapValues.insert(id,settings.value(sName,varDefault));
    }

    QString sLastDirectory=mapValues.value(ID_LASTDIRECTORY).toString();

    if(sLastDirectory!="")
    {
        if(!QDir(sLastDirectory).exists())
        {
            mapValues.insert(ID_LASTDIRECTORY,"");
        }
    }
}

void XOptions::save()
{
    QSettings settings(sFilePath,QSettings::IniFormat);

    int nCount=listValueIDs.count();

    for(int i=0;i<nCount;i++)
    {
        ID id=listValueIDs.at(i);
        QString sName=idToString(id);
        settings.setValue(sName,mapValues.value(id));
    }
}

QVariant XOptions::getValue(XOptions::ID id)
{
    return mapValues.value(id);
}

void XOptions::setValue(XOptions::ID id, QVariant value)
{
    mapValues.insert(id,value);
}

QString XOptions::idToString(ID id)
{
    QString sResult="Unknown";

    switch(id)
    {
        case ID_STAYONTOP:                  sResult=QString("StayOnTop");                   break;
        case ID_SCANAFTEROPEN:              sResult=QString("ScanAfterOpen");               break;
        case ID_SAVELASTDIRECTORY:          sResult=QString("SaveLastDirectory");           break;
        case ID_LASTDIRECTORY:              sResult=QString("LastDirectory");               break;
        case ID_SAVEBACKUP:                 sResult=QString("SaveBackup");                  break;
        case ID_STYLE:                      sResult=QString("Style");                       break;
    }

    return sResult;
}

QString XOptions::getLastDirectory()
{
    QString sResult;

    bool bSaveLastDirectory=getValue(ID_SAVELASTDIRECTORY).toBool();
    QString sLastDirectory=getValue(ID_LASTDIRECTORY).toString();

    if(bSaveLastDirectory&&QDir().exists(sLastDirectory))
    {
        sResult=sLastDirectory;
    }

    return sResult;
}

void XOptions::setLastDirectory(QString sValue)
{
    if(getValue(ID_SAVELASTDIRECTORY).toBool())
    {
        setValue(ID_LASTDIRECTORY,sValue);
    }
}

void XOptions::adjustStayOnTop(QWidget *pWidget)
{
    Qt::WindowFlags wf=pWidget->windowFlags();

    if(getValue(ID_STAYONTOP).toBool())
    {
        wf|=Qt::WindowStaysOnTopHint;
    }
    else
    {
        wf&=~(Qt::WindowStaysOnTopHint);
    }

    pWidget->setWindowFlags(wf);

    pWidget->show();
}

void XOptions::setCheckBox(QCheckBox *pCheckBox, XOptions::ID id)
{
    pCheckBox->setChecked(getValue(id).toBool());
}

void XOptions::getCheckBox(QCheckBox *pCheckBox, XOptions::ID id)
{
    setValue(id,pCheckBox->isChecked());
}

void XOptions::setComboBox(QComboBox *pComboBox, XOptions::ID id)
{
    QSignalBlocker signalBlocker(pComboBox);

    pComboBox->clear();
    pComboBox->addItem("","");

    QString sValue=getValue(id).toString();

    QStringList sl=QStyleFactory::keys();

    int nCount=sl.count();

    int nIndex=-1;

    for(int i=0;i<nCount;i++)
    {
        QString sRecord=sl.at(i);
        pComboBox->addItem(sRecord,sRecord);

        if(sValue==sRecord)
        {
            nIndex=i+1;
        }
    }

    if(nIndex!=-1)
    {
        pComboBox->setCurrentIndex(nIndex);
    }
}

void XOptions::getComboBox(QComboBox *pComboBox, XOptions::ID id)
{
    setValue(id,pComboBox->currentData());
}

void XOptions::adjustApplicationView()
{
    XOptions xOptions;

    QList<XOptions::ID> listIDs;

    listIDs.append(XOptions::ID_STYLE);

    xOptions.setValueIDs(listIDs);
    xOptions.load();

    QString sStyle=xOptions.getValue(XOptions::ID_STYLE).toString();

    if(sStyle!="")
    {
        QApplication::setStyle(QStyleFactory::create(sStyle));
    }
}
