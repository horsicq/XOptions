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

XOptions::XOptions(QObject *pParent) : QObject(pParent)
{
    g_bIsNeedRestart=false;
}

void XOptions::setName(QString sName)
{
    this->g_sName=sName;
    this->g_sFilePath=getApplicationDataPath()+QDir::separator()+QString("%1").arg(sName);
}

void XOptions::setValueIDs(QList<ID> listVariantIDs)
{
    this->g_listValueIDs=listVariantIDs;
}

void XOptions::setDefaultValues(QMap<XOptions::ID, QVariant> mapDefaultValues)
{
    this->g_mapDefaultValues=mapDefaultValues;
}

void XOptions::load()
{
    QSettings settings(g_sFilePath,QSettings::IniFormat);

    int nNumberOfIDs=g_listValueIDs.count();

    for(int i=0;i<nNumberOfIDs;i++)
    {
        ID id=g_listValueIDs.at(i);
        QString sName=idToString(id);

        QVariant varDefault;

        if(g_mapDefaultValues.contains(id))
        {
            varDefault=g_mapDefaultValues.value(id);
        }
        else
        {
            switch(id)
            {
                case ID_STAYONTOP:              varDefault=false;           break;
                case ID_SCANAFTEROPEN:          varDefault=true;            break;
                case ID_RECURSIVESCAN:          varDefault=true;            break;
                case ID_DEEPSCAN:               varDefault=true;            break;
                case ID_HEURISTICSCAN:          varDefault=true;            break;
                case ID_SAVELASTDIRECTORY:      varDefault=true;            break;
                case ID_LASTDIRECTORY:          varDefault="";              break;
                case ID_SAVEBACKUP:             varDefault=true;            break;
                case ID_STYLE:                  varDefault="Fusion";        break;
                case ID_LANG:                   varDefault="System";        break;
                case ID_QSS:                    varDefault="veles";         break;
                case ID_DBPATH:                 varDefault="$data/db";      break;
                case ID_INFOPATH:               varDefault="$data/info";    break;
                case ID_SCANENGINE:             varDefault="die";           break;
                case ID_ROOTPATH:               varDefault="";              break;
                case ID_DATAPATH:               varDefault="$data/data";    break;
                case ID_JSON:                   varDefault="";              break;
                case ID_SINGLEAPPLICATION:      varDefault=false;           break;
                case ID_AUTHUSER:               varDefault="";              break;
                case ID_AUTHTOKEN:              varDefault="";              break;
                default:                        varDefault="";
            }
        }

        g_mapValues.insert(id,settings.value(sName,varDefault));
    }

    QString sLastDirectory=g_mapValues.value(ID_LASTDIRECTORY).toString();

    if(sLastDirectory!="")
    {
        if(!QDir(sLastDirectory).exists())
        {
            g_mapValues.insert(ID_LASTDIRECTORY,"");
        }
    }
}

void XOptions::save()
{
    QSettings settings(g_sFilePath,QSettings::IniFormat);

    int nNumberOfIDs=g_listValueIDs.count();

    for(int i=0;i<nNumberOfIDs;i++)
    {
        ID id=g_listValueIDs.at(i);
        QString sName=idToString(id);
        settings.setValue(sName,g_mapValues.value(id));
    }
}

QVariant XOptions::getValue(XOptions::ID id)
{
    return g_mapValues.value(id);
}

void XOptions::setValue(XOptions::ID id, QVariant vValue)
{
    if( (id==ID_STYLE)||
        (id==ID_LANG)||
        (id==ID_QSS))
    {
        QVariant vOld=g_mapValues.value(id);

        if(vValue!=vOld)
        {
            g_bIsNeedRestart=true;
        }
    }

    g_mapValues.insert(id,vValue);
}

void XOptions::clearValue(XOptions::ID id)
{
    g_mapValues.insert(id,"");
}

