// Copyright (c) 2020-2021 hors<horsicq@gmail.com>
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
    g_bIsNative=checkNative();

    g_sName=QString("%1.ini").arg(qApp->applicationName()); // default name
}

void XOptions::setValueIDs(QList<ID> listVariantIDs)
{
    int nNumberOfIds=listVariantIDs.count();

    bool bSaveLastDirectory=false;
    bool bLastDirectory=false;
    bool bSaveRecentFiles=false;
    bool bRecentFiles=false;

    for(qint32 i=0;i<nNumberOfIds;i++)
    {
        if(listVariantIDs.at(i)==ID_SAVELASTDIRECTORY)
        {
            bSaveLastDirectory=true;
        }

        if(listVariantIDs.at(i)==ID_NU_LASTDIRECTORY)
        {
            bLastDirectory=true;
        }

        if(listVariantIDs.at(i)==ID_SAVERECENTFILES)
        {
            bSaveRecentFiles=true;
        }

        if(listVariantIDs.at(i)==ID_NU_RECENTFILES)
        {
            bRecentFiles=true;
        }
    }

    if(bSaveLastDirectory&&(!bLastDirectory))
    {
        listVariantIDs.append(ID_NU_LASTDIRECTORY);
    }

    if(bSaveRecentFiles&&(!bRecentFiles))
    {
        listVariantIDs.append(ID_NU_RECENTFILES);
    }

    this->g_listValueIDs=listVariantIDs;
}

void XOptions::setDefaultValues(QMap<XOptions::ID, QVariant> mapDefaultValues)
{
    this->g_mapDefaultValues=mapDefaultValues;
}

void XOptions::setNative(bool bValue)
{
    g_bIsNative=bValue;
}

bool XOptions::isNative()
{
    return g_bIsNative;
}

bool XOptions::isAppImage()
{
    // TODO more checks
    bool bResult=false;

    QString sApplicationDirPath=qApp->applicationDirPath();

    bResult=(sApplicationDirPath.contains("/tmp/.mount_")); // TODO check

    return bResult;
}

void XOptions::setName(QString sValue)
{
    g_sName=sValue;
}

void XOptions::load()
{
    QSettings *pSettings=nullptr;

    if(g_bIsNative)
    {
        pSettings=new QSettings;
    }
    else
    {
        pSettings=new QSettings(getApplicationDataPath()+QDir::separator()+QString("%1").arg(g_sName),QSettings::IniFormat);
    }

#ifdef QT_DEBUG
    if(pSettings)
    {
        qDebug("XOptions load file %s",pSettings->fileName().toLatin1().data());
    }
#endif

    int nNumberOfIDs=g_listValueIDs.count();

    for(qint32 i=0;i<nNumberOfIDs;i++)
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
                case ID_STAYONTOP:              varDefault=false;                   break;
                case ID_SCANAFTEROPEN:          varDefault=true;                    break;
                case ID_RECURSIVESCAN:          varDefault=true;                    break;
                case ID_DEEPSCAN:               varDefault=true;                    break;
                case ID_HEURISTICSCAN:          varDefault=true;                    break;
                case ID_SAVELASTDIRECTORY:      varDefault=true;                    break;
                case ID_SAVERECENTFILES:        varDefault=true;                    break;
                case ID_SAVEBACKUP:             varDefault=true;                    break;
                case ID_STYLE:                  varDefault="Fusion";                break; // TODO Check styles in OSX and Linux
                case ID_LANG:                   varDefault="System";                break;
                case ID_QSS:                    varDefault="";                      break;
                case ID_DATABASEPATH:           varDefault="$data/db";              break;
                case ID_INFOPATH:               varDefault="$data/info";            break;
                case ID_SCANENGINE:             varDefault="die";                   break;
                case ID_DISASMSYNTAX:           varDefault="";                      break;
                case ID_ROOTPATH:               varDefault="";                      break;
                case ID_DATAPATH:               varDefault="$data/data";            break;
                case ID_JSON:                   varDefault="";                      break;
                case ID_SINGLEAPPLICATION:      varDefault=false;                   break;
                case ID_SEARCHSIGNATURESPATH:   varDefault="$data/signatures";      break;
                case ID_STRUCTSPATH:            varDefault="$data/structs";         break;
                case ID_AUTHUSER:               varDefault="";                      break;
                case ID_AUTHTOKEN:              varDefault="";                      break;
                case ID_SHOWLOGO:               varDefault=true;                    break;
                case ID_NU_RECENTFILES:         varDefault=QList<QVariant>();       break;
                case ID_NU_LASTDIRECTORY:       varDefault="";                      break;
                default:                        varDefault="";
            }
        }

        QVariant variant=pSettings->value(sName,varDefault);

        if(!variant.toString().contains("$data"))
        {
            if( (id==ID_DATABASEPATH)||
                (id==ID_QSS)||
                (id==ID_INFOPATH)||
                (id==ID_DATAPATH)||
                (id==ID_SEARCHSIGNATURESPATH)||
                (id==ID_STRUCTSPATH))
            {
                if(!QDir(variant.toString()).exists())
                {
                    variant=varDefault;
                }
            }
        }

        if(g_bIsNative)
        {
            if(variant.toString().contains("$data"))
            {
                QString sValue=variant.toString();
                sValue=sValue.replace("$data",getApplicationDataPath());
                variant=sValue;
            }
        }

        g_mapValues.insert(id,variant);
    }

    QString sLastDirectory=g_mapValues.value(ID_NU_LASTDIRECTORY).toString();

    if(sLastDirectory!="")
    {
        if(!QDir(sLastDirectory).exists())
        {
            g_mapValues.insert(ID_NU_LASTDIRECTORY,"");
        }
    }

    delete pSettings;
}

