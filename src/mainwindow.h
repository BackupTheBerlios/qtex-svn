#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtCore/QVariant>
#include <QtGui/QCloseEvent>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QToolBox>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

#include "src/compiler.h"
#include "src/editor.h"
#include "src/recentfileaction.h"
#include "src/recentfilemanager.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
  public:
    MainWindow();
    
  public slots:
    void closeAllTabs();
    void closeCurrentTab();
    void compileLatex();
    void compilePdflatex();
    void createRecentFilesMenu();
    void openDocument();
    void openRecentDocument(QAction *);
    void newDocument();
    void quit();
    void reconnectTab(int);
    void save();
    void saveAll();
    void saveAs();
    void settings();
    
  private:
    void closeEvent(QCloseEvent *);
    void createConnections();
    void createMenus();
    void createStatusbar();
    void createToolbars();
    void createWorkspace();
    Editor * getCurrentEditor();
    void loadSettings();
    void openDocument(QString);
    
    Compiler *compiler;
    int newFileCount;
    RecentFileManager recentFiles;
    //RecentFileAction **action_letzteDatei;
    QAction *action_Neu; 
    QAction *action_Oeffnen;
    QAction *action_zuletztOffen;
    QAction *action_Speichern;
    QAction *action_SpeichernUnter;
    QAction *action_AlleSpeichern;
    QAction *action_Schliessen;
    QAction *action_AlleSchliessen;
    QAction *action_Beenden;
    QAction *action_Rueckgaengig;
    QAction *action_Wiederherstellen;
    QAction *action_Ausschneiden;
    QAction *action_Kopieren;
    QAction *action_Einfuegen;
    QAction *action_Einstellungen;
    QAction *action_kompiliereLatex;
    QAction *action_kompilierePdflatex;
    QHBoxLayout *hboxLayout;
    QLabel *cursorPosition;
    QList<Editor *> editorList;
    //QList<RecentFileAction *> action_letzteDatei;
    //QList<QString> recentFiles;
    QMenu *menu_Datei;
    QMenu *menu_Bearbeiten;
    QMenu *menu_ZuletztOffen;
    QMenu *menu_Erstellen;
    QMenuBar *menubar;
    QSplitter *vSplitter;
    QSplitter *hSplitter;
    QStatusBar *statusbar;
    QTextEdit *output;
    QTabWidget *tabs;
    QToolBar *toolbarBearbeiten;
    QToolBar *toolbarDatei;
    QToolBar *toolbarKompilierwerkzeuge;
    QToolBox *toolbox;
    QToolButton *closeButton;
    QWidget *centralwidget;
    QWidget *page;
    QWidget *page_2;
};

#endif