QString XOptions::idToString(ID id)
{
    QString sResult="Unknown";

    switch(id)
    {
        case ID_STAYONTOP:                  sResult=QString("StayOnTop");                   break;
        case ID_SCANAFTEROPEN:              sResult=QString("ScanAfterOpen");               break;
        case ID_RECURSIVESCAN:              sResult=QString("RecursiveScan");               break;
        case ID_DEEPSCAN:                   sResult=QString("DeepScan");                    break;
        case ID_HEURISTICSCAN:              sResult=QString("HeuristicScan");               break;
        case ID_SAVELASTDIRECTORY:          sResult=QString("SaveLastDirectory");           break;
        case ID_LASTDIRECTORY:              sResult=QString("LastDirectory");               break;
        case ID_SAVEBACKUP:                 sResult=QString("SaveBackup");                  break;
        case ID_STYLE:                      sResult=QString("Style");                       break;
        case ID_LANG:                       sResult=QString("Lang");                        break;
        case ID_QSS:                        sResult=QString("Qss");                         break;
        case ID_DBPATH:                     sResult=QString("DbPath");                      break;
        case ID_INFOPATH:                   sResult=QString("InfoPath");                    break;
        case ID_SCANENGINE:                 sResult=QString("ScanEngine");                  break;
        case ID_ROOTPATH:                   sResult=QString("RootPath");                    break;
        case ID_DATAPATH:                   sResult=QString("DataPath");                    break;
        case ID_JSON:                       sResult=QString("Json");                        break;
        case ID_SINGLEAPPLICATION:          sResult=QString("SingleApplication");           break;
        case ID_AUTHUSER:                   sResult=QString("AuthUser");                    break;
        case ID_AUTHTOKEN:                  sResult=QString("AuthToken");                   break;
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
    QFileInfo fi(sValue);

    if(fi.isFile())
    {
        sValue=fi.absolutePath();
    }
    else if(fi.isDir())
    {
        sValue=fi.absoluteFilePath();
    }

    if(getValue(ID_SAVELASTDIRECTORY).toBool())
    {
        setValue(ID_LASTDIRECTORY,sValue);
    }
}

QString XOptions::getDbPath()
{
    return getValue(ID_DBPATH).toString();
}

QString XOptions::getInfoPath()
{
    return getValue(ID_INFOPATH).toString();
}

QString XOptions::getScanEngine()
{
    return getValue(ID_SCANENGINE).toString();
}

QString XOptions::getRootPath()
{
    return getValue(ID_ROOTPATH).toString();
}

QString XOptions::getDataPath()
{
    return getValue(ID_DATAPATH).toString();
}

QString XOptions::getJson()
{
    return getValue(ID_JSON).toString();
}
#ifdef QT_GUI_LIB
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
#endif
#ifdef QT_GUI_LIB
void XOptions::setMonoFont(QWidget *pWidget)
{
    QFont font=pWidget->font();
    font.setFamily("Courier"); // TODO OSX and Linux
    pWidget->setFont(font);
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setCheckBox(QCheckBox *pCheckBox, XOptions::ID id)
{
    pCheckBox->setChecked(getValue(id).toBool());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::getCheckBox(QCheckBox *pCheckBox, XOptions::ID id)
{
    setValue(id,pCheckBox->isChecked());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setComboBox(QComboBox *pComboBox, XOptions::ID id)
{
    QSignalBlocker signalBlocker(pComboBox);

    pComboBox->clear();

    QString sValue=getValue(id).toString();

    if(id==ID_STYLE)
    {
        pComboBox->addItem("Default","");
        QStringList listKeys=QStyleFactory::keys();

        int nNumberOfKeys=listKeys.count();

        for(int i=0;i<nNumberOfKeys;i++)
        {
            QString sRecord=listKeys.at(i);
            pComboBox->addItem(sRecord,sRecord);
        }
    }
    else if(id==ID_LANG)
    {
        pComboBox->addItem("English","");
        pComboBox->addItem("System","System");

        QList<QString> listFileNames=getAllFilesFromDirectory(getApplicationLangPath(),"*.qm");

        int nNumberOfRecords=listFileNames.count();

        for(int i=0;i<nNumberOfRecords;i++)
        {
            QFileInfo fi(listFileNames.at(i));

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
        pComboBox->addItem("Default","");

        QList<QString> listFileNames=getAllFilesFromDirectory(getApplicationQssPath(),"*.qss");

        int nNumberOfRecords=listFileNames.count();

        for(int i=0;i<nNumberOfRecords;i++)
        {
            QFileInfo fi(listFileNames.at(i));

            QString sRecord=fi.baseName();

            pComboBox->addItem(sRecord,sRecord);
        }
    }
    else if(id==ID_SCANENGINE)
    {
        pComboBox->addItem(QString("Auto"),"auto");
        pComboBox->addItem(QString("Detect It Easy(DiE)"),"die");
        pComboBox->addItem(QString("Nauz File Detector(NFD)"),"nfd");
    }

    int nNumberOfItems=pComboBox->count();
    int nIndex=-1;

    for(int i=0;i<nNumberOfItems;i++)
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
#endif
#ifdef QT_GUI_LIB
void XOptions::getComboBox(QComboBox *pComboBox, XOptions::ID id)
{
    setValue(id,pComboBox->currentData());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setLineEdit(QLineEdit *pLineEdit, XOptions::ID id)
{
    pLineEdit->setText(getValue(id).toString());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::getLineEdit(QLineEdit *pLineEdit, XOptions::ID id)
{
    setValue(id,pLineEdit->text());
}
#endif
bool XOptions::isSaveBackup()
{
    return getValue(XOptions::ID_SAVEBACKUP).toBool();
}

bool XOptions::isSaveLastDirectory()
{
    return getValue(XOptions::ID_SAVELASTDIRECTORY).toBool();
}

bool XOptions::isRestartNeeded()
{
    return g_bIsNeedRestart;
}

bool XOptions::isScanAfterOpen()
{
    return getValue(XOptions::ID_SCANAFTEROPEN).toBool();
}

bool XOptions::isRecursiveScan()
{
    return getValue(XOptions::ID_RECURSIVESCAN).toBool();
}

bool XOptions::isDeepScan()
{
    return getValue(XOptions::ID_DEEPSCAN).toBool();
}

bool XOptions::isHeuristicScan()
{
    return getValue(XOptions::ID_HEURISTICSCAN).toBool();
}

bool XOptions::isSingleApplication()
{
    return getValue(XOptions::ID_SINGLEAPPLICATION).toBool();
}
#ifdef QT_GUI_LIB
void XOptions::adjustApplicationView(QString sOptionFileName, QString sTranslationName)
{
    XOptions xOptions;

    xOptions.setName(sOptionFileName);

    QList<XOptions::ID> listIDs;

    listIDs.append(XOptions::ID_STYLE);
    listIDs.append(XOptions::ID_LANG);
    listIDs.append(XOptions::ID_QSS);
    // mb TODO SingleApplication

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
#endif
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

QList<QString> XOptions::getAllFilesFromDirectory(QString sDirectory, QString sExtension)
{
    QDir directory(sDirectory);

    return directory.entryList(QStringList()<<sExtension,QDir::Files);
}

QString XOptions::getApplicationDataPath()
{
    QString sResult;
#ifdef Q_OS_MAC
    sResult=qApp->applicationDirPath()+"/../Resources";
#else
    sResult=qApp->applicationDirPath();
#endif
    return sResult;
}

#ifdef WIN32
bool XOptions::registerContext(QString sApplicationName, QString sType, QString sApplicationFilePath)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell\\%2\\command").arg(sType).arg(sApplicationName),QSettings::NativeFormat);
    settings.setValue(".","\""+sApplicationFilePath.replace("/","\\")+"\" \"%1\"");

    QSettings settingsIcon(QString("HKEY_CLASSES_ROOT\\%1\\shell\\%2").arg(sType).arg(sApplicationName),QSettings::NativeFormat);
    settingsIcon.setValue("Icon","\""+sApplicationFilePath.replace("/","\\")+"\"");

    // TODO Check if not send message
    return checkContext(sApplicationName,sType);
}
#endif
#ifdef WIN32
bool XOptions::clearContext(QString sApplicationName, QString sType)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell\\%2").arg(sType).arg(sApplicationName),QSettings::NativeFormat);
    settings.clear();
    // TODO Check if not send message

    return !(checkContext(sApplicationName,sType));
}
#endif
#ifdef WIN32
bool XOptions::checkContext(QString sApplicationName, QString sType)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell").arg(sType),QSettings::NativeFormat);

    return (settings.value(QString("%1/command/Default").arg(sApplicationName)).toString()!="");
}
#endif
