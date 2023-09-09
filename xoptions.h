/* Copyright (c) 2020-2023 hors<horsicq@gmail.com>
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
#ifndef XOPTIONS_H
#define XOPTIONS_H

#include <QDir>
#include <QMap>
#include <QSettings>
#include <QSysInfo>
#include <QTranslator>
// #include <QVector>
#ifdef QT_GUI_LIB
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFontDialog>
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
#include <QStandardItemModel>
#include <QImageWriter>
#else
#include <QCoreApplication>
#endif
#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
#include <QTextCodec>
#endif
#if defined(Q_OS_FREEBSD)
#include <QStandardPaths>
#endif

class XOptions : public QObject {
    Q_OBJECT

public:
    // TODO Check if update available
    // TODO Show in dialog all available updates
    // Ubuntu -> Ubuntu + Debian + appImage
    // Debian -> Debian + appImage
    // Parrot -> Parrot + Debian + appImage
    // MX Linux -> MX Linux + Debian + appImage
    // Linux X64 -> appImage
    // Windows 64 -> Windows 32 + Windows 64 + Windows XP
    // Windows 32 -> Windows 32 + Windows XP
    // Windows XP -> Windows XP
    // MacOS M1 -> MacOS M1 + MacOS X64
    // MacOS X64 -> MacOS X64
    // TODO Linux ARM types 32/64, old ARM? Check
    // TODO Check other BSD
    // TODO Check Raspberry PI
    // TODO Check Windows ARM
    // TODO Linux FEDORA
    enum BUNDLE {
        BUNDLE_UNKNOWN = 0,
        BUNDLE_LINUX_ARCH_X64,
        BUNDLE_WINDOWS_QT6_X64,
        BUNDLE_LINUX_APPIMAGE_X64,
        BUNDLE_LINUX_DEBIAN_X64,
        BUNDLE_LINUX_UBUNTU_X64,
        BUNDLE_LINUX_PARROT_X64,
        BUNDLE_WINDOWS_XP_X86,
        BUNDLE_WINDOWS_X86,
        BUNDLE_WINDOWS_X64,
        BUNDLE_MACOS_X64,
        BUNDLE_MACOS_QT6_ARM64,  // TODO Check
        BUNDLE_FREEBSD_X64
        // TODO WIN ARM64
        // TODO KALI
        // TODO more
    };

    enum CR {
        CR_SUCCESS = 0,
        CR_CANNOTFINDFILE = 1,
        CR_CANNOTOPENFILE = 2,
        CR_CANNOTFINDDATABASE = 3,
        // TODO more
    };

    enum GROUPID {
        GROUPID_UNKNOWN = 0,
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
        GROUPID_ONLINETOOLS,
        GROUPID_INFO
        // TODO GitHub
    };

    enum ID {
        ID_UNKNOWN = 0,
        ID_ROOTPATH,
        ID_DATAPATH,  // TODO Create group
        ID_JSON,      // TODO Create group
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
        ID_VIEW_ADVANCED,
        ID_VIEW_SELECTSTYLE,
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
        ID_SCAN_YARARULESPATH,
        ID_SCAN_EDITORFONT,  // TODO more
        ID_SIGNATURES_PATH,
        ID_INFO_PATH,
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
        ID_DISASM_COLOR_ARM_NOP,
        ID_HEX_FONT,
        ID_HEX_ADDRESSCOLON,
        //        ID_HEX_BLINKINGCURSOR,
        ID_STACK_FONT,
        ID_STACK_ADDRESSCOLON,
        ID_REGISTERS_FONT,
        ID_DEBUGGER_BREAKPOINT_SYSTEM,        // TODO remove
        ID_DEBUGGER_BREAKPOINT_ENTRYPOINT,    // TODO remove
        ID_DEBUGGER_BREAKPOINT_DLLMAIN,       // TODO remove
        ID_DEBUGGER_BREAKPOINT_TLSFUNCTIONS,  // TODO remove
        // TODO debugger animate timeout
        // TODO more
        ID_IODRIVER_FILENAME,
        ID_IODRIVER_SERVICENAME,
        ID_STRUCTS_PATH,
        // for internal use.
        ID_NU_LASTDIRECTORY,  // Using if ID_SAVELASTDIRECTORY
        ID_NU_RECENTFILES     // Using if ID_SAVERECENTFILES
    };

    explicit XOptions(QObject *pParent = nullptr);

    void resetToDefault();
    void setValueIDs(const QList<ID> &listValueIDs);
    void setDefaultValues(QMap<ID, QVariant> mapDefaultValues);
    void addID(ID id, QVariant varDefaultValue = QVariant());
    void removeID(ID id);
    GROUPID getGroupID(ID id);
    bool isIDPresent(ID id);
    bool isGroupIDPresent(GROUPID groupID);
    bool isNative();
    static bool isAppImage();
    void setName(const QString &sValue);
    void load();
    void save();
    QVariant getValue(ID id);
    void setValue(ID id, QVariant vValue);
    void clearValue(ID id);
    QVariant getDefaultValue(ID id);
    static QString idToString(ID id);
    QString getLastDirectory();
    void setLastDirectory(const QString &sPathName);
    void setLastFileName(const QString &sFileName);
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
    bool isVerboseScan();
    bool isAllTypesScan();
    bool isSingleApplication();
    bool isShowLogo();
    QString getSearchSignaturesPath();
    QString getStructsPath();
    QString getApplicationLangPath();
    QString getApplicationQssPath();
    static QList<QString> getAllFilesFromDirectory(const QString &sDirectory, const QString &sExtension);
    static bool checkNative(const QString &sIniFileName);
    QString getApplicationDataPath();
    static QString getTitle(const QString &sName, const QString &sVersion, bool bShowOS = true);
    bool isWritable();
#ifdef QT_GUI_LIB
    void setCheckBox(QCheckBox *pCheckBox, ID id);
    void getCheckBox(QCheckBox *pCheckBox, ID id);
    void setCheckBox(QGroupBox *pGroupBox, ID id);
    void getCheckBox(QGroupBox *pGroupBox, ID id);
    void setComboBox(QComboBox *pComboBox, ID id);
    void getComboBox(QComboBox *pComboBox, ID id);
    void setLineEdit(QLineEdit *pLineEdit, ID id);
    void getLineEdit(QLineEdit *pLineEdit, ID id);
    void adjustStayOnTop(QWidget *pWidget);
    static void _adjustStayOnTop(QWidget *pWidget, bool bState);
    static void _adjustApplicationModal(QWidget *pWidget, bool bState);
    static void _adjustFullScreen(QWidget *pWidget, bool bState);
    // mb TODO max/min
    void adjustFont(QWidget *pWidget);
    void adjustWindow(QWidget *pWidget);
    static void setMonoFont(QWidget *pWidget, qint32 nSize = -1);
    static void adjustApplicationView(const QString &sTranslationName, XOptions *pOptions);
    //    static void adjustApplicationView(QString sApplicationFileName,QString
    //    sTranslationName);
    static QWidget *getMainWidget(QWidget *pWidget);
    static QString getTableModelText(QAbstractItemModel *pModel);
    static void _getTreeModelText(QString *psString, QAbstractItemModel *pModel, QModelIndex index, qint32 nLevel);
    static QString getTreeModelText(QAbstractItemModel *pModel);
    static bool saveTableModel(QAbstractItemModel *pModel, const QString &sFileName);
    static bool saveTreeModel(QAbstractItemModel *pModel, const QString &sFileName);
    static bool saveTextEdit(QTextEdit *pTextEdit, const QString &sFileName);
    static bool savePlainTextEdit(QPlainTextEdit *pPlainTextEdit, const QString &sFileName);
    static bool saveTableView(QTableView *pTableView, const QString &sFileName);
    static bool saveTableWidget(QTableWidget *pTableWidget, const QString &sFileName);
    static bool saveListView(QListView *pListView, const QString &sFileName);
    static bool saveListWidget(QListWidget *pListWidget, const QString &sFileName);
    static bool saveTreeView(QTreeView *pTreeView, const QString &sFileName);
    static bool saveTreeWidget(QTreeWidget *pTreeWidget, const QString &sFileName);
    static bool saveTextBrowser(QTextBrowser *pTextBrowser, const QString &sFileName);
    static bool saveTextBrowserHtml(QTextBrowser *pTextBrowser, const QString &sFileName);
    QMenu *createRecentFilesMenu(QWidget *pParent);
    static qint32 getCharWidth(QWidget *pWidget);
    static qint32 getCharHeight(QWidget *pWidget);
    static void showInFolder(const QString &sFileName);
    static void showFolder(const QString &sDirectory);
    static void handleFontButton(QWidget *pParent, QLineEdit *pLineEdit);
    static void setModelTextAlignment(QStandardItemModel *pModel, qint32 nColumn, Qt::Alignment flag);
    static void setTableViewHeaderWidth(QTableView *pTableView, qint32 nColumn, qint32 nContentWidth);
    static void setTreeViewHeaderWidth(QTreeView *pTreeView, qint32 nColumn, qint32 nContentWidth);
    static void setTableWidgetHeaderAlignment(QTableWidget *pTableWidget, qint32 nColumn, Qt::Alignment flag);
    static QString getImageFilter();
#endif
#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
    static QList<QString> getCodePages(bool bAll);
#ifdef QT_GUI_LIB
    QMenu *createCodePagesMenu(QWidget *pParent, bool bAll);
#endif
#endif
#ifdef Q_OS_WIN
    enum USERROLE {
        USERROLE_NORMAL = 0,
        USERROLE_ADMIN
    };
    QString getClassesPrefix(USERROLE userRole);
    bool registerContext(const QString &sApplicationName, const QString &sType, const QString &sApplicationFilePath, USERROLE userRole = USERROLE_ADMIN);
    bool clearContext(const QString &sApplicationName, const QString &sType, USERROLE userRole = USERROLE_ADMIN);
    bool checkContext(const QString &sApplicationName, const QString &sType, USERROLE userRole = USERROLE_ADMIN);
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
    void errorMessage(const QString &sText);
    void infoMessage(const QString &sText);
    void openFile(const QString &sFileName);
    void setCodePage(const QString &sCodePage);

private:
    static const qint32 N_MAX_RECENT_FILES_COUNT = 20;  // TODO Set get
    QString g_sName;
    QList<ID> g_listValueIDs;
    QMap<ID, QVariant> g_mapValues;
    QMap<ID, QVariant> g_mapDefaultValues;
    bool g_bIsNeedRestart;
    qint32 g_nMaxRecentFilesCount;
#ifdef QT_GUI_LIB
    QMenu *g_pRecentFilesMenu;
    QMenu *g_pCodePagesMenu;
#endif
};

#endif  // XOPTIONS_H
