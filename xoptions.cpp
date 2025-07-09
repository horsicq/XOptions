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
#include "xoptions.h"

XOptions::XOptions(QObject *pParent) : QObject(pParent)
{
#ifdef QT_GUI_LIB
    g_pRecentFilesMenu = nullptr;
    g_pCodePagesMenu = nullptr;
#endif
    g_bIsNeedRestart = false;
    g_nMaxRecentFilesCount = N_MAX_RECENT_FILES_COUNT;
    g_sName = QString("%1.ini").arg(qApp->applicationName());  // default name
    //    qRegisterMetaType<QVector<int>>("QVector<int>"); // Windows debug messages

    // #ifdef QT_DEBUG
    //     getBundle();
    // #endif
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

void XOptions::setValueIDs(const QList<ID> &listVariantIDs)
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

void XOptions::removeID(ID id)
{
    g_listValueIDs.removeOne(id);
    g_mapDefaultValues.remove(id);
    g_mapValues.remove(id);
}

XOptions::GROUPID XOptions::getGroupID(ID id)
{
    static const QMap<ID, GROUPID> idToGroupMap = {
        {ID_VIEW_STAYONTOP, GROUPID_VIEW},
        {ID_VIEW_STYLE, GROUPID_VIEW},
        {ID_VIEW_QSS, GROUPID_VIEW},
        {ID_VIEW_LANG, GROUPID_VIEW},
        {ID_VIEW_SINGLEAPPLICATION, GROUPID_VIEW},
        {ID_VIEW_SHOWLOGO, GROUPID_VIEW},
        {ID_VIEW_FONT, GROUPID_VIEW},
        {ID_VIEW_FONT_CONTROLS, GROUPID_VIEW},
        {ID_VIEW_FONT_TABLEVIEWS, GROUPID_VIEW},
        {ID_VIEW_FONT_TREEVIEWS, GROUPID_VIEW},
        {ID_VIEW_FONT_TEXTEDITS, GROUPID_VIEW},
        {ID_VIEW_ADVANCED, GROUPID_VIEW},
        {ID_VIEW_SELECTSTYLE, GROUPID_VIEW},
        {ID_FILE_SAVELASTDIRECTORY, GROUPID_FILE},
        {ID_FILE_SAVERECENTFILES, GROUPID_FILE},
        {ID_FILE_SAVEBACKUP, GROUPID_FILE},
        {ID_FILE_CONTEXT, GROUPID_FILE},
        {ID_SCAN_SCANAFTEROPEN, GROUPID_SCAN},
        {ID_SCAN_FLAG_RECURSIVE, GROUPID_SCAN},
        {ID_SCAN_FLAG_DEEP, GROUPID_SCAN},
        {ID_SCAN_FLAG_HEURISTIC, GROUPID_SCAN},
        {ID_SCAN_FLAG_AGGRESSIVE, GROUPID_SCAN},
        {ID_SCAN_FLAG_VERBOSE, GROUPID_SCAN},
        {ID_SCAN_FLAG_ALLTYPES, GROUPID_SCAN},
        {ID_SCAN_FORMATRESULT, GROUPID_SCAN},
        {ID_SCAN_LOG_PROFILING, GROUPID_SCAN},
        {ID_SCAN_BUFFERSIZE, GROUPID_SCAN},
        {ID_SCAN_HIGHLIGHT, GROUPID_SCAN},
        {ID_SCAN_SORT, GROUPID_SCAN},
        {ID_SCAN_HIDEUNKNOWN, GROUPID_SCAN},
        {ID_SCAN_ENGINE, GROUPID_SCAN},
        {ID_SCAN_ENGINE_EMPTY, GROUPID_SCAN},
        {ID_SCAN_DATABASE_MAIN_PATH, GROUPID_SCAN},
        {ID_SCAN_DATABASE_EXTRA_PATH, GROUPID_SCAN},
        {ID_SCAN_DATABASE_CUSTOM_PATH, GROUPID_SCAN},
        {ID_SCAN_DATABASE_EXTRA_ENABLED, GROUPID_SCAN},
        {ID_SCAN_DATABASE_CUSTOM_ENABLED, GROUPID_SCAN},
        {ID_SCAN_YARARULESPATH, GROUPID_SCAN},
        {ID_SIGNATURES_PATH, GROUPID_SIGNATURES},
        {ID_INFO_PATH, GROUPID_INFO},
        {ID_DISASM_FONT, GROUPID_DISASM},
        {ID_DISASM_SYNTAX, GROUPID_DISASM},
        {ID_DISASM_LOCATIONCOLON, GROUPID_DISASM},
        {ID_DISASM_UPPERCASE, GROUPID_DISASM},
        {ID_DISASM_HIGHLIGHT, GROUPID_DISASM},
        {ID_DISASM_COLOR_ARROWS, GROUPID_DISASM},
        {ID_DISASM_COLOR_ARROWS_SELECTED, GROUPID_DISASM},
        {ID_DISASM_COLOR_REGS, GROUPID_DISASM},
        {ID_DISASM_COLOR_NUMBERS, GROUPID_DISASM},
        {ID_DISASM_COLOR_OPCODE, GROUPID_DISASM},
        {ID_DISASM_COLOR_REFS, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_REGS_GENERAL, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_REGS_STACK, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_REGS_SEGMENT, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_REGS_DEBUG, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_REGS_IP, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_REGS_FLAGS, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_REGS_FPU, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_REGS_XMM, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_OPCODE_CALL, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_OPCODE_RET, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_OPCODE_COND_JMP, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_OPCODE_PUSH, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_OPCODE_POP, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_OPCODE_NOP, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_OPCODE_JMP, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_OPCODE_INT3, GROUPID_DISASM},
        {ID_DISASM_COLOR_X86_OPCODE_SYSCALL, GROUPID_DISASM},
        {ID_DISASM_COLOR_ARM_REGS_GENERAL, GROUPID_DISASM},
        {ID_DISASM_COLOR_ARM_REGS_STACK, GROUPID_DISASM},
        {ID_DISASM_COLOR_ARM_OPCODE_B, GROUPID_DISASM},
        {ID_DISASM_COLOR_ARM_OPCODE_BL, GROUPID_DISASM},
        {ID_DISASM_COLOR_ARM_OPCODE_RET, GROUPID_DISASM},
        {ID_DISASM_COLOR_ARM_OPCODE_PUSH, GROUPID_DISASM},
        {ID_DISASM_COLOR_ARM_OPCODE_POP, GROUPID_DISASM},
        {ID_DISASM_COLOR_ARM_OPCODE_NOP, GROUPID_DISASM},
        {ID_DEBUGGER_COLOR_BREAKPOINT, GROUPID_DEBUGGER},
        {ID_HEX_FONT, GROUPID_HEX},
        {ID_HEX_LOCATIONCOLON, GROUPID_HEX},
        {ID_STACK_FONT, GROUPID_STACK},
        {ID_STACK_LOCATIONCOLON, GROUPID_STACK},
        {ID_REGISTERS_FONT, GROUPID_REGISTERS},
        {ID_IODRIVER_FILENAME, GROUPID_IODRIVER},
        {ID_IODRIVER_SERVICENAME, GROUPID_IODRIVER}
    };

    GROUPID result = idToGroupMap.value(id, GROUPID_UNKNOWN);

#ifdef QT_DEBUG
    if (result == GROUPID_UNKNOWN) {
        qDebug("Unknown GroupID for ID: %d", id);
    }
#endif

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

bool XOptions::isNative()
{
#ifdef X_BUILD_INSTALL
    return true;
#else
    QString sApplicationDirPath = qApp->applicationDirPath();
    sApplicationDirPath = QDir::cleanPath(sApplicationDirPath);

    bool bResult = false;
#if defined(Q_OS_MAC)
    bResult = true;
#elif defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD)
    if ((sApplicationDirPath == "/bin") || (sApplicationDirPath == "/usr/bin") || (sApplicationDirPath == "/usr/local/bin") || (sApplicationDirPath == "/app/bin") ||
        (sApplicationDirPath.contains("/usr/local/bin$")) || isAppImage()) {
        bResult = true;
    } else {
        bResult = false;
    }
#elif defined(Q_OS_WIN)
    if (sApplicationDirPath.toLower().contains(":\\program files")) {
        bResult = true;
    }
#endif

    bResult = bResult || (!QFileInfo(sApplicationDirPath).isWritable());

    return bResult;
#endif
}

bool XOptions::isAppImage()
{
#ifdef X_BUILD_APPIMAGE
    return true;
#else
    return false;
#endif
}

void XOptions::setName(const QString &sValue)
{
    g_sName = sValue;
#ifdef QT_DEBUG
#ifdef Q_OS_WIN
#ifndef Q_OS_WIN64
    g_sName += "win32.debug.ini";
#else
    g_sName += "win64.debug.ini";
#endif
#endif
#ifdef Q_OS_LINUX
    g_sName += "linux.debug.ini";
#endif
#ifdef Q_OS_MACOS
    g_sName += "macos.debug.ini";
#endif
#endif
}

void XOptions::load()
{
    QSettings *pSettings = nullptr;

    bool bIsNative = isNative();

    if (bIsNative) {
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
        } else if (g_listValueIDs.at(i) == ID_NU_LASTDIRECTORY) {
            bLastDirectory = true;
        } else if (g_listValueIDs.at(i) == ID_FILE_SAVERECENTFILES) {
            bSaveRecentFiles = true;
        } else if (g_listValueIDs.at(i) == ID_NU_RECENTFILES) {
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
                case ID_ROOTPATH: varDefault = ""; break;
                case ID_DATAPATH: varDefault = "$data/data"; break;
                case ID_JSON: varDefault = ""; break;
                case ID_STRUCTSPATH: varDefault = "$data/structs"; break;
                case ID_STRUCTS_PATH: varDefault = "$data/structs"; break;
                case ID_AUTHUSER: varDefault = ""; break;
                case ID_AUTHTOKEN: varDefault = ""; break;
                case ID_NU_RECENTFILES: varDefault = QList<QVariant>(); break;
                case ID_NU_LASTDIRECTORY: varDefault = ""; break;
                default: varDefault = "";
            }
        }

        QVariant variant = pSettings->value(sName, varDefault);

        if (!variant.toString().contains("$data")) {
            if ((id == ID_SCAN_DATABASE_MAIN_PATH) || (id == ID_SCAN_DATABASE_EXTRA_PATH) || (id == ID_SCAN_DATABASE_CUSTOM_PATH)) {
                if ((!QDir(variant.toString()).exists()) && (!QFile(variant.toString()).exists())) {
                    variant = varDefault;
                }
            } else if ((id == ID_DATAPATH) ||
                (id == ID_SIGNATURES_PATH) || (id == ID_STRUCTS_PATH) || (id == ID_STRUCTSPATH) || (id == ID_INFO_PATH) || (id == ID_SCAN_YARARULESPATH)) {
                if (!QDir(variant.toString()).exists()) {
                    variant = varDefault;
                }
            }
        }

        if (bIsNative) {
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

    bool bIsNative = isNative();

    if (bIsNative) {
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

void XOptions::setValue(XOptions::ID id, QVariant varValue)
{
    if ((id == ID_VIEW_STYLE) || (id == ID_VIEW_LANG) || (id == ID_VIEW_QSS)) {
        QVariant varOld = g_mapValues.value(id);

        if (varValue != varOld) {
            g_bIsNeedRestart = true;
        }
    }

    g_mapValues.insert(id, varValue);
}

void XOptions::clearValue(XOptions::ID id)
{
    g_mapValues.insert(id, "");
}

bool XOptions::isValuePresent(ID id)
{
    return g_mapValues.contains(id);
}

QVariant XOptions::getDefaultValue(ID id)
{
    return g_mapDefaultValues.value(id);
}

#include <QMap>
#include <QString>

QString XOptions::idToString(ID id)
{
    static const QMap<ID, QString> idMap = {
        // TODO remove
        {ID_ROOTPATH, "RootPath"},
        {ID_DATAPATH, "DataPath"},
        {ID_JSON, "Json"},
        {ID_STRUCTSPATH, "StructsPath"},
        {ID_AUTHUSER, "AuthUser"},
        {ID_AUTHTOKEN, "AuthToken"},
        // new
        {ID_VIEW_STAYONTOP, "View/StayOnTop"},
        {ID_VIEW_STYLE, "View/Style"},
        {ID_VIEW_QSS, "View/Qss"},
        {ID_VIEW_LANG, "View/Lang"},
        {ID_VIEW_SINGLEAPPLICATION, "View/SingleApplication"},
        {ID_VIEW_SHOWLOGO, "View/ShowLogo"},
        {ID_VIEW_FONT, "View/Font"},
        {ID_VIEW_FONT_CONTROLS, "View/Font/Controls"},
        {ID_VIEW_FONT_TABLEVIEWS, "View/Font/TableViews"},
        {ID_VIEW_FONT_TREEVIEWS, "View/Font/TreeViews"},
        {ID_VIEW_FONT_TEXTEDITS, "View/Font/TextEdits"},
        {ID_VIEW_ADVANCED, "View/Advanced"},
        {ID_VIEW_SELECTSTYLE, "View/SelectStyle"},
        {ID_FILE_SAVELASTDIRECTORY, "File/SaveLastDirectory"},
        {ID_FILE_SAVERECENTFILES, "File/SaveRecentFiles"},
        {ID_FILE_SAVEBACKUP, "File/SaveBackup"},
        {ID_FILE_CONTEXT, "File/Context"},
        {ID_SCAN_SCANAFTEROPEN, "Scan/ScanAfterOpen"},
        {ID_SCAN_FLAG_RECURSIVE, "Scan/Flag/Recursive"},
        {ID_SCAN_FLAG_DEEP, "Scan/Flag/Deep"},
        {ID_SCAN_FLAG_HEURISTIC, "Scan/Flag/Heuristic"},
        {ID_SCAN_FLAG_AGGRESSIVE, "Scan/Flag/Aggressive"},
        {ID_SCAN_FLAG_VERBOSE, "Scan/Flag/Verbose"},
        {ID_SCAN_FLAG_ALLTYPES, "Scan/Flag/AllTypes"},
        {ID_SCAN_FORMATRESULT, "Scan/FormatResult"},
        {ID_SCAN_LOG_PROFILING, "Scan/Log/Profiling"},
        {ID_SCAN_BUFFERSIZE, "Scan/BufferSize"},
        {ID_SCAN_HIGHLIGHT, "Scan/Highlight"},
        {ID_SCAN_SORT, "Scan/Sort"},
        {ID_SCAN_HIDEUNKNOWN, "Scan/HideUnknown"},
        {ID_SCAN_ENGINE, "Scan/Engine"},
        {ID_SCAN_ENGINE_EMPTY, "Scan/Engine"},
        {ID_SCAN_DATABASE_MAIN_PATH, "Scan/Database/Main/Path"},
        {ID_SCAN_DATABASE_EXTRA_PATH, "Scan/Database/Extra/Path"},
        {ID_SCAN_DATABASE_CUSTOM_PATH, "Scan/Database/Custom/Path"},
        {ID_SCAN_DATABASE_EXTRA_ENABLED, "Scan/Database/Extra/Enabled"},
        {ID_SCAN_DATABASE_CUSTOM_ENABLED, "Scan/Database/Custom/Enabled"},
        {ID_SCAN_YARARULESPATH, "Scan/YaraRulesPath"},
        {ID_SIGNATURES_PATH, "Signatures/Path"},
        {ID_INFO_PATH, "Info/Path"},
        {ID_ONLINETOOLS_VIRUSTOTAL_APIKEY, "OnlineTools/VirusTotalApi"},
        {ID_DISASM_FONT, "Disasm/Font"},
        {ID_DISASM_LOCATIONCOLON, "Disasm/LocationColon"},
        {ID_DISASM_UPPERCASE, "Disasm/Uppercase"},
        {ID_DISASM_HIGHLIGHT, "Disasm/Highlight"},
        {ID_DISASM_SYNTAX, "Disasm/Syntax"},
        {ID_DISASM_COLOR_ARROWS, "Disasm/Color/Arrows"},
        {ID_DISASM_COLOR_ARROWS_SELECTED, "Disasm/Color/Arrows/Selected"},
        {ID_DISASM_COLOR_REGS, "Disasm/Color/Regs"},
        {ID_DISASM_COLOR_NUMBERS, "Disasm/Color/Numbers"},
        {ID_DISASM_COLOR_OPCODE, "Disasm/Color/Opcode"},
        {ID_DISASM_COLOR_REFS, "Disasm/Color/Refs"},
        {ID_DISASM_COLOR_X86_REGS_GENERAL, "Disasm/Color/x86/Regs/General"},
        {ID_DISASM_COLOR_X86_REGS_STACK, "Disasm/Color/x86/Regs/Stack"},
        {ID_DISASM_COLOR_X86_REGS_SEGMENT, "Disasm/Color/x86/Regs/Segment"},
        {ID_DISASM_COLOR_X86_REGS_DEBUG, "Disasm/Color/x86/Regs/Debug"},
        {ID_DISASM_COLOR_X86_REGS_IP, "Disasm/Color/x86/Regs/IP"},
        {ID_DISASM_COLOR_X86_REGS_FLAGS, "Disasm/Color/x86/Regs/Flags"},
        {ID_DISASM_COLOR_X86_REGS_FPU, "Disasm/Color/x86/Regs/Float"},
        {ID_DISASM_COLOR_X86_REGS_XMM, "Disasm/Color/x86/Regs/XMM"},
        {ID_DISASM_COLOR_X86_OPCODE_CALL, "Disasm/Color/x86/Opcode/call"},
        {ID_DISASM_COLOR_X86_OPCODE_RET, "Disasm/Color/x86/Opcode/ret"},
        {ID_DISASM_COLOR_X86_OPCODE_COND_JMP, "Disasm/Color/x86/Opcode/cond_jmp"},
        {ID_DISASM_COLOR_X86_OPCODE_PUSH, "Disasm/Color/x86/Opcode/push"},
        {ID_DISASM_COLOR_X86_OPCODE_POP, "Disasm/Color/x86/Opcode/pop"},
        {ID_DISASM_COLOR_X86_OPCODE_NOP, "Disasm/Color/x86/Opcode/nop"},
        {ID_DISASM_COLOR_X86_OPCODE_JMP, "Disasm/Color/x86/Opcode/jmp"},
        {ID_DISASM_COLOR_X86_OPCODE_INT3, "Disasm/Color/x86/Opcode/int3"},
        {ID_DISASM_COLOR_X86_OPCODE_SYSCALL, "Disasm/Color/x86/Opcode/syscall"},
        {ID_DISASM_COLOR_ARM_REGS_GENERAL, "Disasm/Color/arm/Regs/General"},
        {ID_DISASM_COLOR_ARM_REGS_STACK, "Disasm/Color/arm/Regs/Stack"},
        {ID_DISASM_COLOR_ARM_OPCODE_B, "Disasm/Color/arm/Opcode/b"},
        {ID_DISASM_COLOR_ARM_OPCODE_BL, "Disasm/Color/arm/Opcode/bl"},
        {ID_DISASM_COLOR_ARM_OPCODE_RET, "Disasm/Color/arm/Opcode/ret"},
        {ID_DISASM_COLOR_ARM_OPCODE_PUSH, "Disasm/Color/arm/Opcode/push"},
        {ID_DISASM_COLOR_ARM_OPCODE_POP, "Disasm/Color/arm/Opcode/pop"},
        {ID_DISASM_COLOR_ARM_OPCODE_NOP, "Disasm/Color/arm/Opcode/nop"},
        {ID_HEX_FONT, "Hex/Font"},
        {ID_HEX_LOCATIONCOLON, "Hex/LocationColon"},
        {ID_STACK_FONT, "Stack/Font"},
        {ID_STACK_LOCATIONCOLON, "Stack/LocationColon"},
        {ID_REGISTERS_FONT, "Registers/Font"},
        {ID_DEBUGGER_COLOR_BREAKPOINT, "Debugger/Color/Breakpoint"},
        {ID_IODRIVER_FILENAME, "IODriver/FileName"},
        {ID_IODRIVER_SERVICENAME, "IODriver/ServiceName"},
        {ID_STRUCTS_PATH, "Structs/Path"},
        {ID_NU_RECENTFILES, "RecentFiles"},
        {ID_NU_LASTDIRECTORY, "LastDirectory"}
    };

    return idMap.value(id, "Unknown");
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

void XOptions::setLastDirectory(const QString &sPathName)
{
    QString _sPathName = sPathName;

    if (getValue(ID_FILE_SAVELASTDIRECTORY).toBool()) {
        QFileInfo fi(_sPathName);

        if (fi.isFile()) {
            _sPathName = fi.absolutePath();
        } else if (fi.isDir()) {
            _sPathName = fi.absoluteFilePath();
        }

        setValue(ID_NU_LASTDIRECTORY, _sPathName);
    }
}

void XOptions::setLastFileName(const QString &sFileName)
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

        if (QFile::exists(sFileName)) {
            emit openFile(sFileName);
        } else {
            emit errorMessage(QString("%1: %2").arg(tr("Cannot find file"), sFileName));
        }
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
    return getValue(ID_SCAN_DATABASE_MAIN_PATH).toString();
}

QString XOptions::getExtraDatabasePath()
{
    return getValue(ID_SCAN_DATABASE_EXTRA_PATH).toString();
}

QString XOptions::getCustomDatabasePath()
{
    return getValue(ID_SCAN_DATABASE_CUSTOM_PATH).toString();
}

QString XOptions::getScanEngine()
{
    QString sResult;

    if (isValuePresent(ID_SCAN_ENGINE)) {
        sResult = getValue(ID_SCAN_ENGINE).toString();
    } else if (isValuePresent(ID_SCAN_ENGINE_EMPTY)) {
        sResult = getValue(ID_SCAN_ENGINE_EMPTY).toString();
    }

    return sResult;
}

QString XOptions::getInfoPath()
{
    return getValue(ID_INFO_PATH).toString();
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
    if (isIDPresent(XOptions::ID_VIEW_STAYONTOP)) {
        _adjustStayOnTop(pWidget, isStayOnTop());
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::_adjustStayOnTop(QWidget *pWidget, bool bState)
{
    Qt::WindowFlags wf = pWidget->windowFlags();

    if (bState) {
        wf |= Qt::WindowStaysOnTopHint;
    } else {
        wf &= ~(Qt::WindowStaysOnTopHint);
    }

    pWidget->setWindowFlags(wf);
    pWidget->show();
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::_isAjustStayOnTop(QWidget *pWidget)
{
    bool bResult = false;

    Qt::WindowFlags wf = pWidget->windowFlags();

    if (wf & Qt::WindowStaysOnTopHint) {
        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
void XOptions::_adjustApplicationModal(QWidget *pWidget, bool bState)
{
    if (bState) {
        pWidget->setWindowModality(Qt::ApplicationModal);
    } else {
        pWidget->setWindowModality(Qt::NonModal);
    }

    pWidget->show();
}
#endif
#ifdef QT_GUI_LIB
void XOptions::_adjustFullScreen(QWidget *pWidget, bool bState)
{
    if (bState) {
        pWidget->showFullScreen();
    } else {
        pWidget->showNormal();
    }
}
#endif
#ifdef QT_GUI_LIB
QFont XOptions::adjustFont(QWidget *pWidget, ID id)
{
    QFont result;
    QString sFont = getValue(id).toString();

    if ((sFont != "") && result.fromString(sFont)) {
        QFont fontOld = pWidget->font();

        result.setBold(fontOld.bold());

        pWidget->setFont(result);
    }

    return result;
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustWindow(QWidget *pWidget)
{
    if (isIDPresent(XOptions::ID_VIEW_STAYONTOP)) {
        adjustStayOnTop(pWidget);
    }

    adjustWidget(pWidget, XOptions::ID_VIEW_FONT);
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustWidget(QWidget *pWidget, ID id)
{
    if (isIDPresent(id)) {
        adjustFont(pWidget, id);
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustTableWidget(QTableWidget *pWidget, ID id)
{
    if (isIDPresent(id)) {
        QFont _font = adjustFont(pWidget, id);

        const QFontMetricsF fm(_font);

        qint32 nCharHeight = fm.height();

        pWidget->verticalHeader()->setMinimumSectionSize(nCharHeight + 3);
        pWidget->verticalHeader()->setDefaultSectionSize(nCharHeight + 3);
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustTableView(QTableView *pWidget, ID id)
{
    if (isIDPresent(id)) {
        QFont _font = adjustFont(pWidget, id);

        const QFontMetricsF fm(_font);

        qint32 nCharHeight = fm.height();

        pWidget->verticalHeader()->setMinimumSectionSize(nCharHeight + 3);
        pWidget->verticalHeader()->setDefaultSectionSize(nCharHeight + 3);
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustTreeWidget(QTreeWidget *pWidget, ID id, bool bAdjustSize)
{
    if (isIDPresent(id)) {
        adjustFont(pWidget, id);
    }

    if (bAdjustSize) {
        adjustTreeWidgetSize(pWidget);
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustTreeView(QTreeView *pWidget, ID id)
{
    if (isIDPresent(id)) {
        adjustFont(pWidget, id);
    }
}
#endif
#ifdef QT_GUI_LIB
QFont XOptions::getDefaultFont(qint32 nFontSize)
{
    QFont fontResult;

    if (nFontSize == -1) {
#ifdef Q_OS_MACOS
        nFontSize = 13;
#endif
#ifdef Q_OS_WIN
        nFontSize = 9;
#endif
#ifdef Q_OS_LINUX
        nFontSize = 11;
#endif
    }

#ifdef Q_OS_WIN
    fontResult = QFont("Segoe UI", nFontSize);
#endif
#ifdef Q_OS_LINUX
    fontResult = QFont("Ubuntu", nFontSize);
#endif
#ifdef Q_OS_MACOS
    fontResult = QFont(".AppleSystemUIFont", nFontSize);
#endif

    return fontResult;
}
#endif
#ifdef QT_GUI_LIB
QFont XOptions::getMonoFont(qint32 nFontSize)
{
    QFont fontResult;

    if (nFontSize == -1) {
#ifdef Q_OS_MACOS
        nFontSize = 12;
#endif
#ifdef Q_OS_WIN
        nFontSize = 10;
#endif
#ifdef Q_OS_LINUX
        nFontSize = 10;
#endif
    }

#ifdef Q_OS_WIN
    fontResult = QFont("Courier", nFontSize);
#endif
#ifdef Q_OS_LINUX
    fontResult = QFont("DejaVu Sans Mono", nFontSize);
#endif
#ifdef Q_OS_MACOS
    fontResult = QFont("Menlo", nFontSize);
#endif

    return fontResult;
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setMonoFont(QWidget *pWidget, qint32 nSize)
{
    QFont font = pWidget->font();
    QFont fontMono = getMonoFont(nSize);

    font.setFamily(fontMono.family());
    font.setPointSize(fontMono.pointSize());

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
        pComboBox->addItem(QString("Detect It Easy (DiE)"), "die");
        pComboBox->addItem(QString("Nauz File Detector (NFD)"), "nfd");
#ifdef USE_YARA
        pComboBox->addItem(QString("Yara rules"), "yara");
#endif
    } else if (id == ID_SCAN_ENGINE_EMPTY) {
        pComboBox->addItem("", "");
        pComboBox->addItem(QString("Detect It Easy (DiE)"), "die");
        pComboBox->addItem(QString("Nauz File Detector (NFD)"), "nfd");
#ifdef USE_YARA
        pComboBox->addItem(QString("Yara rules"), "yara");
#endif
    } else if (id == ID_DISASM_SYNTAX) {
        pComboBox->addItem("Default", "");
        pComboBox->addItem(QString("ATT"), "ATT");
        pComboBox->addItem(QString("INTEL"), "INTEL");
        pComboBox->addItem(QString("MASM"), "MASM");
        pComboBox->addItem(QString("MOTOROLA"), "MOTOROLA");
    } else if (id == ID_SCAN_BUFFERSIZE) {
        pComboBox->addItem("", 0);
        pComboBox->addItem("1 MiB", 1 * 1024 * 1024);
        pComboBox->addItem("2 MiB", 2 * 1024 * 1024);
        pComboBox->addItem("4 MiB", 4 * 1024 * 1024);
        pComboBox->addItem("8 MiB", 8 * 1024 * 1024);
        pComboBox->addItem("16 MiB", 16 * 1024 * 1024);
        pComboBox->addItem("32 MiB", 32 * 1024 * 1024);
        pComboBox->addItem("64 MiB", 64 * 1024 * 1024);
        pComboBox->addItem("128 MiB", 128 * 1024 * 1024);
        pComboBox->addItem("256 MiB", 256 * 1024 * 1024);
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

void XOptions::adjustApplicationInitAttributes()
{
#ifdef QT_GUI_LIB
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#endif
#endif
}
#ifdef QT_GUI_LIB
QColor XOptions::stringToColor(QString sColor)
{
    QColor result;

    if (sColor != "") {
        result.setNamedColor(sColor);
    }

    return result;
}
#endif
#ifdef QT_GUI_LIB
QString XOptions::colorToString(const QColor &color)
{
    return color.name();
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustApplicationView(const QString &sTranslationName, XOptions *pOptions)
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
#ifdef QT_GUI_LIB
void XOptions::adjustListWidgetSize(QListWidget *pListWidget, qint32 nMinimumWidth)
{
    qint32 nWidth = nMinimumWidth;

    qint32 nNumberOfRecords = pListWidget->count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        QString sText = pListWidget->item(i)->text() + "WWWW";
        nWidth = qMax(QFontMetrics(pListWidget->item(i)->font()).size(Qt::TextSingleLine, sText).width(), nWidth);
    }

    pListWidget->setMinimumWidth(0);
    pListWidget->setMaximumWidth(nWidth);
}
#endif
#ifdef QT_GUI_LIB
qint32 XOptions::_getTreeWidgetItemSize(QTreeWidget *pTreeWidget, QTreeWidgetItem *pTreeWidgetItem, qint32 nIndent, qint32 nLevel)
{
    qint32 nResult = 0;

    QString sText = pTreeWidgetItem->text(0) + "WW";

    QFont _font = pTreeWidget->font();
    QString sFont = _font.toString();

    nResult = QFontMetrics(_font).size(Qt::TextSingleLine, sText).width() + ((nIndent + 16) * nLevel);  // 16 size of icon

    qint32 nNumberOfRecords = pTreeWidgetItem->childCount();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        nResult = qMax(_getTreeWidgetItemSize(pTreeWidget, pTreeWidgetItem->child(i), nIndent, nLevel + 1), nResult);
    }

    return nResult;
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustTreeWidgetSize(QTreeWidget *pTreeWidget, qint32 nMinimumWidth)
{
    qint32 nWidth = nMinimumWidth;
    qint32 nIndent = pTreeWidget->indentation();
    qint32 nNumberOfItems = pTreeWidget->topLevelItemCount();

    for (qint32 i = 0; i < nNumberOfItems; i++) {
        nWidth = qMax(_getTreeWidgetItemSize(pTreeWidget, pTreeWidget->topLevelItem(i), nIndent, 1), nWidth);
    }

    pTreeWidget->setMinimumWidth(0);
    pTreeWidget->setMaximumWidth(nWidth);
}
#endif
// #ifdef QT_GUI_LIB
//  void XOptions::adjustApplicationView(QString sApplicationFileName,QString
//  sTranslationName)
//{
//     XOptions xOptions;

//    xOptions.setName(sApplicationFileName);

//    QList<XOptions::ID> listIDs;

//    listIDs.append(XOptions::ID_VIEW_STYLE);
//    listIDs.append(XOptions::ID_VIEW_LANG);
//    listIDs.append(XOptions::ID_VIEW_QSS);

//    xOptions.setValueIDs(listIDs);
//    xOptions.load();

//    xOptions.adjustApplicationView(sTranslationName,&xOptions);
//}
// #endif
#ifdef QT_GUI_LIB
QWidget *XOptions::getMainWidget(QWidget *pWidget)
{
    QWidget *pResult = pWidget;

    if (pResult) {
        while (pResult->parent()) {
            pResult = qobject_cast<QWidget *>(pResult->parent());
        }
    }

    return pResult;
}
#endif
#ifdef QT_GUI_LIB
QString XOptions::getTableModelText(QAbstractItemModel *pModel)
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
        qint32 _nNumberOfLines = listListStrings.count();

        for (qint32 i = 0; i < _nNumberOfLines; i++) {
            qint32 _nNumberOfColumns = listListStrings.at(i).count();

            for (qint32 j = 0; j < _nNumberOfColumns; j++) {
                QString sString = listListStrings.at(i).at(j);

                if (j != (_nNumberOfColumns - 1)) {
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
void XOptions::_getTreeModelText(QString *psString, QAbstractItemModel *pModel, QModelIndex index, qint32 nLevel)
{
    qint32 nNumberOfRows = pModel->rowCount(index);
    qint32 nNumberOfColumns = pModel->columnCount(index);

    for (qint32 i = 0; i < nNumberOfRows; i++) {
        QString sPrefix;
        sPrefix = sPrefix.leftJustified(4 * nLevel, ' ');

        *psString += sPrefix;

        for (qint32 j = 0; j < nNumberOfColumns; j++) {
            QString sString = pModel->data(pModel->index(i, j, index)).toString();

            if (j != (nNumberOfColumns - 1)) {
                *psString += QString("%1\t").arg(sString);
            } else {
                *psString += QString("%1\r\n").arg(sString);
            }
        }

        if (nNumberOfColumns) {
            _getTreeModelText(psString, pModel, pModel->index(i, 0, index), nLevel + 1);
        }
    }
}
#endif
#ifdef QT_GUI_LIB
QString XOptions::getTreeModelText(QAbstractItemModel *pModel)
{
    QString sResult;

    if (pModel) {
        _getTreeModelText(&sResult, pModel, QModelIndex(), 0);
    }

    return sResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTableModel(QAbstractItemModel *pModel, const QString &sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sText = getTableModelText(pModel);

        file.resize(0);
        file.write(sText.toUtf8().data());
        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTreeModel(QAbstractItemModel *pModel, const QString &sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sText = getTreeModelText(pModel);

        file.resize(0);
        file.write(sText.toUtf8().data());
        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTextEdit(QTextEdit *pTextEdit, const QString &sFileName)
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
bool XOptions::savePlainTextEdit(QPlainTextEdit *pPlainTextEdit, const QString &sFileName)
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
bool XOptions::saveTableView(QTableView *pTableView, const QString &sFileName)
{
    return saveTableModel(pTableView->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTableWidget(QTableWidget *pTableWidget, const QString &sFileName)
{
    return saveTableModel(pTableWidget->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveListView(QListView *pListView, const QString &sFileName)
{
    return saveTableModel(pListView->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveListWidget(QListWidget *pListWidget, const QString &sFileName)
{
    return saveTableModel(pListWidget->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTreeView(QTreeView *pTreeView, const QString &sFileName)
{
    return saveTreeModel(pTreeView->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTreeWidget(QTreeWidget *pTreeWidget, const QString &sFileName)
{
    return saveTreeModel(pTreeWidget->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTextBrowser(QTextBrowser *pTextBrowser, const QString &sFileName)
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
bool XOptions::saveTextBrowserHtml(QTextBrowser *pTextBrowser, const QString &sFileName)
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
qint32 XOptions::getControlWidth(QWidget *pWidget, qint32 nSymbolSize)
{
    QFontMetrics fm(pWidget->font());

    QString sText = QString(nSymbolSize, '0') + QString(2, ' ');

    return fm.boundingRect(sText).width();
}
#endif
#ifdef QT_GUI_LIB
void XOptions::showInFolder(const QString &sFileName)
{
    // TODO https://github.com/qt-creator/qt-creator/blob/master/src/plugins/coreplugin/fileutils.cpp#L67
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
    QDesktopServices::openUrl(QUrl::fromLocalFile(fi.path()));
#endif
}
#endif
#ifdef QT_GUI_LIB
void XOptions::showFolder(const QString &sDirectory)
{
#if defined(Q_OS_WIN)
    QStringList slParams;

    slParams += QDir::toNativeSeparators(sDirectory);

    QProcess::startDetached("explorer.exe", slParams);
#elif defined(Q_OS_MAC)
    QStringList slParams;
    slParams << "-e";
    slParams << "tell application \"Finder\"";
    slParams << "-e";
    slParams << "activate";
    slParams << "-e";
    slParams << "select POSIX file \"" + sDirectory + "\"";  // TODO Check
    slParams << "-e";
    slParams << "end tell";
    slParams << "-e";
    slParams << "return";
    QProcess::execute("/usr/bin/osascript", slParams);
#else
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
#ifdef QT_GUI_LIB
void XOptions::setModelTextAlignment(QStandardItemModel *pModel, qint32 nColumn, Qt::Alignment flag)
{
    qint32 nNumberOfRows = pModel->rowCount();

    pModel->setHeaderData(nColumn, Qt::Horizontal, (qint32)flag, Qt::TextAlignmentRole);

    for (qint32 i = 0; i < nNumberOfRows; i++) {
        QStandardItem *pItem = pModel->item(i, nColumn);

        if (pItem) {
            pItem->setTextAlignment(flag);
            // pModel->setData(pModel->index(i, nColumn), (qint32)flag, Qt::TextAlignmentRole);

            QModelIndex index = pModel->index(i, 0);
            qint32 _nNumberOfRows = pModel->rowCount(index);

            for (qint32 j = 0; j < _nNumberOfRows; j++) {
                pModel->setData(pModel->index(j, nColumn, index), (qint32)flag, Qt::TextAlignmentRole);
            }
        }
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setTableViewHeaderWidth(QTableView *pTableView, qint32 nColumn, qint32 nContentWidth)
{
    QFont font = pTableView->font();
    font.setBold(true);

    const QFontMetricsF fm(font);

    QString sTitle = pTableView->model()->headerData(nColumn, Qt::Horizontal, Qt::DisplayRole).toString();

    qreal rWidth = fm.boundingRect(sTitle + "  ").width();

    nContentWidth = qMax(nContentWidth, (qint32)(rWidth));

    pTableView->setColumnWidth(nColumn, nContentWidth);
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setTreeViewHeaderWidth(QTreeView *pTreeView, qint32 nColumn, qint32 nContentWidth)
{
    QFont font = pTreeView->font();
    font.setBold(true);

    const QFontMetricsF fm(font);

    QString sTitle = pTreeView->model()->headerData(nColumn, Qt::Horizontal, Qt::DisplayRole).toString();

    qreal rWidth = fm.boundingRect(sTitle + "  ").width();

    nContentWidth = qMax(nContentWidth, (qint32)(rWidth));

    pTreeView->setColumnWidth(nColumn, nContentWidth);
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setTableWidgetHeaderAlignment(QTableWidget *pTableWidget, qint32 nColumn, Qt::Alignment flag)
{
    pTableWidget->model()->setHeaderData(nColumn, Qt::Horizontal, (qint32)flag, Qt::TextAlignmentRole);
}
#endif

void XOptions::deleteQObjectList(QList<QObject *> *pList)
{
    qint32 nNumberOfRecords = pList->count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        delete pList->at(i);
    }
}

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

QList<QString> XOptions::getAllFilesFromDirectory(const QString &sDirectory, const QString &sExtension)
{
    QDir directory(sDirectory);

    return directory.entryList(QStringList() << sExtension, QDir::Files);
}

bool XOptions::checkNative(const QString &sIniFileName)
{
    Q_UNUSED(sIniFileName)

    QString sApplicationDirPath = qApp->applicationDirPath();
    sApplicationDirPath = QDir::cleanPath(sApplicationDirPath);

    bool bResult = false;
#if defined(Q_OS_MAC)
    bResult = true;
#elif defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD)
    if ((sApplicationDirPath == "/bin") || (sApplicationDirPath == "/usr/bin") || (sApplicationDirPath == "/usr/local/bin") || (sApplicationDirPath == "/app/bin") ||
        (sApplicationDirPath.contains("/usr/local/bin$")) || isAppImage()) {
        bResult = true;
    } else {
        bResult = false;
    }
#elif defined(Q_OS_WIN)
    if (sApplicationDirPath.toLower().contains(":\\program files")) {
        bResult = true;
    }
#endif

    bResult = bResult || (!QFileInfo(sApplicationDirPath).isWritable());

    return bResult;
}

QString XOptions::getApplicationDataPath()
{
    QString sResult;

#ifdef Q_OS_MAC
    sResult = sApplicationDirPath + "/../Resources";
#endif
#ifdef Q_OS_WIN
    if (isNative()) {
        sResult = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        if (!sResult.isEmpty()) {
            sResult += QDir::separator() + qApp->applicationName();
        }
    } else {
        sResult = qApp->applicationDirPath();
    }
#endif
#ifdef Q_OS_LINUX
    if (isNative()) {
        QString sApplicationDirPath = qApp->applicationDirPath();

        if (sApplicationDirPath.contains("/usr/local/bin$")) {
            QString sPrefix = sApplicationDirPath.section("/usr/local/bin", 0, 0);

            sResult += sPrefix + QString("/usr/local/lib/%1").arg(qApp->applicationName());
        } else if (sApplicationDirPath.startsWith("/app/bin")) {  // Flatpak
            sResult += QString("/app/lib/%1").arg(qApp->applicationName());
        } else {
            if (sApplicationDirPath.contains("/tmp/.mount_"))  // AppImage
            {
                sResult = sApplicationDirPath.section("/", 0, 2);
            }

            sResult += QString("/usr/lib/%1").arg(qApp->applicationName());
        }
    } else {
        sResult = qApp->applicationDirPath();
    }
#endif
#ifdef X_BUILD_APPIMAGE
    QString sApplicationDirPath = qApp->applicationDirPath();
    if (sApplicationDirPath.contains("/tmp/.mount_")) {
        sResult = sApplicationDirPath.section("/", 0, 2);
    }
    sResult += QString("/usr/lib/%1").arg(qApp->applicationName());
#endif
#ifdef X_BUILD_FLATPACK
    sResult = QString("/app/lib/%1").arg(qApp->applicationName());
#endif
#ifdef Q_OS_FREEBSD
    sResult = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(1) + QDir::separator() + qApp->applicationName();
#endif

    return sResult;
}

QString XOptions::getTitle(const QString &sName, const QString &sVersion, bool bShowOS)
{
    QString sResult = QString("%1 v%2").arg(sName, sVersion);

    if (bShowOS) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
        // TODO Check Windows 11
        QString architecture = QSysInfo::buildCpuArchitecture();
        if (architecture == "x86_64") {
            architecture = "x64";
        } else if (architecture == "i386" || architecture == "i686") {
            architecture = "x86";
        }
        sResult += QString(" [%1] (%2)").arg(QSysInfo::prettyProductName(), architecture);
#else
        // TODO OS Name // For Windows Arch GetVersionExA
#endif
    }

    return sResult;
}

bool XOptions::isWritable()
{
    bool bResult = false;
    QSettings *pSettings = nullptr;

    bool bIsNative = isNative();

    if (bIsNative) {
        pSettings = new QSettings;
    } else {
        pSettings = new QSettings(getApplicationDataPath() + QDir::separator() + QString("%1").arg(g_sName), QSettings::IniFormat);
    }

    bResult = pSettings->isWritable();

    delete pSettings;

    return bResult;
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
            if (nMIB == 106) bAdd = false;                            // UTF8
            else if ((nMIB >= 1013) && (nMIB <= 1019)) bAdd = false;  // Unicode
        }

        if (bAdd) {
            QString sName = QTextCodec::codecForMib(nMIB)->name();

            listResult.append(sName);
        }
    }

    return listResult;
}
#endif
void XOptions::registerCodecs()
{
#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
    {
        codec_cp437 *pCodec = new codec_cp437;

        if (!pCodec) {
            qFatal("Codec failed");
        }
        // TODO more codecs

        // delete pCodec; // TODO unregisterCodecs
    }
#endif
}
#ifndef QT_GUI_LIB
void XOptions::printConsole(QString sString, Qt::GlobalColor colorText, Qt::GlobalColor colorBackground)
{
#ifdef Q_OS_WIN
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi = {};
    WORD wOldAttribute = 0;

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        wOldAttribute = csbi.wAttributes;
    }

    if (colorText != Qt::transparent || colorBackground != Qt::transparent) {
        WORD wAttribute = 0;

        switch (colorText) {
        case Qt::black:        wAttribute |= 0; break;
        case Qt::white:        wAttribute |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        case Qt::blue:         wAttribute |= FOREGROUND_BLUE; break;
        case Qt::red:          wAttribute |= FOREGROUND_RED; break;
        case Qt::green:        wAttribute |= FOREGROUND_GREEN; break;
        case Qt::yellow:       wAttribute |= FOREGROUND_RED | FOREGROUND_GREEN; break;
        case Qt::magenta:      wAttribute |= FOREGROUND_RED | FOREGROUND_BLUE; break;
        case Qt::cyan:         wAttribute |= FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        case Qt::darkBlue:     wAttribute |= FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case Qt::darkRed:      wAttribute |= FOREGROUND_RED | FOREGROUND_INTENSITY; break;
        case Qt::darkGreen:    wAttribute |= FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case Qt::darkYellow:   wAttribute |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case Qt::darkMagenta:  wAttribute |= FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case Qt::darkCyan:     wAttribute |= FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case Qt::gray:         wAttribute |= FOREGROUND_INTENSITY; break;
        case Qt::darkGray:     wAttribute |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case Qt::transparent:  wAttribute |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        }

        switch (colorBackground) {
        case Qt::black:        wAttribute |= 0; break;
        case Qt::white:        wAttribute |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE; break;
        case Qt::blue:         wAttribute |= BACKGROUND_BLUE; break;
        case Qt::red:          wAttribute |= BACKGROUND_RED; break;
        case Qt::green:        wAttribute |= BACKGROUND_GREEN; break;
        case Qt::yellow:       wAttribute |= BACKGROUND_RED | BACKGROUND_GREEN; break;
        case Qt::magenta:      wAttribute |= BACKGROUND_RED | BACKGROUND_BLUE; break;
        case Qt::cyan:         wAttribute |= BACKGROUND_GREEN | BACKGROUND_BLUE; break;
        case Qt::darkBlue:     wAttribute |= BACKGROUND_BLUE | BACKGROUND_INTENSITY; break;
        case Qt::darkRed:      wAttribute |= BACKGROUND_RED | BACKGROUND_INTENSITY; break;
        case Qt::darkGreen:    wAttribute |= BACKGROUND_GREEN | BACKGROUND_INTENSITY; break;
        case Qt::darkYellow:   wAttribute |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY; break;
        case Qt::darkMagenta:  wAttribute |= BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY; break;
        case Qt::darkCyan:     wAttribute |= BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY; break;
        case Qt::gray:         wAttribute |= BACKGROUND_INTENSITY; break;
        case Qt::darkGray:     wAttribute |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY; break;
        case Qt::transparent:  break;
        }

        if (wAttribute) {
            SetConsoleTextAttribute(hConsole, wAttribute);
        }
    }
#else
    if (colorText != Qt::transparent || colorBackground != Qt::transparent) {
        int fg = 39;
        int bg = 49;

        switch (colorText) {
        case Qt::black:        fg = 30; break;
        case Qt::red:          fg = 31; break;
        case Qt::green:        fg = 32; break;
        case Qt::yellow:       fg = 33; break;
        case Qt::blue:         fg = 34; break;
        case Qt::magenta:      fg = 35; break;
        case Qt::cyan:         fg = 36; break;
        case Qt::white:        fg = 37; break;
        case Qt::gray:         fg = 90; break;
        case Qt::darkRed:      fg = 91; break;
        case Qt::darkGreen:    fg = 92; break;
        case Qt::darkYellow:   fg = 93; break;
        case Qt::darkBlue:     fg = 94; break;
        case Qt::darkMagenta:  fg = 95; break;
        case Qt::darkCyan:     fg = 96; break;
        case Qt::darkGray:     fg = 90; break;
        default:               fg = 39; break;
        }

        switch (colorBackground) {
        case Qt::black:        bg = 40; break;
        case Qt::red:          bg = 41; break;
        case Qt::green:        bg = 42; break;
        case Qt::yellow:       bg = 43; break;
        case Qt::blue:         bg = 44; break;
        case Qt::magenta:      bg = 45; break;
        case Qt::cyan:         bg = 46; break;
        case Qt::white:        bg = 47; break;
        case Qt::gray:         bg = 100; break;
        case Qt::darkRed:      bg = 101; break;
        case Qt::darkGreen:    bg = 102; break;
        case Qt::darkYellow:   bg = 103; break;
        case Qt::darkBlue:     bg = 104; break;
        case Qt::darkMagenta:  bg = 105; break;
        case Qt::darkCyan:     bg = 106; break;
        case Qt::darkGray:     bg = 100; break;
        default:               bg = 49; break;
        }

        printf("\033[%d;%dm", fg, bg);
    }
#endif

    printf("%s", sString.toUtf8().data());

#ifdef Q_OS_WIN
    if (colorText != Qt::transparent || colorBackground != Qt::transparent) {
        if (wOldAttribute) {
            SetConsoleTextAttribute(hConsole, wOldAttribute);
        }
    }
#else
    if (colorText != Qt::transparent || colorBackground != Qt::transparent) {
        printf("\033[0m");
    }
#endif
}
#endif
#ifndef QT_GUI_LIB
void XOptions::printModel(QAbstractItemModel *pModel)
{
    if (pModel) {
        qint32 nNumberOfRows = pModel->rowCount();
        qint32 nNumberOfColumns = pModel->columnCount();

        printConsole("TST");

        // QList<QString> listHeaders;
        // QList<QList<QString>> listListStrings;

        // for (qint32 i = 0; i < nNumberOfColumns; i++) {
        //     QString sHeader = pModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();

        //     listHeaders.append(sHeader);
        // }

        // for (qint32 i = 0; i < nNumberOfRows; i++) {
        //     QList<QString> listStrings;

        //     for (qint32 j = 0; j < nNumberOfColumns; j++) {
        //         QString sString = pModel->data(pModel->index(i, j)).toString();

        //         listStrings.append(sString);
        //     }

        //     listListStrings.append(listStrings);
        // }

        // for (qint32 i = 0; i < nNumberOfColumns; i++) {
        //     if (i != (nNumberOfColumns - 1)) {
        //         sResult += QString("%1\t").arg(listHeaders.at(i));
        //     } else {
        //         sResult += QString("%1\r\n").arg(listHeaders.at(i));
        //     }
        // }

        // // mb TODO csv,tsv,json,xml,json
        // qint32 _nNumberOfLines = listListStrings.count();

        // for (qint32 i = 0; i < _nNumberOfLines; i++) {
        //     qint32 _nNumberOfColumns = listListStrings.at(i).count();

        //     for (qint32 j = 0; j < _nNumberOfColumns; j++) {
        //         QString sString = listListStrings.at(i).at(j);

        //         if (j != (_nNumberOfColumns - 1)) {
        //             sResult += QString("%1\t").arg(sString);
        //         } else {
        //             sResult += QString("%1\r\n").arg(sString);
        //         }
        //     }
        // }
    }
}
#endif

#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
#ifdef QT_GUI_LIB
QMenu *XOptions::createCodePagesMenu(QWidget *pParent, bool bAll)
{
    g_pCodePagesMenu = new QMenu(tr("Code pages"), pParent);

    if (g_pCodePagesMenu) {
        g_pCodePagesMenu->clear();  // TODO Check

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
QString XOptions::getClassesPrefix(USERROLE userRole)
{
    QString sResult;

    if (userRole == USERROLE_ADMIN) {
        sResult = "HKEY_CLASSES_ROOT";
    } else if (userRole == USERROLE_NORMAL) {
        sResult = "HKEY_CURRENT_USER\\Software\\Classes";
    }

    return sResult;
}
#endif
#ifdef Q_OS_WIN
bool XOptions::registerContext(const QString &sApplicationName, const QString &sType, const QString &sApplicationFilePath, USERROLE userRole)
{
    QString _sApplicationFilePath = sApplicationFilePath;

    QSettings settings(getClassesPrefix(userRole) + QString("\\%1\\shell\\%2\\command").arg(sType, sApplicationName), QSettings::NativeFormat);
    settings.setValue(".", "\"" + _sApplicationFilePath.replace("/", "\\") + "\" \"%1\"");

    QSettings settingsIcon(getClassesPrefix(userRole) + QString("\\%1\\shell\\%2").arg(sType, sApplicationName), QSettings::NativeFormat);
    settingsIcon.setValue("Icon", "\"" + _sApplicationFilePath.replace("/", "\\") + "\"");

    // TODO Check if not send message
    return checkContext(sApplicationName, sType);
}
#endif
#ifdef Q_OS_WIN
bool XOptions::clearContext(const QString &sApplicationName, const QString &sType, USERROLE userRole)
{
    QSettings settings(getClassesPrefix(userRole) + QString("\\%1\\shell\\%2").arg(sType, sApplicationName), QSettings::NativeFormat);
    settings.clear();

    // TODO Check if not send message
    return !(checkContext(sApplicationName, sType));
}
#endif
#ifdef Q_OS_WIN
bool XOptions::checkContext(const QString &sApplicationName, const QString &sType, USERROLE userRole)
{
    QSettings settings(getClassesPrefix(userRole) + QString("\\%1\\shell").arg(sType), QSettings::NativeFormat);

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

QString XOptions::getBundleIdToString(BUNDLE bundle)
{
    static const QMap<BUNDLE, QString> bundleMap = {
        {BUNDLE_LINUX_ARCH_X64, "Linux Arch x64"},
        {BUNDLE_WINDOWS_QT6_X64, "Windows Qt6 x64"},
        {BUNDLE_LINUX_APPIMAGE_X64, "Linux AppImage x64"},
        {BUNDLE_LINUX_DEBIAN_X64, "Linux Debian x64"},
        {BUNDLE_LINUX_UBUNTU_X64, "Linux Ubuntu x64"},
        {BUNDLE_LINUX_PARROT_X64, "Linux Parrot x64"},
        {BUNDLE_LINUX_KALI_X64, "Linux Kali x64"},
        {BUNDLE_WINDOWS_XP_X86, "Windows XP x86"},
        {BUNDLE_WINDOWS_X86, "Windows x86"},
        {BUNDLE_WINDOWS_X64, "Windows x64"},
        {BUNDLE_WINDOWS_ARM64, "Windows ARM64"},
        {BUNDLE_MACOS_X64, "MacOS x64"},
        {BUNDLE_MACOS_QT6_ARM64, "MacOS Qt6 ARM64"},
        {BUNDLE_FREEBSD_X64, "FreeBSD x64"}
    };

    return bundleMap.value(bundle, tr("Unknown"));
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
#ifdef QT_GUI_LIB
QString XOptions::getImageFilter()
{
    const QList<QByteArray> listImageFormats = QImageWriter::supportedImageFormats();

    QStringList listFilter;

    qint32 nNumberOfImageFormats = listImageFormats.count();

    if (nNumberOfImageFormats) {
        QString sImageFilter = tr("Images") + " (";

        for (qint32 i = 0; i < nNumberOfImageFormats; i++) {
            if (i > 0) {
                sImageFilter += " ";
            }

            sImageFilter += "*.";
            sImageFilter += listImageFormats.at(i);
        }

        sImageFilter += ")";

        listFilter.append(sImageFilter);
    }

    listFilter.append(QString("PDF %1 (*.pdf)").arg(tr("Documents")));
    //    listFilter.append(QString("Postscript %1 (*.ps)").arg(tr("Documents")));

    return listFilter.join(";;");
}
#endif
#ifdef QT_GUI_LIB
QColor XOptions::getColorDialog(QWidget *pParent, const QString &sTitle, QColor &color)
{
    QColor colResult;

    QColorDialog colorDialog(pParent);
    colorDialog.setCurrentColor(color);
    colorDialog.setWindowTitle(sTitle);

    _adjustApplicationModal(&colorDialog, true);  // TODO Check on Ubuntu
    _adjustStayOnTop(&colorDialog, true);

    if (colorDialog.exec() == QDialog::Accepted) {
        colResult = colorDialog.currentColor();
    }

    return colResult;
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustToolButton(QToolButton *pToolButton, ICONTYPE iconType, Qt::ToolButtonStyle style)
{
    QString sIconName = getIconPath(iconType);

    if (sIconName != "") {
        QIcon icon;
        icon.addFile(sIconName, QSize(), QIcon::Normal, QIcon::Off);
        pToolButton->setIcon(icon);
        pToolButton->setIconSize(QSize(16, 16));
        pToolButton->setToolButtonStyle(style);
    } else {
#ifdef QT_DEBUG
        qDebug("Icon not found: %s", sIconName.toUtf8().data());
#endif
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustTreeWidgetItem(QTreeWidgetItem *pTreeWidgetItem, ICONTYPE iconType)
{
    QString sIconName = getIconPath(iconType);

    if (sIconName != "") {
        QIcon icon;
        icon.addFile(sIconName, QSize(), QIcon::Normal, QIcon::Off);
        pTreeWidgetItem->setIcon(0, icon);
    } else {
#ifdef QT_DEBUG
        qDebug("Icon not found: %s", sIconName.toUtf8().data());
#endif
    }
}
#endif
#ifdef QT_GUI_LIB
QString XOptions::getIconPath(ICONTYPE iconType)
{
    static const QMap<ICONTYPE, QString> iconMap = {
        {ICONTYPE_NONE, ""},
        {ICONTYPE_GENERIC, "://icons/BreakpointEnabled.16.16.png"},
        {ICONTYPE_ACTION, "://icons/Action.16.16.png"},
        {ICONTYPE_HEX, "://icons/Binary.16.16.png"},
        {ICONTYPE_DISASM, "://icons/Disasm.16.16.png"},
        {ICONTYPE_ENTROPY, "://icons/Entropy.16.16.png"},
        {ICONTYPE_STRING, "://icons/String.16.16.png"},
        {ICONTYPE_SIGNATURE, "://icons/Signature.16.16.png"},
        {ICONTYPE_SIZE, "://icons/Size.16.16.png"},
        {ICONTYPE_VALUE, "://icons/Value.16.16.png"},
        {ICONTYPE_MEMORYMAP, "://icons/MemoryMap.16.16.png"},
        {ICONTYPE_INFO, "://icons/Info.16.16.png"},
        {ICONTYPE_HASH, "://icons/Hash.16.16.png"},
        {ICONTYPE_VISUALIZATION, "://icons/Image.16.16.png"},
        {ICONTYPE_SEARCH, "://icons/Search.16.16.png"},
        {ICONTYPE_EXTRACTOR, "://icons/Extract.16.16.png"},
        {ICONTYPE_FILE, "://icons/File.16.16.png"},
        {ICONTYPE_SAVE, "://icons/Save.16.16.png"},
        {ICONTYPE_COPY, "://icons/Copy.16.16.png"},
        {ICONTYPE_EDIT, "://icons/Edit.16.16.png"},
        {ICONTYPE_OVERLAY, "://icons/Overlay.16.16.png"},
        {ICONTYPE_RELOAD, "://icons/Refresh.16.16.png"},
        {ICONTYPE_SCAN, "://icons/Refresh.16.16.png"},
        {ICONTYPE_DUMPTOFILE, "://icons/Download.16.16.png"},
        {ICONTYPE_ENTRY, "://icons/Entry.16.16.png"},
        {ICONTYPE_BACKWARD, "://icons/Backward.16.16.png"},
        {ICONTYPE_FORWARD, "://icons/Forward.16.16.png"},
        {ICONTYPE_ADD, "://icons/Add.16.16.png"},
        {ICONTYPE_OPEN, "://icons/Open.16.16.png"},
        {ICONTYPE_LIST, "://icons/List.16.16.png"},
        {ICONTYPE_NEW, "://icons/Add.16.16.png"},
        {ICONTYPE_OPTION, "://icons/Option.16.16.png"},
        {ICONTYPE_YARA, "://icons/Yara.16.16.png"},
        {ICONTYPE_MIME, "://icons/Mime.16.16.png"},
        {ICONTYPE_VIRUSTOTAL, "://icons/Virustotal.16.16.png"},
        {ICONTYPE_TOOL, "://icons/Tool.16.16.png"},
        {ICONTYPE_EXIT, "://icons/Exit.16.16.png"},
        {ICONTYPE_DEMANGLE, "://icons/Demangle.16.16.png"},
        {ICONTYPE_SHORTCUT, "://icons/Shortcut.16.16.png"},
        {ICONTYPE_GOTO, "://icons/Goto.16.16.png"},
        {ICONTYPE_SECTION, "://icons/Section.16.16.png"},
        {ICONTYPE_SEGMENT, "://icons/Segment.16.16.png"},
        {ICONTYPE_EXCEPTION, "://icons/Exception.16.16.png"},
        {ICONTYPE_CERTIFICATE, "://icons/Certificate.16.16.png"},
        {ICONTYPE_RELOC, "://icons/Reloc.16.16.png"},
        {ICONTYPE_DEBUG, "://icons/Debug.16.16.png"},
        {ICONTYPE_HEADER, "://icons/Header.16.16.png"},
        {ICONTYPE_LIBRARY, "://icons/Library.16.16.png"},
        {ICONTYPE_SYMBOL, "://icons/Symbol.16.16.png"},
        {ICONTYPE_TABLE, "://icons/Table.16.16.png"},
        {ICONTYPE_DOTNET, "://icons/DotNet.16.16.png"},
        {ICONTYPE_METADATA, "://icons/Metadata.16.16.png"},
        {ICONTYPE_RESOURCE, "://icons/Resource.16.16.png"},
        {ICONTYPE_TLS, "://icons/TLS.16.16.png"},
        {ICONTYPE_SELECT, "://icons/Select.16.16.png"},
        {ICONTYPE_ADDRESS, "://icons/Address.16.16.png"},
        {ICONTYPE_OFFSET, "://icons/Offset.16.16.png"},
        {ICONTYPE_IMPORT, "://icons/Import.16.16.png"},
        {ICONTYPE_EXPORT, "://icons/Export.16.16.png"},
        {ICONTYPE_DATA, "://icons/Data.16.16.png"},
        {ICONTYPE_DIE, "://icons/DIE.16.16.png"},
        {ICONTYPE_NFD, "://icons/NFD.16.16.png"},
        {ICONTYPE_VERSION, "://icons/Version.16.16.png"},
        {ICONTYPE_MANIFEST, "://icons/Manifest.16.16.png"},
        {ICONTYPE_FOLLOW, "://icons/Follow.16.16.png"},
        {ICONTYPE_NEXT, "://icons/Next.16.16.png"},
        {ICONTYPE_ALL, "://icons/All.16.16.png"},
        {ICONTYPE_PATH, "://icons/Path.16.16.png"},
        {ICONTYPE_NOTE, "://icons/Note.16.16.png"},
        {ICONTYPE_FUNCTION, "://icons/Function.16.16.png"},
        {ICONTYPE_SCRIPT, "://icons/Script.16.16.png"},
        {ICONTYPE_PATCH, "://icons/Patch.16.16.png"},
        {ICONTYPE_REMOVE, "://icons/Remove.16.16.png"},
        {ICONTYPE_RESIZE, "://icons/Resize.16.16.png"},
        {ICONTYPE_CODE, "://icons/Code.16.16.png"},
        {ICONTYPE_REFERENCE, "://icons/Reference.16.16.png"},
        {ICONTYPE_BOOKMARK, "://icons/Bookmark.16.16.png"},
        {ICONTYPE_INSPECTOR, "://icons/Inspector.16.16.png"},
        {ICONTYPE_CONVERTOR, "://icons/Convertor.16.16.png"},
        {ICONTYPE_STRUCTS, "://icons/Structs.16.16.png"}
    };

    QString result = iconMap.value(iconType, "://icons/BreakpointDisabled.16.16.png");

    if (!result.isEmpty() && !QFile::exists(result)) {
        result = "";
    }

    return result;
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustMenu(QMenu *pParentMenu, QMenu *pMenu, const QString &sText, ICONTYPE iconType)
{
    pMenu->setTitle(sText);

    QString sIconPath = XOptions::getIconPath(iconType);

    if (sIconPath != "") {
        QIcon icon;
        icon.addFile(sIconPath, QSize(), QIcon::Normal, QIcon::Off);
        pMenu->setIcon(icon);
    }

    if (pParentMenu) {
        pParentMenu->addMenu(pMenu);
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustAction(QMenu *pParentMenu, QAction *pAction, const QString &sText, const QObject *pRecv, const char *pMethod, ICONTYPE iconType)
{
    connect(pAction, SIGNAL(triggered()), pRecv, pMethod);

    pAction->setText(sText);

    QString sIconPath = XOptions::getIconPath(iconType);

    if (sIconPath != "") {
        QIcon icon;
        icon.addFile(sIconPath, QSize(), QIcon::Normal, QIcon::Off);
        pAction->setIcon(icon);
    }

    if (pParentMenu) {
        pParentMenu->addAction(pAction);
    }
}
#endif

XOptions::BUNDLE XOptions::getBundle()
{
    BUNDLE result = BUNDLE_UNKNOWN;

#ifdef QT_DEBUG
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    QString _sProductType = QSysInfo::productType();
    qDebug("OS: %s", _sProductType.toUtf8().data());
#endif
#endif

#ifdef Q_OS_WIN
#if QT_VERSION <= QT_VERSION_CHECK(5, 6, 3)
    result = BUNDLE_WINDOWS_XP_X86;
#elif (QT_VERSION_MAJOR >= 6)
    // TODO ARM
    result = BUNDLE_WINDOWS_QT6_X64;
#else
#ifndef Q_OS_WIN64
    result = BUNDLE_WINDOWS_X86;
#else
    result = BUNDLE_WINDOWS_X64;
#endif
#endif
#endif

#ifdef Q_PROCESSOR_X86_64
#ifdef Q_OS_LINUX
#if QT_VERSION == QT_VERSION_CHECK(5, 2, 1)
    result = BUNDLE_LINUX_APPIMAGE_X64;
#elif QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    QString sProductType = QSysInfo::productType();

    if (sProductType == "ubuntu") {
        result = BUNDLE_LINUX_UBUNTU_X64;
    }
#endif
#endif
#ifdef Q_OS_FRREBSD
    result = BUNDLE_FREEBSD_X64;
#endif
#ifdef Q_OS_MACOS
    // TODO
    // TODO QSysInfo::currentCpyArchitecture();
    // M
#endif
#endif

    return result;
}
