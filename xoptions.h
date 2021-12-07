/* Copyright (c) 2020-2021 hors<horsicq@gmail.com>
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
#include <QFileDialog>
#include <QLineEdit>
#include <QListView>
#include <QListWidget>
#include <QMessageBox>
#include <QPlainTextEdit>
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

class XOptions : public QObject
{
    Q_OBJECT

    const qint32 N_MAX_RECENTFILES=10; // TODO Check

public:
    enum ID
    {
        ID_STAYONTOP=0,
        ID_SCANAFTEROPEN,
        ID_RECURSIVESCAN,
        ID_DEEPSCAN,
        ID_HEURISTICSCAN,
        ID_SAVELASTDIRECTORY,
        ID_SAVERECENTFILES,
        ID_SAVEBACKUP,
        ID_STYLE,
        ID_QSS,
        ID_LANG,
        ID_DATABASEPATH,
        ID_INFOPATH,
        ID_SCANENGINE,
        ID_DISASMSYNTAX,
        ID_ROOTPATH,
        ID_DATAPATH,
        ID_JSON,
        ID_SINGLEAPPLICATION,
        ID_SEARCHSIGNATURESPATH,
        ID_STRUCTSPATH,
        ID_AUTHUSER,
        ID_AUTHTOKEN,
        ID_SHOWLOGO,
        ID_BREAKPOINT_ENTRYPOINT,
        ID_BREAKPOINT_DLLMAIN,
        ID_BREAKPOINT_TLSFUNCTIONS,
        // TODO Breakpoints on systen breakpoints
        // for internal use.
        ID_NU_LASTDIRECTORY,    // Using if ID_SAVELASTDIRECTORY
        ID_NU_RECENTFILES       // Using if ID_SAVERECENTFILES
    };

    explicit XOptions(QObject *pParent=nullptr);
    void setValueIDs(QList<ID> listValueIDs);
    void setDefaultValues(QMap<ID,QVariant> mapDefaultValues);
    void addID(ID id, QVariant varDefaultValue=QVariant());
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
    void clearRecentFiles();
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
    bool isSaveBackup();
    bool isSaveLastDirectory();
    bool isSaveRecentFiles();
    bool isRestartNeeded();
    bool isStayOnTop();
    bool isScanAfterOpen();
    bool isRecursiveScan();
    bool isDeepScan();
    bool isHeuristicScan();
    bool isSingleApplication();
    bool isShowLogo();
    QString getSearchSignaturesPath();
    QString getStructsPath();
    QString getApplicationLangPath();
    QString getApplicationQssPath();
    static QList<QString> getAllFilesFromDirectory(QString sDirectory,QString sExtension);
    static bool checkNative();
    QString getApplicationDataPath();
    static QString getTitle(QString sName,QString sVersion);
#ifdef QT_GUI_LIB
    void setCheckBox(QCheckBox *pCheckBox,ID id);
    void getCheckBox(QCheckBox *pCheckBox,ID id);
    void setComboBox(QComboBox *pComboBox,ID id);
    void getComboBox(QComboBox *pComboBox,ID id);
    void setLineEdit(QLineEdit *pLineEdit,ID id);
    void getLineEdit(QLineEdit *pLineEdit,ID id);
    void adjustStayOnTop(QWidget *pWidget);
    void static setMonoFont(QWidget *pWidget,qint32 nSize=-1);
    static void adjustApplicationView(QString sTranslationName,XOptions *pOptions);
    static void adjustApplicationView(QString sApplicationFileName,QString sTranslationName);
    static QWidget *getMainWidget(QWidget *pWidget);
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
#endif
#ifdef Q_OS_WIN
    bool registerContext(QString sApplicationName,QString sType,QString sApplicationFilePath);
    bool clearContext(QString sApplicationName,QString sType);
    bool checkContext(QString sApplicationName,QString sType);
#endif

signals:
    void errorMessage(QString sErrorMessage);

private:
    QString g_sName;
    bool g_bIsNative;
    QList<ID> g_listValueIDs;
    QMap<ID,QVariant> g_mapValues;
    QMap<ID,QVariant> g_mapDefaultValues;
    bool g_bIsNeedRestart;
};

#endif // XOPTIONS_H