void XOptions::save()
{
    QSettings *pSettings=nullptr;

    if(g_bIsNative)
    {
        pSettings=new QSettings;
    }
    else
    {
        pSettings=new QSettings(getApplicationDataPath()+QDir::separator()+QString("%1").arg(g_sName),QSettings::IniFormat);
    }

#ifdef QT_DEBUG
    if(pSettings)
    {
        qDebug("XOptions save file %s",pSettings->fileName().toLatin1().data());
    }
#endif

    int nNumberOfIDs=g_listValueIDs.count();

    for(qint32 i=0;i<nNumberOfIDs;i++)
    {
        ID id=g_listValueIDs.at(i);
        QString sName=idToString(id);
        pSettings->setValue(sName,g_mapValues.value(id));
    }

    delete pSettings;
}

QVariant XOptions::getValue(XOptions::ID id)
{
    return g_mapValues.value(id);
}

void XOptions::setValue(XOptions::ID id, QVariant vValue)
{
    if( (id==ID_STYLE)||
        (id==ID_LANG)||
        (id==ID_QSS)||
        (id==ID_SEARCHSIGNATURESPATH)||     // TODO remove
        (id==ID_STRUCTSPATH))               // TODO remove
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
    QString sResult=QString("Unknown");

    switch(id)
    {
        case ID_STAYONTOP:                  sResult=QString("StayOnTop");                   break;
        case ID_SCANAFTEROPEN:              sResult=QString("ScanAfterOpen");               break;
        case ID_RECURSIVESCAN:              sResult=QString("RecursiveScan");               break;
        case ID_DEEPSCAN:                   sResult=QString("DeepScan");                    break;
        case ID_HEURISTICSCAN:              sResult=QString("HeuristicScan");               break;
        case ID_SAVELASTDIRECTORY:          sResult=QString("SaveLastDirectory");           break;
        case ID_SAVERECENTFILES:            sResult=QString("SaveRecentFiles");             break;
        case ID_SAVEBACKUP:                 sResult=QString("SaveBackup");                  break;
        case ID_STYLE:                      sResult=QString("Style");                       break;
        case ID_LANG:                       sResult=QString("Lang");                        break;
        case ID_QSS:                        sResult=QString("Qss");                         break;
        case ID_DATABASEPATH:               sResult=QString("DatabasePath");                break;
        case ID_INFOPATH:                   sResult=QString("InfoPath");                    break;
        case ID_SCANENGINE:                 sResult=QString("ScanEngine");                  break;
        case ID_DISASMSYNTAX:               sResult=QString("DisasmSyntax");                break;
        case ID_ROOTPATH:                   sResult=QString("RootPath");                    break;
        case ID_DATAPATH:                   sResult=QString("DataPath");                    break;
        case ID_JSON:                       sResult=QString("Json");                        break;
        case ID_SINGLEAPPLICATION:          sResult=QString("SingleApplication");           break;
        case ID_SEARCHSIGNATURESPATH:       sResult=QString("SearchSignaturesPath");        break;
        case ID_STRUCTSPATH:                sResult=QString("StructsPath");                 break;
        case ID_AUTHUSER:                   sResult=QString("AuthUser");                    break;
        case ID_AUTHTOKEN:                  sResult=QString("AuthToken");                   break;
        case ID_SHOWLOGO:                   sResult=QString("ShowLogo");                    break;
        case ID_NU_RECENTFILES:             sResult=QString("RecentFiles");                 break;
        case ID_NU_LASTDIRECTORY:           sResult=QString("LastDirectory");               break;
    }

    return sResult;
}

