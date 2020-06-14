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
    bIsRestartNeeded=false;
}

void XOptions::setName(QString sName)
{
    this->sName=sName;
    this->sFilePath=getApplicationDataPath()+QDir::separator()+QString("%1").arg(sName);
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
            case ID_STYLE:                  varDefault="Fusion";    break;
            case ID_LANG:                   varDefault="System";    break;
            case ID_QSS:                    varDefault="orange";    break;
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
    if( (id==ID_STYLE)||
        (id==ID_LANG)||
        (id==ID_QSS))
    {
        QVariant varOld=mapValues.value(id);

        if(value!=varOld)
        {
            bIsRestartNeeded=true;
        }
    }

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
        case ID_LANG:                       sResult=QString("Lang");                        break;
        case ID_QSS:                        sResult=QString("Qss");                         break;
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

    if(id==ID_STYLE)
    {
        QStringList sl=QStyleFactory::keys();

        int nCount=sl.count();

        for(int i=0;i<nCount;i++)
        {
            QString sRecord=sl.at(i);
            pComboBox->addItem(sRecord,sRecord);
        }
    }
    else if(id==ID_LANG)
    {
        pComboBox->addItem("System","System");

        QList<QString> listRecords=getAllFilesFromDirectory(getApplicationLangPath(),"*.qm");

        int nCount=listRecords.count();

        for(int i=0;i<nCount;i++)
        {
            QFileInfo fi(listRecords.at(i));

            QString sRecord=fi.baseName();

            QLocale locale(sRecord.section("_",1,-1));
            QString sLocale=locale.nativeLanguageName();

            if(sRecord.count("_")==2)
            {
                sLocale+=QString("(%1)").arg(locale.nativeCountryName());
            }

            pComboBox->addItem(sLocale,sRecord);
        }
    }
    else if(id==ID_QSS)
    {
        QList<QString> listRecords=getAllFilesFromDirectory(getApplicationQssPath(),"*.qss");

        int nCount=listRecords.count();

        for(int i=0;i<nCount;i++)
        {
            QFileInfo fi(listRecords.at(i));

            QString sRecord=fi.baseName();

            pComboBox->addItem(sRecord,sRecord);
        }
    }

    int nCount=pComboBox->count();
    int nIndex=-1;

    for(int i=0;i<nCount;i++)
    {
        if(sValue==pComboBox->itemData(i,Qt::UserRole))
        {
            nIndex=i;
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

bool XOptions::isSaveBackup()
{
    return getValue(XOptions::ID_SAVEBACKUP).toBool();
}

bool XOptions::isRestartNeeded()
{
    return bIsRestartNeeded;
}

void XOptions::adjustApplicationView(QString sOptionName, QString sTranslationName)
{
    XOptions xOptions;

    xOptions.setName(sOptionName);

    QList<XOptions::ID> listIDs;

    listIDs.append(XOptions::ID_STYLE);
    listIDs.append(XOptions::ID_LANG);
    listIDs.append(XOptions::ID_QSS);

    xOptions.setValueIDs(listIDs);
    xOptions.load();

    QString sStyle=xOptions.getValue(XOptions::ID_STYLE).toString();

    if(sStyle!="")
    {
        qApp->setStyle(QStyleFactory::create(sStyle));
    }

    QTranslator *pTranslator=new QTranslator; // Important
    QString sLang=xOptions.getValue(XOptions::ID_LANG).toString();
    QString sLangsPath=getApplicationLangPath();

    bool bLoad=false;

    if(sLang=="System")
    {
        QLocale locale=QLocale::system();
        if(locale!=QLocale::English)
        {
            bLoad=pTranslator->load(locale,sTranslationName,"_",sLangsPath,".qm");
        }
    }
    else if(sLang!="")
    {
        bLoad=pTranslator->load(sLang,sLangsPath);
    }

    if(bLoad)
    {
        qApp->installTranslator(pTranslator);
    }

    QString sQss=xOptions.getValue(XOptions::ID_QSS).toString();

    if(sQss!="")
    {
        QString sQssFileName=getApplicationQssPath()+QDir::separator()+QString("%1.qss").arg(sQss);

        if(QFile::exists(sQssFileName))
        {
            QFile file;
            file.setFileName(sQssFileName);

            if(file.open(QIODevice::ReadOnly))
            {
                QByteArray baQss=file.readAll();
                qApp->setStyleSheet(baQss.data());
                file.close();
            }
        }
    }
}

QString XOptions::getApplicationLangPath()
{
    QString sResult;

    sResult=getApplicationDataPath()+QDir::separator()+"lang";

    return sResult;
}

QString XOptions::getApplicationQssPath()
{
    QString sResult;

    sResult=getApplicationDataPath()+QDir::separator()+"qss";

    return sResult;
}

QList<QString> XOptions::getAllFilesFromDirectory(QString sDirectory, QString sExt)
{
    QDir directory(sDirectory);

    return directory.entryList(QStringList()<<sExt,QDir::Files);
}

QString XOptions::getApplicationDataPath()
{
    QString sResult;

#ifdef Q_OS_MAC
    sResult=qApp->applicationDirPath()+"/../Resources";
#else
//    sResult=qApp->applicationDirPath();
    sResult="E:\\ownCloud\\prepare\\release\\xpeviewer\\0.01\\xpeviewer_win32_portable\\base";
#endif

    return sResult;
}

#ifdef WIN32
void XOptions::registerContext(QString sApplication, QString sType)
{
    // TODO
}
#endif
#ifdef WIN32
void XOptions::clearContext(QString sApplication, QString sType)
{
    // TODO
}
#endif
#ifdef WIN32
bool XOptions::checkContext(QString sApplication, QString sType)
{
    // TODO
    return false;
}
#endif
