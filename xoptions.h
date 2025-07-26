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
#ifndef XOPTIONS_H
#define XOPTIONS_H

#include <QDir>
#include <QMap>
#include <QSettings>
#include <QSysInfo>
#include <QTranslator>
#include <QAbstractItemModel>
#if (QT_VERSION_MAJOR < 5)  // TODO Check
#include <QRegExp>
#else
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#endif
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
#include <QColorDialog>
#include <QToolButton>
#include <QHeaderView>
#else
#include <QCoreApplication>
#endif
#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
#include <QTextCodec>
#include "codecs/codec_cp437.h"
#endif
#if defined(Q_OS_FREEBSD)
#include <QStandardPaths>
#endif
#ifdef Q_OS_WIN
#include <Windows.h>
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
    // TODO Linux Mint
    enum BUNDLE {
        BUNDLE_UNKNOWN = 0,
        BUNDLE_LINUX_ARCH_X64,
        BUNDLE_WINDOWS_QT6_X64,
        BUNDLE_LINUX_APPIMAGE_X64,
        BUNDLE_LINUX_DEBIAN_X64,
        BUNDLE_LINUX_UBUNTU_X64,
        BUNDLE_LINUX_UBUNTU_ARM64,
        BUNDLE_LINUX_PARROT_X64,
        BUNDLE_LINUX_KALI_X64,
        BUNDLE_WINDOWS_XP_X86,
        BUNDLE_WINDOWS_X86,
        BUNDLE_WINDOWS_X64,
        BUNDLE_WINDOWS_ARM64,
        BUNDLE_MACOS_X64,
        BUNDLE_MACOS_QT6_ARM64,  // TODO Check
        BUNDLE_FREEBSD_X64
        // TODO WIN ARM64
        // TODO KALI
        // TODO more
    };

    enum WIDGETTYPE {
        WIDGETTYPE_UNKNOWN = 0,
        WIDGETTYPE_HEX,
        WIDGETTYPE_DISASM,
        WIDGETTYPE_MEMORYMAP,
        WIDGETTYPE_CUSTOM
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
        GROUPID_FONTS,
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
        ID_VIEW_FONT_CONTROLS,
        ID_VIEW_FONT_TABLEVIEWS,
        ID_VIEW_FONT_TREEVIEWS,
        ID_VIEW_FONT_TEXTEDITS,
        ID_VIEW_ADVANCED,
        ID_VIEW_SELECTSTYLE,
        ID_FILE_SAVELASTDIRECTORY,
        ID_FILE_SAVERECENTFILES,
        ID_FILE_SAVEBACKUP,
        ID_FILE_CONTEXT,
        ID_SCAN_SCANAFTEROPEN,
        ID_SCAN_FLAG_RECURSIVE,
        ID_SCAN_FLAG_DEEP,
        ID_SCAN_FLAG_HEURISTIC,
        ID_SCAN_FLAG_AGGRESSIVE,
        ID_SCAN_FLAG_VERBOSE,
        ID_SCAN_FLAG_ALLTYPES,
        ID_SCAN_FORMATRESULT,
        ID_SCAN_LOG_PROFILING,
        ID_SCAN_BUFFERSIZE,
        ID_SCAN_HIGHLIGHT,
        ID_SCAN_SORT,
        ID_SCAN_HIDEUNKNOWN,
        ID_SCAN_ENGINE,
        ID_SCAN_ENGINE_EMPTY,
        ID_SCAN_DATABASE_MAIN_PATH,
        ID_SCAN_DATABASE_EXTRA_PATH,
        ID_SCAN_DATABASE_CUSTOM_PATH,
        ID_SCAN_DATABASE_EXTRA_ENABLED,
        ID_SCAN_DATABASE_CUSTOM_ENABLED,
        ID_SCAN_YARARULESPATH,
        ID_SIGNATURES_PATH,
        ID_INFO_PATH,
        ID_ONLINETOOLS_VIRUSTOTAL_APIKEY,
        ID_DISASM_FONT,
        ID_DISASM_SYNTAX,
        ID_DISASM_LOCATIONCOLON,
        ID_DISASM_UPPERCASE,
        ID_DISASM_HIGHLIGHT,
        // TODO more
        ID_DISASM_COLOR_ARROWS,
        ID_DISASM_COLOR_ARROWS_SELECTED,
        ID_DISASM_COLOR_REGS,
        ID_DISASM_COLOR_NUMBERS,
        ID_DISASM_COLOR_OPCODE,
        ID_DISASM_COLOR_REFS,
        ID_DISASM_COLOR_X86_REGS_GENERAL,
        ID_DISASM_COLOR_X86_REGS_STACK,
        ID_DISASM_COLOR_X86_REGS_SEGMENT,
        ID_DISASM_COLOR_X86_REGS_DEBUG,
        ID_DISASM_COLOR_X86_REGS_IP,
        ID_DISASM_COLOR_X86_REGS_FLAGS,
        ID_DISASM_COLOR_X86_REGS_FPU,
        ID_DISASM_COLOR_X86_REGS_XMM,
        ID_DISASM_COLOR_X86_OPCODE_CALL,
        ID_DISASM_COLOR_X86_OPCODE_RET,
        ID_DISASM_COLOR_X86_OPCODE_PUSH,
        ID_DISASM_COLOR_X86_OPCODE_POP,
        ID_DISASM_COLOR_X86_OPCODE_NOP,
        ID_DISASM_COLOR_X86_OPCODE_JMP,
        ID_DISASM_COLOR_X86_OPCODE_COND_JMP,
        ID_DISASM_COLOR_X86_OPCODE_INT3,
        ID_DISASM_COLOR_X86_OPCODE_SYSCALL,
        ID_DISASM_COLOR_ARM_REGS_GENERAL,
        ID_DISASM_COLOR_ARM_REGS_STACK,
        ID_DISASM_COLOR_ARM_OPCODE_B,   // TODO  rename
        ID_DISASM_COLOR_ARM_OPCODE_BL,  // TODO  rename
        ID_DISASM_COLOR_ARM_OPCODE_RET,
        ID_DISASM_COLOR_ARM_OPCODE_PUSH,
        ID_DISASM_COLOR_ARM_OPCODE_POP,
        ID_DISASM_COLOR_ARM_OPCODE_NOP,
        ID_HEX_FONT,
        ID_HEX_LOCATIONCOLON,
        ID_STACK_FONT,
        ID_STACK_LOCATIONCOLON,
        ID_REGISTERS_FONT,
        ID_DEBUGGER_COLOR_BREAKPOINT,
        // TODO debugger animate timeout
        // TODO more
        ID_IODRIVER_FILENAME,
        ID_IODRIVER_SERVICENAME,
        ID_STRUCTS_PATH,
        // for internal use.
        ID_NU_LASTDIRECTORY,  // Using if ID_SAVELASTDIRECTORY
        ID_NU_RECENTFILES,     // Using if ID_SAVERECENTFILES
        ID_FILE_SETENV
    };

    enum ICONTYPE {
        ICONTYPE_NONE = 0,
        ICONTYPE_ACTION,
        ICONTYPE_ADD,
        ICONTYPE_ADDRESS,
        ICONTYPE_ALL,
        ICONTYPE_BACKWARD,
        ICONTYPE_BOOKMARK,
        ICONTYPE_CERTIFICATE,
        ICONTYPE_CODE,
        ICONTYPE_COPY,
        ICONTYPE_DATA,
        ICONTYPE_DEBUG,
        ICONTYPE_DEMANGLE,
        ICONTYPE_DIE,
        ICONTYPE_DISASM,
        ICONTYPE_DOTNET,
        ICONTYPE_DUMPTOFILE,
        ICONTYPE_EDIT,
        ICONTYPE_ENTROPY,
        ICONTYPE_ENTRY,
        ICONTYPE_EXCEPTION,
        ICONTYPE_EXIT,
        ICONTYPE_EXPORT,
        ICONTYPE_EXTRACTOR,
        ICONTYPE_FILE,
        ICONTYPE_FOLLOW,
        ICONTYPE_FORWARD,
        ICONTYPE_FUNCTION,
        ICONTYPE_GENERIC,
        ICONTYPE_GOTO,
        ICONTYPE_HASH,
        ICONTYPE_HEADER,
        ICONTYPE_HEX,
        ICONTYPE_IMPORT,
        ICONTYPE_INFO,
        ICONTYPE_LIBRARY,
        ICONTYPE_LIST,
        ICONTYPE_MANIFEST,
        ICONTYPE_MEMORYMAP,
        ICONTYPE_METADATA,
        ICONTYPE_MIME,
        ICONTYPE_NEW,
        ICONTYPE_NEXT,
        ICONTYPE_NFD,
        ICONTYPE_NOTE,
        ICONTYPE_OFFSET,
        ICONTYPE_OPEN,
        ICONTYPE_OPTION,
        ICONTYPE_OVERLAY,
        ICONTYPE_PATCH,
        ICONTYPE_PATH,
        ICONTYPE_REFERENCE,
        ICONTYPE_RELOAD,
        ICONTYPE_RELOC,
        ICONTYPE_REMOVE,
        ICONTYPE_RESIZE,
        ICONTYPE_RESOURCE,
        ICONTYPE_SAVE,
        ICONTYPE_SCAN,
        ICONTYPE_SCRIPT,
        ICONTYPE_SEARCH,
        ICONTYPE_SECTION,
        ICONTYPE_SEGMENT,
        ICONTYPE_SELECT,
        ICONTYPE_SHORTCUT,
        ICONTYPE_SIGNATURE,
        ICONTYPE_SIZE,
        ICONTYPE_STRING,
        ICONTYPE_STRUCTS,
        ICONTYPE_SYMBOL,
        ICONTYPE_TABLE,
        ICONTYPE_TLS,
        ICONTYPE_TOOL,
        ICONTYPE_VALUE,
        ICONTYPE_VERSION,
        ICONTYPE_VIRUSTOTAL,
        ICONTYPE_VISUALIZATION,
        ICONTYPE_WEBSITE,
        ICONTYPE_YARA,
        ICONTYPE_INSPECTOR,
        ICONTYPE_CONVERTOR,
    };

    struct COLOR_RECORD {
        QString sColorMain;
        QString sColorBackground;
    };

    struct GLOBAL_COLOR_RECORD {
        Qt::GlobalColor colorMain;
        Qt::GlobalColor colorBackground;
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
    void setValue(ID id, QVariant varValue);
    void clearValue(ID id);
    bool isValuePresent(ID id);
    QVariant getDefaultValue(ID id);
    static QString idToString(ID id);
    QString getLastDirectory();
    void setLastDirectory(const QString &sPathName);
    void setLastFileName(const QString &sFileName);
    QList<QString> getRecentFiles();
    QString getDatabasePath();
    QString getExtraDatabasePath();
    QString getCustomDatabasePath();
    QString getScanEngine();
    QString getInfoPath();
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
    bool isStayOnTop();  // TODO remove
    bool isScanAfterOpen();
    bool isSingleApplication();
    bool isShowLogo();  // TODO remove
    QString getSearchSignaturesPath();
    QString getStructsPath();
    QString getApplicationLangPath();
    QString getApplicationQssPath();
    static QList<QString> getAllFilesFromDirectory(const QString &sDirectory, const QString &sExtension);
    static bool checkNative(const QString &sIniFileName);
    QString getApplicationDataPath();
    static QString getTitle(const QString &sName, const QString &sVersion, bool bShowOS = true);
    bool isWritable();
    static void adjustApplicationInitAttributes();
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
    static bool _isAjustStayOnTop(QWidget *pWidget);
    static void _adjustApplicationModal(QWidget *pWidget, bool bState);
    static void _adjustFullScreen(QWidget *pWidget, bool bState);
    // mb TODO max/min
    QFont adjustFont(QWidget *pWidget, ID id = ID_VIEW_FONT);
    void adjustWindow(QWidget *pWidget);
    void adjustWidget(QWidget *pWidget, ID id = ID_VIEW_FONT);
    void adjustTableWidget(QTableWidget *pWidget, ID id = ID_VIEW_FONT_TABLEVIEWS);
    void adjustTableView(QTableView *pWidget, ID id = ID_VIEW_FONT_TABLEVIEWS);
    void adjustTreeWidget(QTreeWidget *pWidget, ID id = ID_VIEW_FONT_TREEVIEWS, bool bAdjustSize = false);
    void adjustTreeView(QTreeView *pWidget, ID id = ID_VIEW_FONT_TREEVIEWS);
    static QFont getDefaultFont(qint32 nFontSize = -1);
    static QFont getMonoFont(qint32 nFontSize = -1);
    static void setMonoFont(QWidget *pWidget, qint32 nSize = -1);
    static void adjustApplicationView(const QString &sTranslationName, XOptions *pOptions);
    static void adjustListWidgetSize(QListWidget *pListWidget, qint32 nMinimumWidth = 100);
    static qint32 _getTreeWidgetItemSize(QTreeWidget *pTreeWidget, QTreeWidgetItem *pTreeWidgetItem, qint32 nIndent, qint32 nLevel);
    static void adjustTreeWidgetSize(QTreeWidget *pTreeWidget, qint32 nMinimumWidth = 100);
    //    static void adjustApplicationView(const QString &sApplicationFileName,QString
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
    static qint32 getControlWidth(QWidget *pWidget, qint32 nSymbolSize);
    static void showInFolder(const QString &sFileName);
    static void showFolder(const QString &sDirectory);
    static void handleFontButton(QWidget *pParent, QLineEdit *pLineEdit);
    static void setModelTextAlignment(QStandardItemModel *pModel, qint32 nColumn, Qt::Alignment flag);
    static void setTableViewHeaderWidth(QTableView *pTableView, qint32 nColumn, qint32 nContentWidth);
    static void setTreeViewHeaderWidth(QTreeView *pTreeView, qint32 nColumn, qint32 nContentWidth);
    static void setTableWidgetHeaderAlignment(QTableWidget *pTableWidget, qint32 nColumn, Qt::Alignment flag);
    static QString getImageFilter();
    static QColor getColorDialog(QWidget *pParent, const QString &sTitle, QColor &color);
    static void adjustToolButton(QToolButton *pToolButton, ICONTYPE iconType, Qt::ToolButtonStyle style = Qt::ToolButtonTextBesideIcon);
    static void adjustTreeWidgetItem(QTreeWidgetItem *pTreeWidgetItem, ICONTYPE iconType);
    static QString getIconPath(ICONTYPE iconType);
    static void adjustMenu(QMenu *pParentMenu, QMenu *pMenu, const QString &sText, ICONTYPE iconType);
    static void adjustAction(QMenu *pParentMenu, QAction *pAction, const QString &sText, const QObject *pRecv, const char *pMethod, ICONTYPE iconType);
    static QColor stringToColor(QString sColor);
    static QString colorToString(const QColor &color);
#endif
    static void deleteQObjectList(QList<QObject *> *pList);
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
    bool isPathInUserEnvironment(const QString &checkPath);
    void appendToUserPathVariable(const QString &newPath);
    void removeFromUserPathVariable(const QString &targetPath);
#endif
    void setMaxRecentFilesCount(qint32 nValue);
    qint32 getMaxRecentFilesCount();
    static BUNDLE getBundle();
    static QString getBundleIdToString(BUNDLE bundle);
    static void registerCodecs();

#ifndef QT_GUI_LIB
    static void printConsole(QString sString, Qt::GlobalColor colorText = Qt::transparent, Qt::GlobalColor colorBackground = Qt::transparent);
    static void printModel(QAbstractItemModel *pModel);
#endif

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
    static const qint32 N_MAX_RECENT_FILES_COUNT = 25;
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
