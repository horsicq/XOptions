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
#ifndef XOPTIONS_H
#define XOPTIONS_H

#include <QDir>
#include <QMap>
#include <QSettings>
#include <QSysInfo>
#include <QTranslator>
#ifdef QT_GUI_LIB
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGroupBox>
#include <QLineEdit>
#include <QListView>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProcess>
#include <QStyleFactory>
#include <QTableView>
#include <QTableWidget>
#include <QTextBrowser>
#include <QTextEdit>
#include <QTreeView>
#include <QTreeWidget>
#include <QWidget>
#else
#include <QCoreApplication>
#endif
#if (QT_VERSION_MAJOR<6)||defined(QT_CORE5COMPAT_LIB)
#include <QTextCodec>
#endif

class XOptions : public QObject
{
    Q_OBJECT

public:
    // TODO Check if update available
    // TODO Show in dialog all available updates
    enum BUNDLE
    {
        BUNDLE_UNKNOWN=0,
        BUNDLE_ARCHLINUX,
        BUNDLE_WINXP,
        BUNDLE_WIN32,
        BUNDLE_WIN64,
        BUNDLE_WIN64_QT6,
        BUNDLE_LINUX_APPIMAGE,
        BUNDLE_DEBIAN,
        BUNDLE_UBUNTU,
        BUNDLE_MACOS,
        BUNDLE_MACOS_QT6_M1
    };

    enum CR
    {
        CR_SUCCESS          =0,
        CR_CANNOTFINDFILE   =1,
        CR_CANNOTOPENFILE   =2
        // TODO more
    };

    enum GROUPID
    {
        GROUPID_UNKNOWN=0,
        GROUPID_VIEW,
        GROUPID_EDIT,
        GROUPID_FILE,
        GROUPID_SCAN,
        GROUPID_SIGNATURES,
        GROUPID_DEBUGGER,
        GROUPID_HEX,
        GROUPID_STACK,
        GROUPID_REGISTERS,
        GROUPID_DISASM,
        GROUPID_IODRIVER,
        GROUPID_STRUCTS,
        GROUPID_ONLINETOOLS
        // TODO GitHub
    };

    enum ID
    {
        ID_UNKNOWN=0,
        ID_ROOTPATH,
        ID_DATAPATH,            // TODO Create group
        ID_JSON,                // TODO Create group
        ID_STRUCTSPATH,
        ID_AUTHUSER,
        ID_AUTHTOKEN,
        // new
        ID_VIEW_STAYONTOP,
        ID_VIEW_STYLE,
        ID_VIEW_QSS,
        ID_VIEW_LANG,
        ID_VIEW_SINGLEAPPLICATION,
        ID_VIEW_SHOWLOGO,
        ID_VIEW_FONT,
        ID_FILE_SAVELASTDIRECTORY,
        ID_FILE_SAVERECENTFILES,
        ID_FILE_SAVEBACKUP,
        ID_FILE_CONTEXT,
        ID_SCAN_SCANAFTEROPEN,
        ID_SCAN_RECURSIVE,
        ID_SCAN_DEEP,
        ID_SCAN_HEURISTIC,
        ID_SCAN_VERBOSE,
        ID_SCAN_ALLTYPES,
        ID_SCAN_ENGINE,
        ID_SCAN_DATABASEPATH,
        ID_SCAN_INFOPATH,
        ID_SCAN_EDITORFONT, // TODO more
        ID_SIGNATURES_PATH,
        ID_ONLINETOOLS_VIRUSTOTAL_APIKEY,
        ID_DISASM_FONT,
        ID_DISASM_SYNTAX,
        ID_DISASM_ADDRESSCOLON,
        ID_DISASM_UPPERCASE,
        ID_DISASM_HIGHLIGHT,
        // TODO more
        ID_DISASM_COLOR_X86_CALL,
        ID_DISASM_COLOR_X86_RET,
        ID_DISASM_COLOR_X86_JCC,
        ID_DISASM_COLOR_X86_PUSH,
        ID_DISASM_COLOR_X86_POP,
        ID_DISASM_COLOR_X86_NOP,
        ID_DISASM_COLOR_X86_JMP,
        ID_DISASM_COLOR_X86_INT3,
        ID_DISASM_COLOR_ARM_BL,
        ID_DISASM_COLOR_ARM_RET,
        ID_DISASM_COLOR_ARM_PUSH,
        ID_DISASM_COLOR_ARM_POP,
        ID_HEX_FONT,
        ID_HEX_ADDRESSCOLON,
        ID_HEX_BLINKINGCURSOR,
        ID_STACK_FONT,
        ID_STACK_ADDRESSCOLON,
        ID_REGISTERS_FONT,
        ID_DEBUGGER_BREAKPOINT_SYSTEM,
        ID_DEBUGGER_BREAKPOINT_ENTRYPOINT,
        ID_DEBUGGER_BREAKPOINT_DLLMAIN,
        ID_DEBUGGER_BREAKPOINT_TLSFUNCTIONS,
        // TODO debugger animate timeout
        // TODO more
        ID_IODRIVER_FILENAME,
        ID_IODRIVER_SERVICENAME,
        ID_STRUCTS_PATH,
        // for internal use.
        ID_NU_LASTDIRECTORY,    // Using if ID_SAVELASTDIRECTORY
        ID_NU_RECENTFILES       // Using if ID_SAVERECENTFILES
    };

    explicit XOptions(QObject *pParent=nullptr);