QString XOptions::getLastDirectory()
{
    QString sResult;

    bool bSaveLastDirectory=getValue(ID_SAVELASTDIRECTORY).toBool();
    QString sLastDirectory=getValue(ID_NU_LASTDIRECTORY).toString();

    if(bSaveLastDirectory&&(sLastDirectory!="")&&QDir().exists(sLastDirectory))
    {
        sResult=sLastDirectory;
    }

    return sResult;
}

void XOptions::setLastDirectory(QString sPathName)
{
    QFileInfo fi(sPathName);

    if(fi.isFile())
    {
        sPathName=fi.absolutePath();
    }
    else if(fi.isDir())
    {
        sPathName=fi.absoluteFilePath();
    }

    if(getValue(ID_SAVELASTDIRECTORY).toBool())
    {
        setValue(ID_NU_LASTDIRECTORY,sPathName);
    }
}

void XOptions::setLastFileName(QString sFileName)
{
    QFileInfo fi(sFileName);

    QString sDirectory;

    if(fi.isFile())
    {
        sDirectory=fi.absolutePath();
    }

    if(getValue(ID_SAVELASTDIRECTORY).toBool())
    {
        setValue(ID_NU_LASTDIRECTORY,sDirectory);
    }

    if(getValue(ID_SAVERECENTFILES).toBool())
    {
        QString _sFileName=QString("\"%1\"").arg(fi.absoluteFilePath());

        QList<QVariant> listFiles=getValue(ID_NU_RECENTFILES).toList();

        listFiles.removeAll(_sFileName);

        listFiles.append(QVariant(_sFileName));

        if(listFiles.count()>N_MAX_RECENTFILES)
        {
            listFiles.removeLast();
        }

        g_mapValues.insert(ID_NU_RECENTFILES,listFiles);
    }
}

void XOptions::clearRecentFiles()
{
    g_mapValues.insert(ID_NU_RECENTFILES,QList<QVariant>());
}

QList<QString> XOptions::getRecentFiles()
{
    QList<QString> listResult;

    QList<QVariant> listFiles=getValue(ID_NU_RECENTFILES).toList();

    int nNumberOfRecords=listFiles.size();

    for(qint32 i=0;i<nNumberOfRecords;i++)
    {
        listResult.append(listFiles.at(i).toString());
    }

    return listResult;
}

QString XOptions::getDatabasePath()
{
    return getValue(ID_DATABASEPATH).toString();
}

QString XOptions::getInfoPath()
{
    return getValue(ID_INFOPATH).toString();
}

QString XOptions::getScanEngine()
{
    return getValue(ID_SCANENGINE).toString();
}

