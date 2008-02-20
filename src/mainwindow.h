#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtCore/QVariant>
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
    void openDocument();
    void newDocument();
    void quit();
    void reconnectTab(int);
    void save();
    void saveAll();
    void saveAs();
    
  private:
    void createConnections();
    void createMenus();
    void createStatusbar();
    void createToolbar();
    void createWorkspace();
    Editor * getCurrentEditor();
    
    Compiler *compiler;
    int newFileCount;    
    QAction *action_Neu; 
    QAction *action_Oeffnen;
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
    QAction *action_kompiliereLatex;
    QAction *action_kompilierePdflatex;
    QHBoxLayout *hboxLayout;
    QLabel *cursorPosition;
    QList<Editor *> editorList;
    QMenu *menu_Datei;
    QMenu *menu_Bearbeiten;
    QMenu *menu_Erstellen;
    QMenuBar *menubar;
    QSplitter *vSplitter;
    QSplitter *hSplitter;
    QStatusBar *statusbar;
    QTextEdit *output;
    QTabWidget *tabs;
    QToolBar *toolbar;
    QToolBox *toolbox;
    QToolButton *closeButton;
    QWidget *centralwidget;
    QWidget *page;
    QWidget *page_2;
};

#endif