    void resetToDefault();
    void setValueIDs(QList<ID> listValueIDs);
    void setDefaultValues(QMap<ID,QVariant> mapDefaultValues);
    void addID(ID id,QVariant varDefaultValue=QVariant());
    GROUPID getGroupID(ID id);
    bool isIDPresent(ID id);
    bool isGroupIDPresent(GROUPID groupID);
    void setNative(bool bValue);
    bool isNative();
    static bool isAppImage();
    void setName(QString sValue);
    void load();
    void save();
    QVariant getValue(ID id);
    void setValue(ID id,QVariant vValue);
    void clearValue(ID id);
    static QString idToString(ID id);
    QString getLastDirectory();
    void setLastDirectory(QString sPathName);
    void setLastFileName(QString sFileName);
    QList<QString> getRecentFiles();
    QString getDatabasePath();
    QString getInfoPath();
    QString getScanEngine();
    QString getDisasmSyntax();
    QString getRootPath();
    QString getDataPath();
    QString getJson();
    QString getAuthUser();
    QString getAuthToken();
    QString getVirusTotalApiKey();
    bool isSaveBackup();
    bool isSaveLastDirectory();
    bool isSaveRecentFiles();
    bool isRestartNeeded();
    bool isStayOnTop();
    bool isScanAfterOpen();
    bool isRecursiveScan();
    bool isDeepScan();
    bool isHeuristicScan();
    bool isVerbose();
    bool isAllTypesScan();
    bool isSingleApplication();
    bool isShowLogo();
    QString getSearchSignaturesPath();
    QString getStructsPath();
    QString getApplicationLangPath();
    QString getApplicationQssPath();
    static QList<QString> getAllFilesFromDirectory(QString sDirectory,QString sExtension);
    static bool checkNative();
    QString getApplicationDataPath();
    static QString getTitle(QString sName,QString sVersion,bool bShowOS=true);
#ifdef QT_GUI_LIB
    void setCheckBox(QCheckBox *pCheckBox,ID id);
    void getCheckBox(QCheckBox *pCheckBox,ID id);
    void setCheckBox(QGroupBox *pGroupBox,ID id);
    void getCheckBox(QGroupBox *pGroupBox,ID id);
    void setComboBox(QComboBox *pComboBox,ID id);
    void getComboBox(QComboBox *pComboBox,ID id);
    void setLineEdit(QLineEdit *pLineEdit,ID id);
    void getLineEdit(QLineEdit *pLineEdit,ID id);
    void adjustStayOnTop(QWidget *pWidget);
    void adjustFont(QWidget *pWidget);
    void adjustWindow(QWidget *pWidget);
    void static setMonoFont(QWidget *pWidget,qint32 nSize=-1);
    static void adjustApplicationView(QString sTranslationName,XOptions *pOptions);
//    static void adjustApplicationView(QString sApplicationFileName,QString sTranslationName);
    static QWidget *getMainWidget(QWidget *pWidget);
    static QString getModelText(QAbstractItemModel *pModel);
    static bool saveModel(QAbstractItemModel *pModel,QString sFileName);
    static bool saveTextEdit(QTextEdit *pTextEdit,QString sFileName);
    static bool savePlainTextEdit(QPlainTextEdit *pPlainTextEdit,QString sFileName);
    static bool saveTableView(QTableView *pTableView,QString sFileName);
    static bool saveTableWidget(QTableWidget *pTableWidget,QString sFileName);
    static bool saveListView(QListView *pListView,QString sFileName);
    static bool saveListWidget(QListWidget *pListWidget,QString sFileName);
    static bool saveTreeView(QTreeView *pTreeView,QString sFileName);
    static bool saveTreeWidget(QTreeWidget *pTreeWidget,QString sFileName);
    static bool saveTextBrowser(QTextBrowser *pTextBrowser,QString sFileName);
    static bool saveTextBrowserHtml(QTextBrowser *pTextBrowser,QString sFileName);
    QMenu *createRecentFilesMenu(QWidget *pParent);
    static qint32 getCharWidth(QWidget *pWidget);
    static qint32 getCharHeight(QWidget *pWidget);
    static void showInFolder(QString sFileName);
#endif
#if (QT_VERSION_MAJOR<6)||defined(QT_CORE5COMPAT_LIB)
    static QList<QString> getCodePages(bool bAll);
#ifdef QT_GUI_LIB
    QMenu *createCodePagesMenu(QWidget *pParent,bool bAll);
#endif
#endif
#ifdef Q_OS_WIN
    bool registerContext(QString sApplicationName,QString sType,QString sApplicationFilePath);
    bool clearContext(QString sApplicationName,QString sType);
    bool checkContext(QString sApplicationName,QString sType);
#endif
    void setMaxRecentFilesCount(qint32 nValue);
    qint32 getMaxRecentFilesCount();

    static BUNDLE getBundle();

public slots:
    void clearRecentFiles();

private slots:
    void openRecentFile();
    void setCodePageSlot();

private:
    void _updateRecentFilesMenu();

signals:
    void errorMessage(QString sText);
    void infoMessage(QString sText);
    void openFile(QString sFileName);
    void setCodePage(QString sCodePage);

private:
    static const int N_MAX_RECENT_FILES_COUNT=20; // TODO Set get
    QString g_sName;
    bool g_bIsNative;
    QList<ID> g_listValueIDs;
    QMap<ID,QVariant> g_mapValues;
    QMap<ID,QVariant> g_mapDefaultValues;
    bool g_bIsNeedRestart;
    qint32 g_nMaxRecentFilesCount;
#ifdef QT_GUI_LIB
    QMenu *g_pRecentFilesMenu;
    QMenu *g_pCodePagesMenu;
#endif
};

#endif // XOPTIONS_H