QString XOptions::getDisasmSyntax()
{
    return getValue(ID_DISASMSYNTAX).toString();
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

QString XOptions::getAuthUser()
{
    return getValue(ID_AUTHUSER).toString();
}

QString XOptions::getAuthToken()
{
    return getValue(ID_AUTHTOKEN).toString();
}

#ifdef QT_GUI_LIB
void XOptions::adjustStayOnTop(QWidget *pWidget)
{
    Qt::WindowFlags wf=pWidget->windowFlags();

    if(isStayOnTop())
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
void XOptions::setMonoFont(QWidget *pWidget,qint32 nSize)
{
    QFont font=pWidget->font();

#ifdef Q_OS_WIN
    font.setFamily("Courier");
#endif
#ifdef Q_OS_LINUX
    font.setFamily("Monospace");    // TODO Check
#endif
#ifdef Q_OS_OSX
    font.setFamily("Menlo");        // TODO Check
#endif

    if(nSize!=-1)
    {
        font.setPointSize(nSize);
    }

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
#if QT_VERSION >= QT_VERSION_CHECK(5,3,0)
    const QSignalBlocker signalBlocker(pComboBox);
#else
    const bool bBlocked1=pComboBox->blockSignals(true);
#endif

    pComboBox->clear();

    QString sValue=getValue(id).toString();

    if(id==ID_STYLE)
    {
        pComboBox->addItem("Default","");
        QStringList listKeys=QStyleFactory::keys();

        int nNumberOfKeys=listKeys.count();

        for(qint32 i=0;i<nNumberOfKeys;i++)
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

            QString sRecord=fi.completeBaseName();

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

            QString sRecord=fi.completeBaseName();

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

#if QT_VERSION < QT_VERSION_CHECK(5,3,0)
    pComboBox->blockSignals(bBlocked1);
#endif
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

bool XOptions::isSaveRecentFiles()
{
    return getValue(XOptions::ID_SAVERECENTFILES).toBool();
}

bool XOptions::isRestartNeeded()
{
    return g_bIsNeedRestart;
}

bool XOptions::isStayOnTop()
{
    return getValue(XOptions::ID_STAYONTOP).toBool();
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

bool XOptions::isShowLogo()
{
    return getValue(XOptions::ID_SHOWLOGO).toBool();
}

QString XOptions::getSearchSignaturesPath()
{
    return getValue(XOptions::ID_SEARCHSIGNATURESPATH).toString();
}

QString XOptions::getStructsPath()
{
    return getValue(XOptions::ID_STRUCTSPATH).toString();
}
#ifdef QT_GUI_LIB
void XOptions::adjustApplicationView(QString sTranslationName, XOptions *pOptions)
{
    QString sStyle=pOptions->getValue(XOptions::ID_STYLE).toString();

    if(sStyle!="")
    {
        qApp->setStyle(QStyleFactory::create(sStyle));
    }

    QTranslator *pTranslator=new QTranslator; // Important
    QString sLang=pOptions->getValue(XOptions::ID_LANG).toString();
    QString sLangsPath=pOptions->getApplicationLangPath();

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

    QString sQss=pOptions->getValue(XOptions::ID_QSS).toString();

    if(sQss!="")
    {
        QString sQssFileName=pOptions->getApplicationQssPath()+QDir::separator()+QString("%1.qss").arg(sQss);

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
#ifdef QT_GUI_LIB
void XOptions::adjustApplicationView(QString sApplicationFileName,QString sTranslationName)
{
    XOptions xOptions;

    xOptions.setName(sApplicationFileName);

    QList<XOptions::ID> listIDs;

    listIDs.append(XOptions::ID_STYLE);
    listIDs.append(XOptions::ID_LANG);
    listIDs.append(XOptions::ID_QSS);

    xOptions.setValueIDs(listIDs);
    xOptions.load();

    xOptions.adjustApplicationView(sTranslationName,&xOptions);
}
#endif
#ifdef QT_GUI_LIB
QWidget *XOptions::getMainWidget(QWidget *pWidget)
{
    QWidget *pResult=pWidget;

    while(pResult->parent())
    {
        pResult=qobject_cast<QWidget *>(pResult->parent());
    }

    return pResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveModel(QAbstractItemModel *pModel,QString sFileName)
{
    bool bResult=false;

    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadWrite))
    {
        int nNumberOfRows=pModel->rowCount();
        int nNumberOfColumns=pModel->columnCount();

        QString sResult;

        for(int i=0;i<nNumberOfRows;i++)
        {
            for(int j=0;j<nNumberOfColumns;j++)
            {
                QString sText=pModel->data(pModel->index(i,j)).toString();

                if(j!=(nNumberOfColumns-1))
                {
                    sResult+=QString("%1\t").arg(sText);
                }
                else
                {
                    sResult+=QString("%1\r\n").arg(sText);
                }
            }
        }

        file.resize(0);
        file.write(sResult.toLatin1().data());

        file.close();

        bResult=true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTextEdit(QTextEdit *pTextEdit, QString sFileName)
{
    bool bResult=false;

    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadWrite))
    {
        QString sResult=pTextEdit->toPlainText();

        file.resize(0);
        file.write(sResult.toLatin1().data());

        file.close();

        bResult=true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::savePlainTextEdit(QPlainTextEdit *pPlainTextEdit, QString sFileName)
{
    bool bResult=false;

    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadWrite))
    {
        QString sResult=pPlainTextEdit->toPlainText();

        file.resize(0);
        file.write(sResult.toLatin1().data());

        file.close();

        bResult=true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTableView(QTableView *pTableView, QString sFileName)
{
    return saveModel(pTableView->model(),sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTableWidget(QTableWidget *pTableWidget, QString sFileName)
{
    return saveModel(pTableWidget->model(),sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveListView(QListView *pListView, QString sFileName)
{
    return saveModel(pListView->model(),sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveListWidget(QListWidget *pListWidget, QString sFileName)
{
    return saveModel(pListWidget->model(),sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTreeView(QTreeView *pTreeView, QString sFileName)
{
    return saveModel(pTreeView->model(),sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTreeWidget(QTreeWidget *pTreeWidget, QString sFileName)
{
    return saveModel(pTreeWidget->model(),sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTextBrowser(QTextBrowser *pTextBrowser, QString sFileName)
{
    bool bResult=false;

    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadWrite))
    {
        QString sResult=pTextBrowser->toPlainText();

        file.resize(0);
        file.write(sResult.toLatin1().data());

        file.close();

        bResult=true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTextBrowserHtml(QTextBrowser *pTextBrowser, QString sFileName)
{
    bool bResult=false;

    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadWrite))
    {
        QString sResult=pTextBrowser->toHtml();

        file.resize(0);
        file.write(sResult.toLatin1().data());

        file.close();

        bResult=true;
    }

    return bResult;
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

bool XOptions::checkNative()
{
    bool bResult=false;
#ifdef Q_OS_MAC
    bResult=true;
#elif defined(Q_OS_LINUX)
    QString sApplicationDirPath=qApp->applicationDirPath();

    if( (sApplicationDirPath=="/bin")||
        (sApplicationDirPath=="/usr/bin")||
        (sApplicationDirPath=="/usr/local/bin")||
        isAppImage())
    {
        bResult=true;
    }
    else
    {
        bResult=false;
    }
#elif defined(Q_OS_WIN)
    QString sApplicationDirPath=qApp->applicationDirPath();

    if(sApplicationDirPath.contains("C:\\Program Files\\")||sApplicationDirPath.contains("C:\\Program Files (x86)\\"))
    {
        bResult=true;
    }
#endif

    return bResult;
}

QString XOptions::getApplicationDataPath()
{
    QString sResult;

    QString sApplicationDirPath=qApp->applicationDirPath();

#ifdef Q_OS_MAC
    sResult=sApplicationDirPath+"/../Resources";
#elif defined(Q_OS_LINUX)
    if(g_bIsNative)
    {
        if(sApplicationDirPath.contains("/tmp/.mount_")) // AppImage
        {
            sResult=sApplicationDirPath.section("/",0,2);
        }

        sResult+=QString("/usr/lib/%1").arg(qApp->applicationName());
    }
    else
    {
        sResult=sApplicationDirPath;
    }
#else
    sResult=sApplicationDirPath;
#endif

    return sResult;
}

QString XOptions::getTitle(QString sName, QString sVersion)
{
    QString sResult;
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
    sResult=QString("%1 v%2 [%3](%4)").arg(sName,sVersion,QSysInfo::prettyProductName(),QSysInfo::buildCpuArchitecture());
#else
    sResult=QString("%1 v%2").arg(sName,sVersion); // TODO OS Name, For Windows Arch GetVersionExA
#endif

    return sResult;
}

#ifdef Q_OS_WIN
bool XOptions::registerContext(QString sApplicationName, QString sType, QString sApplicationFilePath)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell\\%2\\command").arg(sType,sApplicationName),QSettings::NativeFormat);
    settings.setValue(".","\""+sApplicationFilePath.replace("/","\\")+"\" \"%1\"");

    QSettings settingsIcon(QString("HKEY_CLASSES_ROOT\\%1\\shell\\%2").arg(sType,sApplicationName),QSettings::NativeFormat);
    settingsIcon.setValue("Icon","\""+sApplicationFilePath.replace("/","\\")+"\"");

    // TODO Check if not send message
    return checkContext(sApplicationName,sType);
}
#endif
#ifdef Q_OS_WIN
bool XOptions::clearContext(QString sApplicationName, QString sType)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell\\%2").arg(sType,sApplicationName),QSettings::NativeFormat);
    settings.clear();

    // TODO Check if not send message
    return !(checkContext(sApplicationName,sType));
}
#endif
#ifdef Q_OS_WIN
bool XOptions::checkContext(QString sApplicationName, QString sType)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell").arg(sType),QSettings::NativeFormat);

    return (settings.value(QString("%1/command/Default").arg(sApplicationName)).toString()!="");
}
#endif
