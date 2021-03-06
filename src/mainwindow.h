#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLibraryInfo>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QStatusBar>
#include <QString>
#include <QStyle>
#include <QTabWidget>
#include <QTextEdit>
#include <QToolBar>
#include <QToolBox>
#include <QToolButton>
#include <QTranslator>
#include <QWidget>

#include "aboutdialog.h"
#include "codecaction.h"
#include "compiler.h"
#include "editor.h"
#include "finddialog.h"
#include "logconsole.h"
#include "recentfileaction.h"
#include "recentfilemanager.h"
#include "replacedialog.h"
#include "settingdialog.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
  public:
    MainWindow();
    
  public slots:
    void slotAbout();
    void slotCloseAllTabs();
    void slotCloseCurrentTab();
    void slotCompileLatex();
    void slotCompilePdflatex();
    void slotCreateRecentFilesMenu();
    void slotFind();
    void slotFindNext();
    void slotFindPrevious();
    void slotNewDocument();
    void slotOpenDocument();
    void slotOpenRecentDocument(QAction *);
    void slotQuit();
    void slotReconnectTab(int);
    void slotReplace();
    void slotSave();
    void slotSaveAll();
    void slotSaveAs();
    void slotSettings();
    
  private slots:
    void slotContentChanged(Editor *);
    void slotChangeCodec(CodecAction *);
    
  private:
    void closeEvent(QCloseEvent *);
    void createCodecMenu();
    void createConnections();
    void createMenus();
    void createStatusbar();
    void createToolbars();
    void createWorkspace();
    void find(int);
    Editor * getCurrentEditor();
    void loadSettings();
    void openDocument(QString);
    
    Compiler *m_compiler;
    FindDialog *m_findDialog;
    int m_newFileCount;
    LogConsole *m_log;
    QAction *m_actionNew, *m_actionOpen, *m_actionRecentlyOpen;
    QAction *m_actionSave, *m_actionSaveAs, *m_actionSaveAll;
    QAction *m_actionClose, *m_actionCloseAll, *m_actionQuit;
    QAction *m_actionUndo, *m_actionRedo;
    QAction *m_actionCut, *m_actionCopy, *m_actionPaste;
    QAction *m_actionFind, *m_actionFindNext, *m_actionFindPrevious, *m_actionReplace;
    QAction *m_actionSettings;
    QAction *m_actionCompileLatex, *m_actionCompilePdflatex;
    QAction *m_actionAbout, *m_actionAboutQt;
    QActionGroup *m_codecGroup;
    QHBoxLayout *m_hboxLayout;
    QLabel *m_cursorPosition;
    QList<Editor *> m_editorList;
    QMenu *m_menuFile, *m_menuEdit, *m_menuRecentlyOpen, *m_menuBuild, *m_menuEncoding, *m_menuAbout;
    QMenuBar *m_menubar;
    QSplitter *m_hSplitter, *m_vSplitter;
    QStatusBar *m_statusbar;
    QTabWidget *m_tabs;
    QToolBar *m_toolbarEdit, *m_toolbarFile, *m_toolbarBuild;
    QToolBox *m_toolbox;
    QToolButton *m_closeButton;
    QWidget *m_centralwidget;
    QWidget *m_page;
    QWidget *m_page2;
    RecentFileManager m_recentFiles;
    ReplaceDialog *m_replaceDialog;
};

#endif
