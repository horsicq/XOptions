/* Copyright (c) 2020-2022 hors<horsicq@gmail.com>
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
#include "xoptions.h"

XOptions::XOptions(QObject *pParent) : QObject(pParent)
{
#ifdef QT_GUI_LIB
    g_pRecentFilesMenu = nullptr;
    g_pCodePagesMenu = nullptr;
#endif
    g_bIsNeedRestart = false;
    g_nMaxRecentFilesCount = N_MAX_RECENT_FILES_COUNT;
    g_bIsNative = checkNative();

    g_sName = QString("%1.ini").arg(qApp->applicationName());  // default name
}

void XOptions::resetToDefault()
{
    qint32 nNumberOfRecords = g_listValueIDs.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        XOptions::ID id = g_listValueIDs.at(i);

        bool bInsert = false;

        if ((id != ID_NU_LASTDIRECTORY) && (id != ID_NU_RECENTFILES)) {
            bInsert = true;
        }

        if (bInsert) {
            g_mapValues.insert(id, g_mapDefaultValues.value(id));
        }
    }
}

void XOptions::setValueIDs(QList<ID> listVariantIDs)
{
    g_listValueIDs = listVariantIDs;
}

void XOptions::setDefaultValues(QMap<XOptions::ID, QVariant> mapDefaultValues)
{
    g_mapDefaultValues = mapDefaultValues;
}

void XOptions::addID(ID id, QVariant varDefaultValue)
{
    g_listValueIDs.append(id);

    if (varDefaultValue.isValid()) {
        g_mapDefaultValues.insert(id, varDefaultValue);
    }
}

XOptions::GROUPID XOptions::getGroupID(ID id)
{
    GROUPID result = GROUPID_UNKNOWN;

    switch (id) {
        case ID_VIEW_STAYONTOP:
        case ID_VIEW_STYLE:
        case ID_VIEW_QSS:
        case ID_VIEW_LANG:
        case ID_VIEW_SINGLEAPPLICATION:
        case ID_VIEW_SHOWLOGO:
        case ID_VIEW_FONT:
            result = GROUPID_VIEW;
            break;
        case ID_FILE_SAVELASTDIRECTORY:
        case ID_FILE_SAVERECENTFILES:
        case ID_FILE_SAVEBACKUP:
        case ID_FILE_CONTEXT:
            result = GROUPID_FILE;
            break;
        case ID_SCAN_SCANAFTEROPEN:
        case ID_SCAN_RECURSIVE:
        case ID_SCAN_DEEP:
        case ID_SCAN_HEURISTIC:
        case ID_SCAN_VERBOSE:
        case ID_SCAN_ALLTYPES:
        case ID_SCAN_ENGINE:
        case ID_SCAN_DATABASEPATH:
        case ID_SCAN_EDITORFONT:
            result = GROUPID_SCAN;
            break;
        case ID_SIGNATURES_PATH:
            result = GROUPID_SIGNATURES;
            break;
        case ID_INFO_PATH:
            result = GROUPID_INFO;
            break;
        case ID_DISASM_FONT:
        case ID_DISASM_SYNTAX:
        case ID_DISASM_ADDRESSCOLON:
        case ID_DISASM_UPPERCASE:
        case ID_DISASM_HIGHLIGHT:
        case ID_DISASM_COLOR_X86_CALL:
        case ID_DISASM_COLOR_X86_RET:
        case ID_DISASM_COLOR_X86_JCC:
        case ID_DISASM_COLOR_X86_PUSH:
        case ID_DISASM_COLOR_X86_POP:
        case ID_DISASM_COLOR_X86_NOP:
        case ID_DISASM_COLOR_X86_JMP:
        case ID_DISASM_COLOR_X86_INT3:
        case ID_DISASM_COLOR_ARM_BL:
        case ID_DISASM_COLOR_ARM_RET:
        case ID_DISASM_COLOR_ARM_PUSH:
        case ID_DISASM_COLOR_ARM_POP:
            result = GROUPID_DISASM;
            break;
        case ID_HEX_FONT:
        case ID_HEX_ADDRESSCOLON:
        case ID_HEX_BLINKINGCURSOR:
            result = GROUPID_HEX;
            break;
        case ID_STACK_FONT:
        case ID_STACK_ADDRESSCOLON:
            result = GROUPID_STACK;
            break;
        case ID_REGISTERS_FONT:
            result = GROUPID_REGISTERS;
            break;
        case ID_IODRIVER_FILENAME:
        case ID_IODRIVER_SERVICENAME:
            result = GROUPID_IODRIVER;
            break;
        default:
#ifdef QT_DEBUG
        qDebug("Unknown GroupID");
#endif
            result = GROUPID_UNKNOWN;
    }

    return result;
}

bool XOptions::isIDPresent(ID id)
{
    bool bResult = false;

    qint32 nNumberOfRecords = g_listValueIDs.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        if (g_listValueIDs.at(i) == id) {
            bResult = true;
            break;
        }
    }

    return bResult;
}

bool XOptions::isGroupIDPresent(GROUPID groupID)
{
    bool bResult = false;

    qint32 nNumberOfRecords = g_listValueIDs.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        if (getGroupID(g_listValueIDs.at(i)) == groupID) {
            bResult = true;
            break;
        }
    }

    return bResult;
}

void XOptions::setNative(bool bValue)
{
    g_bIsNative = bValue;
}

bool XOptions::isNative()
{
    return g_bIsNative;
}

bool XOptions::isAppImage()
{
    bool bResult = false;

    QString sApplicationDirPath = qApp->applicationDirPath();

    bResult = (sApplicationDirPath.contains("/tmp/.mount_"));  // TODO more check

    return bResult;
}

void XOptions::setName(QString sValue)
{
#ifdef QT_DEBUG
#ifdef Q_OS_WIN
#ifndef Q_OS_WIN64
    sValue += "win32.debug.ini";
#else
    sValue += "win64.debug.ini";
#endif
#endif
#ifdef Q_OS_LINUX
    sValue += "linux.debug.ini";
#endif
#ifdef Q_OS_MACOS
    sValue += "macos.debug.ini";
#endif
#endif
    g_sName = sValue;
}

void XOptions::load()
{
    QSettings *pSettings = nullptr;

    if (g_bIsNative) {
        pSettings = new QSettings;
    } else {
        pSettings = new QSettings(getApplicationDataPath() + QDir::separator() + QString("%1").arg(g_sName), QSettings::IniFormat);
    }

#ifdef QT_DEBUG
    if (pSettings) {
        qDebug("XOptions load file %s", pSettings->fileName().toUtf8().data());
    }
#endif

    qint32 nNumberOfIDs = g_listValueIDs.count();

    bool bSaveLastDirectory = false;
    bool bLastDirectory = false;
    bool bSaveRecentFiles = false;
    bool bRecentFiles = false;

    for (qint32 i = 0; i < nNumberOfIDs; i++) {
        if (g_listValueIDs.at(i) == ID_FILE_SAVELASTDIRECTORY) {
            bSaveLastDirectory = true;
        }

        if (g_listValueIDs.at(i) == ID_NU_LASTDIRECTORY) {
            bLastDirectory = true;
        }

        if (g_listValueIDs.at(i) == ID_FILE_SAVERECENTFILES) {
            bSaveRecentFiles = true;
        }

        if (g_listValueIDs.at(i) == ID_NU_RECENTFILES) {
            bRecentFiles = true;
        }
    }

    if (bSaveLastDirectory && (!bLastDirectory)) {
        g_listValueIDs.append(ID_NU_LASTDIRECTORY);
    }

    if (bSaveRecentFiles && (!bRecentFiles)) {
        g_listValueIDs.append(ID_NU_RECENTFILES);
    }

    nNumberOfIDs = g_listValueIDs.count();

    for (qint32 i = 0; i < nNumberOfIDs; i++) {
        ID id = g_listValueIDs.at(i);
        QString sName = idToString(id);

        QVariant varDefault;

        if (g_mapDefaultValues.contains(id)) {
            varDefault = g_mapDefaultValues.value(id);
        } else {
            // TODO remove,use addID
            switch (id) {
                case ID_ROOTPATH:
                    varDefault = "";
                    break;
                case ID_DATAPATH:
                    varDefault = "$data/data";
                    break;
                case ID_JSON:
                    varDefault = "";
                    break;
                case ID_STRUCTSPATH:
                    varDefault = "$data/structs";
                    break;
                case ID_STRUCTS_PATH:
                    varDefault = "$data/structs";
                    break;
                case ID_AUTHUSER:
                    varDefault = "";
                    break;
                case ID_AUTHTOKEN:
                    varDefault = "";
                    break;
                case ID_NU_RECENTFILES:
                    varDefault = QList<QVariant>();
                    break;
                case ID_NU_LASTDIRECTORY:
                    varDefault = "";
                    break;
                default:
                    varDefault = "";
            }
        }

        QVariant variant = pSettings->value(sName, varDefault);

        if (!variant.toString().contains("$data")) {
            if ((id == ID_SCAN_DATABASEPATH) || (id == ID_DATAPATH) || (id == ID_SIGNATURES_PATH) || (id == ID_STRUCTS_PATH) || (id == ID_STRUCTSPATH) ||
                (id == ID_INFO_PATH)) {
                if (!QDir(variant.toString()).exists()) {
                    variant = varDefault;
                }
            }
        }

        if (g_bIsNative) {
            if (variant.toString().contains("$data")) {
                QString sValue = variant.toString();
                sValue = sValue.replace("$data", getApplicationDataPath());
                variant = sValue;
            }
        }

        g_mapValues.insert(id, variant);
    }

    QString sLastDirectory = g_mapValues.value(ID_NU_LASTDIRECTORY).toString();

    if (sLastDirectory != "") {
        if (!QDir(sLastDirectory).exists()) {
            g_mapValues.insert(ID_NU_LASTDIRECTORY, "");
        }
    }

    delete pSettings;
}

void XOptions::save()
{
    QSettings *pSettings = nullptr;

    if (g_bIsNative) {
        pSettings = new QSettings;
    } else {
        pSettings = new QSettings(getApplicationDataPath() + QDir::separator() + QString("%1").arg(g_sName), QSettings::IniFormat);
    }

#ifdef QT_DEBUG
    if (pSettings) {
        qDebug("XOptions save file %s", pSettings->fileName().toUtf8().data());
    }
#endif

    qint32 nNumberOfIDs = g_listValueIDs.count();

    for (qint32 i = 0; i < nNumberOfIDs; i++) {
        ID id = g_listValueIDs.at(i);
        QString sName = idToString(id);
        pSettings->setValue(sName, g_mapValues.value(id));

        if ((id == ID_FILE_SAVELASTDIRECTORY) && (g_mapValues.value(id).toBool() == false)) {
            pSettings->setValue(idToString(ID_NU_LASTDIRECTORY), "");
        } else if ((id == ID_FILE_SAVERECENTFILES) && (g_mapValues.value(id).toBool() == false)) {
            clearRecentFiles();
        }
    }

    delete pSettings;
}

QVariant XOptions::getValue(XOptions::ID id)
{
    return g_mapValues.value(id);
}

void XOptions::setValue(XOptions::ID id, QVariant vValue)
{
    if ((id == ID_VIEW_STYLE) || (id == ID_VIEW_LANG) || (id == ID_VIEW_QSS)) {
        QVariant vOld = g_mapValues.value(id);

        if (vValue != vOld) {
            g_bIsNeedRestart = true;
        }
    }

    g_mapValues.insert(id, vValue);
}

void XOptions::clearValue(XOptions::ID id)
{
    g_mapValues.insert(id, "");
}

QString XOptions::idToString(ID id)
{
    QString sResult = QString("Unknown");

    switch (id) {
        // TODO remove
        case ID_ROOTPATH:
            sResult = QString("RootPath");
            break;
        case ID_DATAPATH:
            sResult = QString("DataPath");
            break;
        case ID_JSON:
            sResult = QString("Json");
            break;
        case ID_STRUCTSPATH:
            sResult = QString("StructsPath");
            break;
        case ID_AUTHUSER:
            sResult = QString("AuthUser");
            break;
        case ID_AUTHTOKEN:
            sResult = QString("AuthToken");
            break;
        // new
        case ID_VIEW_STAYONTOP:
            sResult = QString("View/StayOnTop");
            break;
        case ID_VIEW_STYLE:
            sResult = QString("View/Style");
            break;
        case ID_VIEW_QSS:
            sResult = QString("View/Qss");
            break;
        case ID_VIEW_LANG:
            sResult = QString("View/Lang");
            break;
        case ID_VIEW_SINGLEAPPLICATION:
            sResult = QString("View/SingleApplication");
            break;
        case ID_VIEW_SHOWLOGO:
            sResult = QString("View/ShowLogo");
            break;
        case ID_VIEW_FONT:
            sResult = QString("View/Font");
            break;
        case ID_FILE_SAVELASTDIRECTORY:
            sResult = QString("File/SaveLastDirectory");
            break;
        case ID_FILE_SAVERECENTFILES:
            sResult = QString("File/SaveRecentFiles");
            break;
        case ID_FILE_SAVEBACKUP:
            sResult = QString("File/SaveBackup");
            break;
        case ID_FILE_CONTEXT:
            sResult = QString("File/Context");
            break;
        case ID_SCAN_SCANAFTEROPEN:
            sResult = QString("Scan/ScanAfterOpen");
            break;
        case ID_SCAN_RECURSIVE:
            sResult = QString("Scan/Recursive");
            break;
        case ID_SCAN_DEEP:
            sResult = QString("Scan/Deep");
            break;
        case ID_SCAN_HEURISTIC:
            sResult = QString("Scan/Heuristic");
            break;
        case ID_SCAN_VERBOSE:
            sResult = QString("Scan/Verbose");
            break;
        case ID_SCAN_ALLTYPES:
            sResult = QString("Scan/AllTypes");
            break;
        case ID_SCAN_ENGINE:
            sResult = QString("Scan/Engine");
            break;
        case ID_SCAN_DATABASEPATH:
            sResult = QString("Scan/DatabasePath");
            break;
        case ID_SCAN_EDITORFONT:
            sResult = QString("Scan/EditorFont");
            break;
        case ID_SIGNATURES_PATH:
            sResult = QString("Signatures/Path");
            break;
        case ID_INFO_PATH:
            sResult = QString("Info/Path");
            break;
        case ID_ONLINETOOLS_VIRUSTOTAL_APIKEY:
            sResult = QString("OnlineTools/VirusTotalApi");
            break;
        case ID_DISASM_FONT:
            sResult = QString("Disasm/Font");
            break;
        case ID_DISASM_ADDRESSCOLON:
            sResult = QString("Disasm/AddressColon");
            break;
        case ID_DISASM_UPPERCASE:
            sResult = QString("Disasm/Uppercase");
            break;
        case ID_DISASM_HIGHLIGHT:
            sResult = QString("Disasm/Highlight");
            break;
        case ID_DISASM_SYNTAX:
            sResult = QString("Disasm/Syntax");
            break;
        case ID_DISASM_COLOR_X86_CALL:
            sResult = QString("Disasm/Color/x86/call");
            break;
        case ID_DISASM_COLOR_X86_RET:
            sResult = QString("Disasm/Color/x86/ret");
            break;
        case ID_DISASM_COLOR_X86_JCC:
            sResult = QString("Disasm/Color/x86/jcc");
            break;
        case ID_DISASM_COLOR_X86_PUSH:
            sResult = QString("Disasm/Color/x86/push");
            break;
        case ID_DISASM_COLOR_X86_POP:
            sResult = QString("Disasm/Color/x86/pop");
            break;
        case ID_DISASM_COLOR_X86_NOP:
            sResult = QString("Disasm/Color/x86/nop");
            break;
        case ID_DISASM_COLOR_X86_JMP:
            sResult = QString("Disasm/Color/x86/jmp");
            break;
        case ID_DISASM_COLOR_X86_INT3:
            sResult = QString("Disasm/Color/x86/int3");
            break;
        case ID_DISASM_COLOR_ARM_BL:
            sResult = QString("Disasm/Color/arm/bl");
            break;
        case ID_DISASM_COLOR_ARM_RET:
            sResult = QString("Disasm/Color/arm/ret");
            break;
        case ID_DISASM_COLOR_ARM_PUSH:
            sResult = QString("Disasm/Color/arm/push");
            break;
        case ID_DISASM_COLOR_ARM_POP:
            sResult = QString("Disasm/Color/arm/pop");
            break;
        case ID_HEX_FONT:
            sResult = QString("Hex/Font");
            break;
        case ID_HEX_ADDRESSCOLON:
            sResult = QString("Hex/AddressColon");
            break;
        case ID_HEX_BLINKINGCURSOR:
            sResult = QString("Hex/BlinkingCursor");
            break;
        case ID_STACK_FONT:
            sResult = QString("Stack/Font");
            break;
        case ID_STACK_ADDRESSCOLON:
            sResult = QString("Stack/AddressColon");
            break;
        case ID_REGISTERS_FONT:
            sResult = QString("Registers/Font");
            break;
        case ID_DEBUGGER_BREAKPOINT_SYSTEM:
            sResult = QString("Debugger/Breakpoint/System");
            break;
        case ID_DEBUGGER_BREAKPOINT_ENTRYPOINT:
            sResult = QString("Debugger/Breakpoint/EntryPoint");
            break;
        case ID_DEBUGGER_BREAKPOINT_DLLMAIN:
            sResult = QString("Debugger/Breakpoint/DLLMain");
            break;
        case ID_DEBUGGER_BREAKPOINT_TLSFUNCTIONS:
            sResult = QString("Debugger/Breakpoint/TLSFunctions");
            break;
        case ID_IODRIVER_FILENAME:
            sResult = QString("IODriver/FileName");
            break;
        case ID_IODRIVER_SERVICENAME:
            sResult = QString("IODriver/ServiceName");
            break;
        case ID_STRUCTS_PATH:
            sResult = QString("Structs/Path");
            break;
        case ID_NU_RECENTFILES:
            sResult = QString("RecentFiles");
            break;
        case ID_NU_LASTDIRECTORY:
            sResult = QString("LastDirectory");
            break;
    }

    return sResult;
}

QString XOptions::getLastDirectory()
{
    QString sResult;

    bool bSaveLastDirectory = getValue(ID_FILE_SAVELASTDIRECTORY).toBool();
    QString sLastDirectory = getValue(ID_NU_LASTDIRECTORY).toString();

    if (bSaveLastDirectory && (sLastDirectory != "") && QDir().exists(sLastDirectory)) {
        sResult = sLastDirectory;
    }

    return sResult;
}

void XOptions::setLastDirectory(QString sPathName)
{
    if (getValue(ID_FILE_SAVELASTDIRECTORY).toBool()) {
        QFileInfo fi(sPathName);

        if (fi.isFile()) {
            sPathName = fi.absolutePath();
        } else if (fi.isDir()) {
            sPathName = fi.absoluteFilePath();
        }

        setValue(ID_NU_LASTDIRECTORY, sPathName);
    }
}

void XOptions::setLastFileName(QString sFileName)
{
    QFileInfo fi(sFileName);

    QString sDirectory;

    if (fi.isFile()) {
        sDirectory = fi.absolutePath();
    }

    if (getValue(ID_FILE_SAVELASTDIRECTORY).toBool()) {
        setValue(ID_NU_LASTDIRECTORY, sDirectory);
    }

    if (getValue(ID_FILE_SAVERECENTFILES).toBool()) {
        QString _sFileName = fi.absoluteFilePath();

        if (_sFileName != "") {
            QList<QVariant> listFiles = getValue(ID_NU_RECENTFILES).toList();

            listFiles.removeAll(_sFileName);

            listFiles.append(QVariant(_sFileName));

            if (listFiles.count() > g_nMaxRecentFilesCount) {
                listFiles.removeFirst();
            }

            g_mapValues.insert(ID_NU_RECENTFILES, listFiles);

#ifdef QT_GUI_LIB
            _updateRecentFilesMenu();
#endif
        }
    }
}

void XOptions::clearRecentFiles()
{
    g_mapValues.insert(ID_NU_RECENTFILES, QList<QVariant>());

#ifdef QT_GUI_LIB
    _updateRecentFilesMenu();
#endif
}

void XOptions::openRecentFile()
{
#ifdef QT_GUI_LIB
    QAction *pAction = qobject_cast<QAction *>(sender());

    if (pAction) {
        QString sFileName = pAction->data().toString();

        emit openFile(sFileName);
    }
#endif
}

void XOptions::setCodePageSlot()
{
#ifdef QT_GUI_LIB
    QAction *pAction = qobject_cast<QAction *>(sender());

    if (pAction) {
        QString sCodePage = pAction->data().toString();

        emit setCodePage(sCodePage);
    }
#endif
}

QList<QString> XOptions::getRecentFiles()
{
    QList<QString> listResult;

    QList<QVariant> listFiles = getValue(ID_NU_RECENTFILES).toList();

    qint32 nNumberOfFiles = listFiles.size();

    for (qint32 i = 0; i < nNumberOfFiles; i++) {
        listResult.append(listFiles.at(i).toString());
    }

    return listResult;
}

QString XOptions::getDatabasePath()
{
    return getValue(ID_SCAN_DATABASEPATH).toString();
}

QString XOptions::getInfoPath()
{
    return getValue(ID_INFO_PATH).toString();
}

QString XOptions::getScanEngine()
{
    return getValue(ID_SCAN_ENGINE).toString();
}

QString XOptions::getDisasmSyntax()
{
    return getValue(ID_DISASM_SYNTAX).toString();
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

QString XOptions::getVirusTotalApiKey()
{
    return getValue(ID_ONLINETOOLS_VIRUSTOTAL_APIKEY).toString();
}

#ifdef QT_GUI_LIB
void XOptions::adjustStayOnTop(QWidget *pWidget)
{
    Qt::WindowFlags wf = pWidget->windowFlags();

    if (isStayOnTop()) {
        wf |= Qt::WindowStaysOnTopHint;
    } else {
        wf &= ~(Qt::WindowStaysOnTopHint);
    }

    pWidget->setWindowFlags(wf);
    pWidget->show();
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustFont(QWidget *pWidget)
{
    QFont _font;
    QString sFont = getValue(XOptions::ID_VIEW_FONT).toString();

    if ((sFont != "") && _font.fromString(sFont)) {
        pWidget->setFont(_font);
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustWindow(QWidget *pWidget)
{
    if (isIDPresent(XOptions::ID_VIEW_STAYONTOP)) {
        adjustStayOnTop(pWidget);
    }

    if (isIDPresent(XOptions::ID_VIEW_FONT)) {
        adjustFont(pWidget);
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setMonoFont(QWidget *pWidget, qint32 nSize)
{
    QFont font = pWidget->font();

#ifdef Q_OS_WIN
    font.setFamily("Courier");
#endif
#ifdef Q_OS_LINUX
    font.setFamily("DejaVu Sans Mono");
#endif
#ifdef Q_OS_MACOS
    font.setFamily("Menlo");  // TODO Check
#endif

    if (nSize != -1) {
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
    setValue(id, pCheckBox->isChecked());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setCheckBox(QGroupBox *pGroupBox, ID id)
{
    pGroupBox->setChecked(getValue(id).toBool());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::getCheckBox(QGroupBox *pGroupBox, ID id)
{
    setValue(id, pGroupBox->isChecked());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setComboBox(QComboBox *pComboBox, XOptions::ID id)
{
    const bool bBlocked1 = pComboBox->blockSignals(true);

    pComboBox->clear();

    QString sValue = getValue(id).toString();

    if (id == ID_VIEW_STYLE) {
        pComboBox->addItem("Default", "");
        QStringList listKeys = QStyleFactory::keys();

        qint32 nNumberOfKeys = listKeys.count();

        for (qint32 i = 0; i < nNumberOfKeys; i++) {
            QString sRecord = listKeys.at(i);
            pComboBox->addItem(sRecord, sRecord);
        }
    } else if (id == ID_VIEW_LANG) {
        pComboBox->addItem("English", "");
        pComboBox->addItem("System", "System");

        QList<QString> listFileNames = getAllFilesFromDirectory(getApplicationLangPath(), "*.qm");

        qint32 nNumberOfRecords = listFileNames.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            QFileInfo fi(listFileNames.at(i));

            QString sRecord = fi.completeBaseName();

            QLocale locale(sRecord.section("_", 1, -1));
            QString sLocale = locale.nativeLanguageName();

            if (sRecord.count("_") == 2) {
                sLocale += QString("(%1)").arg(locale.nativeCountryName());
            }

            if (sLocale != "") {
                sLocale.replace(0, 1, sLocale[0].toUpper());
            }

            sLocale += QString("[%1]").arg(locale.languageToString(locale.language()));

            pComboBox->addItem(sLocale, sRecord);
        }
    } else if (id == ID_VIEW_QSS) {
        pComboBox->addItem("Default", "");

        QList<QString> listFileNames = getAllFilesFromDirectory(getApplicationQssPath(), "*.qss");

        qint32 nNumberOfRecords = listFileNames.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            QFileInfo fi(listFileNames.at(i));

            QString sRecord = fi.completeBaseName();

            pComboBox->addItem(sRecord, sRecord);
        }
    } else if (id == ID_SCAN_ENGINE) {
        pComboBox->addItem(tr("Automatic"), "auto");
        pComboBox->addItem(QString("Detect It Easy(DiE)"), "die");
        pComboBox->addItem(QString("Nauz File Detector(NFD)"), "nfd");
    } else if (id == ID_DISASM_SYNTAX) {
        pComboBox->addItem("Default", "");
        pComboBox->addItem(QString("ATT"), "ATT");
        pComboBox->addItem(QString("INTEL"), "INTEL");
        pComboBox->addItem(QString("MASM"), "MASM");
        pComboBox->addItem(QString("MOTOROLA"), "MOTOROLA");
    }

    qint32 nNumberOfItems = pComboBox->count();
    qint32 nIndex = -1;

    for (qint32 i = 0; i < nNumberOfItems; i++) {
        if (sValue == pComboBox->itemData(i, Qt::UserRole)) {
            nIndex = i;
        }
    }

    if (nIndex != -1) {
        pComboBox->setCurrentIndex(nIndex);
    }

    pComboBox->blockSignals(bBlocked1);
}
#endif
#ifdef QT_GUI_LIB
void XOptions::getComboBox(QComboBox *pComboBox, XOptions::ID id)
{
    setValue(id, pComboBox->currentData());
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
    setValue(id, pLineEdit->text());
}
#endif
bool XOptions::isSaveBackup()
{
    return getValue(XOptions::ID_FILE_SAVEBACKUP).toBool();
}

bool XOptions::isSaveLastDirectory()
{
    return getValue(XOptions::ID_FILE_SAVELASTDIRECTORY).toBool();
}

bool XOptions::isSaveRecentFiles()
{
    return getValue(XOptions::ID_FILE_SAVERECENTFILES).toBool();
}

bool XOptions::isRestartNeeded()
{
    return g_bIsNeedRestart;
}

bool XOptions::isStayOnTop()
{
    return getValue(XOptions::ID_VIEW_STAYONTOP).toBool();
}

bool XOptions::isScanAfterOpen()
{
    return getValue(XOptions::ID_SCAN_SCANAFTEROPEN).toBool();
}

bool XOptions::isRecursiveScan()
{
    return getValue(XOptions::ID_SCAN_RECURSIVE).toBool();
}

bool XOptions::isDeepScan()
{
    return getValue(XOptions::ID_SCAN_DEEP).toBool();
}

bool XOptions::isHeuristicScan()
{
    return getValue(XOptions::ID_SCAN_HEURISTIC).toBool();
}

bool XOptions::isVerbose()
{
    return getValue(XOptions::ID_SCAN_VERBOSE).toBool();
}

bool XOptions::isAllTypesScan()
{
    return getValue(XOptions::ID_SCAN_ALLTYPES).toBool();
}

bool XOptions::isSingleApplication()
{
    return getValue(XOptions::ID_VIEW_SINGLEAPPLICATION).toBool();
}

bool XOptions::isShowLogo()
{
    return getValue(XOptions::ID_VIEW_SHOWLOGO).toBool();
}

QString XOptions::getSearchSignaturesPath()
{
    return getValue(XOptions::ID_SIGNATURES_PATH).toString();
}

QString XOptions::getStructsPath()
{
    return getValue(XOptions::ID_STRUCTS_PATH).toString();
}
#ifdef QT_GUI_LIB
void XOptions::adjustApplicationView(QString sTranslationName, XOptions *pOptions)
{
    if (pOptions->isIDPresent(XOptions::ID_VIEW_STYLE)) {
        QString sStyle = pOptions->getValue(XOptions::ID_VIEW_STYLE).toString();

        if (sStyle != "") {
            qApp->setStyle(QStyleFactory::create(sStyle));
        }
    }

    if (pOptions->isIDPresent(XOptions::ID_VIEW_LANG)) {
        QTranslator *pTranslator = new QTranslator;  // Important
        QString sLang = pOptions->getValue(XOptions::ID_VIEW_LANG).toString();
        QString sLangsPath = pOptions->getApplicationLangPath();

        bool bLoad = false;

        if (sLang == "System") {
            QLocale locale = QLocale::system();
            if (locale != QLocale::English) {
                bLoad = pTranslator->load(locale, sTranslationName, "_", sLangsPath, ".qm");
            }
        } else if (sLang != "") {
            bLoad = pTranslator->load(sLang, sLangsPath);
        }

        if (bLoad) {
            qApp->installTranslator(pTranslator);
        }
    }

    if (pOptions->isIDPresent(XOptions::ID_VIEW_QSS)) {
        QString sQss = pOptions->getValue(XOptions::ID_VIEW_QSS).toString();

        if (sQss != "") {
            QString sQssFileName = pOptions->getApplicationQssPath() + QDir::separator() + QString("%1.qss").arg(sQss);

            if (QFile::exists(sQssFileName)) {
                QFile file;
                file.setFileName(sQssFileName);

                if (file.open(QIODevice::ReadOnly)) {
                    QByteArray baQss = file.readAll();
                    qApp->setStyleSheet(baQss.data());
                    file.close();
                }
            }
        }
    }

    if (pOptions->isIDPresent(XOptions::ID_VIEW_FONT)) {
        QString sFont = pOptions->getValue(XOptions::ID_VIEW_FONT).toString();

        if (sFont != "") {
            qApp->setFont(sFont);
        }
    }
}
#endif
//#ifdef QT_GUI_LIB
// void XOptions::adjustApplicationView(QString sApplicationFileName,QString
// sTranslationName)
//{
//    XOptions xOptions;

//    xOptions.setName(sApplicationFileName);

//    QList<XOptions::ID> listIDs;

//    listIDs.append(XOptions::ID_VIEW_STYLE);
//    listIDs.append(XOptions::ID_VIEW_LANG);
//    listIDs.append(XOptions::ID_VIEW_QSS);

//    xOptions.setValueIDs(listIDs);
//    xOptions.load();

//    xOptions.adjustApplicationView(sTranslationName,&xOptions);
//}
//#endif
#ifdef QT_GUI_LIB
QWidget *XOptions::getMainWidget(QWidget *pWidget)
{
    QWidget *pResult = pWidget;

    while (pResult->parent()) {
        pResult = qobject_cast<QWidget *>(pResult->parent());
    }

    return pResult;
}
#endif
#ifdef QT_GUI_LIB
QString XOptions::getModelText(QAbstractItemModel *pModel)
{
    QString sResult;

    if (pModel) {
        qint32 nNumberOfRows = pModel->rowCount();
        qint32 nNumberOfColumns = pModel->columnCount();

        QList<QString> listHeaders;
        QList<QList<QString>> listListStrings;

        for (qint32 i = 0; i < nNumberOfColumns; i++) {
            QString sHeader = pModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();

            listHeaders.append(sHeader);
        }

        for (qint32 i = 0; i < nNumberOfRows; i++) {
            QList<QString> listStrings;

            for (qint32 j = 0; j < nNumberOfColumns; j++) {
                QString sString = pModel->data(pModel->index(i, j)).toString();

                listStrings.append(sString);
            }

            listListStrings.append(listStrings);
        }

        for (qint32 i = 0; i < nNumberOfColumns; i++) {
            if (i != (nNumberOfColumns - 1)) {
                sResult += QString("%1\t").arg(listHeaders.at(i));
            } else {
                sResult += QString("%1\r\n").arg(listHeaders.at(i));
            }
        }

        // mb TODO csv,tsv,json,xml,json
        for (qint32 i = 0; i < nNumberOfRows; i++) {
            for (qint32 j = 0; j < nNumberOfColumns; j++) {
                QString sString = listListStrings.at(i).at(j);

                if (j != (nNumberOfColumns - 1)) {
                    sResult += QString("%1\t").arg(sString);
                } else {
                    sResult += QString("%1\r\n").arg(sString);
                }
            }
        }
    }

    return sResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveModel(QAbstractItemModel *pModel, QString sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sText = getModelText(pModel);

        file.resize(0);
        file.write(sText.toUtf8().data());

        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTextEdit(QTextEdit *pTextEdit, QString sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sResult = pTextEdit->toPlainText();

        file.resize(0);
        file.write(sResult.toUtf8().data());

        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::savePlainTextEdit(QPlainTextEdit *pPlainTextEdit, QString sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sResult = pPlainTextEdit->toPlainText();

        file.resize(0);
        file.write(sResult.toUtf8().data());

        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTableView(QTableView *pTableView, QString sFileName)
{
    return saveModel(pTableView->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTableWidget(QTableWidget *pTableWidget, QString sFileName)
{
    return saveModel(pTableWidget->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveListView(QListView *pListView, QString sFileName)
{
    return saveModel(pListView->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveListWidget(QListWidget *pListWidget, QString sFileName)
{
    return saveModel(pListWidget->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTreeView(QTreeView *pTreeView, QString sFileName)
{
    return saveModel(pTreeView->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTreeWidget(QTreeWidget *pTreeWidget, QString sFileName)
{
    return saveModel(pTreeWidget->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTextBrowser(QTextBrowser *pTextBrowser, QString sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sResult = pTextBrowser->toPlainText();

        file.resize(0);
        file.write(sResult.toUtf8().data());

        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTextBrowserHtml(QTextBrowser *pTextBrowser, QString sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sResult = pTextBrowser->toHtml();

        file.resize(0);
        file.write(sResult.toUtf8().data());

        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
QMenu *XOptions::createRecentFilesMenu(QWidget *pParent)
{
    g_pRecentFilesMenu = new QMenu(tr("Recent files"), pParent);

    _updateRecentFilesMenu();

    return g_pRecentFilesMenu;
}
#endif
#ifdef QT_GUI_LIB
qint32 XOptions::getCharWidth(QWidget *pWidget)
{
    const QFontMetricsF fm(pWidget->font());

    return fm.maxWidth();
}
#endif
#ifdef QT_GUI_LIB
qint32 XOptions::getCharHeight(QWidget *pWidget)
{
    const QFontMetricsF fm(pWidget->font());

    return fm.height();
}
#endif
#ifdef QT_GUI_LIB
void XOptions::showInFolder(QString sFileName)
{
    QFileInfo fi = QFileInfo(sFileName);

#if defined(Q_OS_WIN)
    QStringList slParams;
    if (!fi.isDir()) {
        slParams += QLatin1String("/select,");
    }

    slParams += QDir::toNativeSeparators(fi.canonicalFilePath());

    QProcess::startDetached("explorer.exe", slParams);
#elif defined(Q_OS_MAC)
    QStringList slParams;
    slParams << "-e";
    slParams << "tell application \"Finder\"";
    slParams << "-e";
    slParams << "activate";
    slParams << "-e";
    slParams << "select POSIX file \"" + fi.path() + "\"";
    slParams << "-e";
    slParams << "end tell";
    slParams << "-e";
    slParams << "return";
    QProcess::execute("/usr/bin/osascript", slParams);
#else
    QString sDirectory = fi.absolutePath();

    QDesktopServices::openUrl(QUrl::fromLocalFile(sDirectory));
#endif
}
#endif
#ifdef QT_GUI_LIB
void XOptions::handleFontButton(QWidget *pParent, QLineEdit *pLineEdit)
{
    QFont _font;
    _font.fromString(pLineEdit->text());

    bool bOK = false;
    _font = QFontDialog::getFont(&bOK, _font, pParent);

    if (bOK) {
        pLineEdit->setText(_font.toString());
    }
}
#endif
QString XOptions::getApplicationLangPath()
{
    QString sResult;

    sResult = getApplicationDataPath() + QDir::separator() + "lang";

    return sResult;
}

QString XOptions::getApplicationQssPath()
{
    QString sResult;

    sResult = getApplicationDataPath() + QDir::separator() + "qss";

    return sResult;
}

QList<QString> XOptions::getAllFilesFromDirectory(QString sDirectory, QString sExtension)
{
    QDir directory(sDirectory);

    return directory.entryList(QStringList() << sExtension, QDir::Files);
}

bool XOptions::checkNative()
{
    bool bResult = false;
#ifdef Q_OS_MAC
    bResult = true;
#elif defined(Q_OS_LINUX)
    QString sApplicationDirPath = qApp->applicationDirPath();

    if ((sApplicationDirPath == "/bin") || (sApplicationDirPath == "/usr/bin") || (sApplicationDirPath == "/usr/local/bin") ||
        (sApplicationDirPath.contains(QRegExp("/usr/local/bin$"))) || isAppImage()) {
        bResult = true;
    } else {
        bResult = false;
    }
#elif defined(Q_OS_WIN)
    QString sApplicationDirPath = qApp->applicationDirPath();

    if (sApplicationDirPath.contains("C:\\Program Files\\") || sApplicationDirPath.contains("C:\\Program Files (x86)\\")) {
        bResult = true;
    }
#endif

    return bResult;
}

QString XOptions::getApplicationDataPath()
{
    QString sResult;

    QString sApplicationDirPath = qApp->applicationDirPath();

#ifdef Q_OS_MAC
    sResult = sApplicationDirPath + "/../Resources";
#elif defined(Q_OS_LINUX)
    if (g_bIsNative) {
        if (sApplicationDirPath.contains(QRegExp("/usr/local/bin$"))) {
            QString sPrefix = sApplicationDirPath.section("/usr/local/bin", 0, 0);

            sResult += sPrefix + QString("/usr/local/lib/%1").arg(qApp->applicationName());
        } else {
            if (sApplicationDirPath.contains("/tmp/.mount_"))  // AppImage
            {
                sResult = sApplicationDirPath.section("/", 0, 2);
            }

            sResult += QString("/usr/lib/%1").arg(qApp->applicationName());
        }
    } else {
        sResult = sApplicationDirPath;
    }
#else
    sResult = sApplicationDirPath;
#endif

    return sResult;
}

QString XOptions::getTitle(QString sName, QString sVersion, bool bShowOS)
{
    QString sResult = QString("%1 v%2").arg(sName, sVersion);

    if (bShowOS) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
        sResult += QString(" [%3](%4)").arg(QSysInfo::prettyProductName(), QSysInfo::buildCpuArchitecture());
#else
        // TODO OS Name // For Windows Arch GetVersionExA
#endif
    }

    return sResult;
}

#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)

bool sort_code_page(const qint32 &nValue1, const qint32 &nValue2)
{
    return (qAbs(nValue1) < qAbs(nValue2));
}

QList<QString> XOptions::getCodePages(bool bAll)
{
    QList<QString> listResult;

    listResult.append("");  // Empty

    QList<qint32> list = QTextCodec::availableMibs();

    std::sort(list.begin(), list.end(), sort_code_page);

    qint32 nNumberOfRecords = list.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        qint32 nMIB = list.at(i);

        //        qDebug("%s",QTextCodec::codecForMib(nMIB)->name().data());

        bool bAdd = true;

        if (!bAll) {
            if (nMIB == 106)
                bAdd = false;  // UTF8
            else if ((nMIB >= 1013) && (nMIB <= 1019))
                bAdd = false;  // Unicode
        }

        if (bAdd) {
            QString sName = QTextCodec::codecForMib(nMIB)->name();

            listResult.append(sName);
        }
    }

    return listResult;
}
#endif
#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
#ifdef QT_GUI_LIB
QMenu *XOptions::createCodePagesMenu(QWidget *pParent, bool bAll)
{
    g_pCodePagesMenu = new QMenu(tr("Code pages"), pParent);

    if (g_pCodePagesMenu) {
        g_pCodePagesMenu->clear();

        QList<QString> listCodePages = getCodePages(bAll);

        qint32 nNumberOfRecords = listCodePages.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            QAction *pAction = new QAction(listCodePages.at(i), g_pCodePagesMenu);
            pAction->setData(listCodePages.at(i));

            connect(pAction, SIGNAL(triggered()), this, SLOT(setCodePageSlot()));

            g_pCodePagesMenu->addAction(pAction);
        }
    }

    return g_pCodePagesMenu;
}
#endif
#endif
#ifdef Q_OS_WIN
bool XOptions::registerContext(QString sApplicationName, QString sType, QString sApplicationFilePath)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell\\%2\\command").arg(sType, sApplicationName), QSettings::NativeFormat);
    settings.setValue(".", "\"" + sApplicationFilePath.replace("/", "\\") + "\" \"%1\"");

    QSettings settingsIcon(QString("HKEY_CLASSES_ROOT\\%1\\shell\\%2").arg(sType, sApplicationName), QSettings::NativeFormat);
    settingsIcon.setValue("Icon", "\"" + sApplicationFilePath.replace("/", "\\") + "\"");

    // TODO Check if not send message
    return checkContext(sApplicationName, sType);
}
#endif
#ifdef Q_OS_WIN
bool XOptions::clearContext(QString sApplicationName, QString sType)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell\\%2").arg(sType, sApplicationName), QSettings::NativeFormat);
    settings.clear();

    // TODO Check if not send message
    return !(checkContext(sApplicationName, sType));
}
#endif
#ifdef Q_OS_WIN
bool XOptions::checkContext(QString sApplicationName, QString sType)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell").arg(sType), QSettings::NativeFormat);

    return (settings.value(QString("%1/command/Default").arg(sApplicationName)).toString() != "");
}
#endif

void XOptions::setMaxRecentFilesCount(qint32 nValue)
{
    g_nMaxRecentFilesCount = nValue;
}

qint32 XOptions::getMaxRecentFilesCount()
{
    return g_nMaxRecentFilesCount;
}

#ifdef QT_GUI_LIB
void XOptions::_updateRecentFilesMenu()
{
    if (g_pRecentFilesMenu) {
        g_pRecentFilesMenu->clear();

        QList<QString> listRecentFiles = getRecentFiles();

        qint32 nNumberOfRecentFiles = listRecentFiles.count();

        for (qint32 i = nNumberOfRecentFiles - 1; i >= 0; i--) {
            QAction *pAction = new QAction(listRecentFiles.at(i), g_pRecentFilesMenu);
            pAction->setData(listRecentFiles.at(i));

            connect(pAction, SIGNAL(triggered()), this, SLOT(openRecentFile()));

            g_pRecentFilesMenu->addAction(pAction);
        }

        if (nNumberOfRecentFiles) {
            g_pRecentFilesMenu->addSeparator();

            QAction *pAction = new QAction(tr("Clear"), g_pRecentFilesMenu);

            connect(pAction, SIGNAL(triggered()), this, SLOT(clearRecentFiles()));

            g_pRecentFilesMenu->addAction(pAction);
        }

        g_pRecentFilesMenu->setEnabled(nNumberOfRecentFiles);
    }
}
#endif
XOptions::BUNDLE XOptions::getBundle()
{
    BUNDLE result = BUNDLE_UNKNOWN;

#ifdef Q_OS_WIN
#if QT_VERSION <= QT_VERSION_CHECK(5, 6, 3)
    result = BUNDLE_WINXP;
#elif (QT_VERSION_MAJOR >= 6)
    result = BUNDLE_WIN64_QT6;
#else
#ifndef Q_OS_WIN64
    result = BUNDLE_WIN32;
#else
    result = BUNDLE_WIN64;
#endif
#endif
#endif

#ifdef Q_PROCESSOR_X86_64
#ifdef Q_OS_LINUX
#if QT_VERSION == QT_VERSION_CHECK(5, 2, 1)
    result = BUNDLE_LINUX_APPIMAGE;
#elif QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    QString sProductType = QSysInfo::productType();

    if (sProductType == "ubuntu") {
        result = BUNDLE_UBUNTU_X64;
    }
    // TODO
    // ARCH

#endif
#endif
#ifdef Q_OS_MACOS
    // TODO
    // M
#endif
#endif
    return result;
}
