#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFlags>
#include <QMessageBox>
#include <QStyle>

#include "mainwindow.h"
#include "settingdialog.h"
#include <stdio.h>

#define APP_TITLE "QteX"
#define BUF_SIZE 1024

MainWindow::MainWindow() : QMainWindow() {
  this->setWindowTitle(trUtf8(APP_TITLE));
  
  createMenus();
  createStatusbar();
  createToolbars();
  createWorkspace();
  createConnections();

  compiler = new Compiler(output);
  compiler->checkEnvironment();
  
  findDialog = new FindDialog(this);
  replaceDialog = new ReplaceDialog(this);
  
  newFileCount = 0;
}

/*
 * Alle Tabs schließen. Sobald für ein Tab in einem
 * eventuellen Speichervorgang 'Abbrechen' gewählt
 * wird, werden keine weiteren Tabs geschlossen.
 */ 
void MainWindow::closeAllTabs() {
  int tabCount = tabs->count();
  while (tabCount > 0) {
    int oldTabCount = tabs->count();
    closeCurrentTab();
    if (tabs->count() == oldTabCount) {
      break;
    }
    tabCount--;
  }
  
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  curInput->setFocus();
}

/*
 * Das aktuelle Tab schließen. Dazu ggf. das
 * vorhandene Dokument speichern lassen, vgl.
 * dazu Editor::maybeSave()
 */
void MainWindow::closeCurrentTab() {
  int curIndex = tabs->currentIndex();
  if (curIndex >= 0) {
    Editor *curInput = editorList.at(curIndex);
    if (curInput == 0) {
      tabs->removeTab(tabs->currentIndex());
      return;
    }
    
    if (curInput->maybeSave() == true) {
      editorList.removeAt(curIndex);
      tabs->removeTab(curIndex);
    }
  }
  
  /* Alle Tabs geschlossen */
  if (tabs->count() == 0) {
    action_Speichern->setEnabled(false);
    action_SpeichernUnter->setEnabled(false);
    action_AlleSpeichern->setEnabled(false);
    action_Schliessen->setEnabled(false);
    action_AlleSchliessen->setEnabled(false);
    action_Rueckgaengig->setEnabled(false);
    action_Wiederherstellen->setEnabled(false);
    action_Ausschneiden->setEnabled(false);
    action_Kopieren->setEnabled(false);
    action_Einfuegen->setEnabled(false);
  } else {
    Editor *curInput = getCurrentEditor();
    if (curInput == 0) {
      return;
    }
    
    curInput->setFocus();
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  closeAllTabs();
  if (tabs->count() > 0) {
    event->ignore();
    Editor *curInput = getCurrentEditor();
    if (curInput == 0) {
      return;
    }
    
    curInput->setFocus();
  } else {
    event->accept();
  }
}

/*
 * Das aktuelle Dokument mit latex kompilieren lassen.
 * Dazu MUSS das Dokument zunächst gespeichert werden.
 */
void MainWindow::compileLatex() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  /* Das Dokument MUSS vor dem Kompilieren gespeichert werden */
  if (curInput->save() == true) {
    compiler->compileLatex(curInput->getFilename());
  }
}

/*
 * Das aktuelle Dokument mit pdflatex kompilieren lassen.
 * Dazu MUSS das Dokument zunächst gespeichert werden.
 */
void MainWindow::compilePdflatex() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  /* Das Dokument MUSS vor dem Kompilieren gespeichert werden */
  if (curInput->save() == true) {
    compiler->compilePdflatex(curInput->getFilename());
  }
}

/*
 * Bestimmte unveränderliche Verbindungen erzeugen.
 */
void MainWindow::createConnections() {
  QObject::connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(reconnectTab(int)));
  
  /* Menusignale einrichten */
  QObject::connect(action_Neu, SIGNAL(triggered()), this, SLOT(newDocument()));
  QObject::connect(action_Oeffnen, SIGNAL(triggered()), this, SLOT(openDocument()));
  QObject::connect(action_Speichern, SIGNAL(triggered()), this, SLOT(save()));
  QObject::connect(action_SpeichernUnter, SIGNAL(triggered()), this, SLOT(saveAs()));
  QObject::connect(action_AlleSpeichern, SIGNAL(triggered()), this, SLOT(saveAll()));
  QObject::connect(action_Schliessen, SIGNAL(triggered()), this, SLOT(closeCurrentTab()));
  QObject::connect(action_AlleSchliessen, SIGNAL(triggered()), this, SLOT(closeAllTabs()));
  QObject::connect(action_Beenden, SIGNAL(triggered()), this, SLOT(quit()));
  
  QObject::connect(action_Suchen, SIGNAL(triggered()), this, SLOT(find()));
  QObject::connect(action_NaechsteSuchen, SIGNAL(triggered()), this, SLOT(findNext()));
  QObject::connect(action_LetzteSuchen, SIGNAL(triggered()), this, SLOT(findPrevious()));
  QObject::connect(action_Ersetzen, SIGNAL(triggered()), this, SLOT(replace()));
  QObject::connect(action_Einstellungen, SIGNAL(triggered()), this, SLOT(settings()));
  
  QObject::connect(action_kompiliereLatex, SIGNAL(triggered()), this, SLOT(compileLatex()));
  QObject::connect(action_kompilierePdflatex, SIGNAL(triggered()), this, SLOT(compilePdflatex()));

  /* Weitere Signale einrichten */
  QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(closeCurrentTab()));
  QObject::connect(&recentFiles, SIGNAL(update()), this, SLOT(createRecentFilesMenu()));
}

/*
 * Die Menustruktur erzeugen.
 */
void MainWindow::createMenus() {
  /* Menuleiste erzeugen */
  menubar = new QMenuBar(this);
  menubar->setObjectName(QString("menubar"));
  this->setMenuBar(menubar);
  
  /* Menu 'Datei' einrichten */
  menu_Datei = new QMenu(menubar);
  menu_Datei->setObjectName(QString("menu_Datei"));
  menu_Datei->setTitle(trUtf8("&Datei"));
  
  action_Neu = new QAction(this);
  action_Neu->setIcon(QIcon(":/images/filenew.png"));
  action_Neu->setObjectName(QString("action_Neu"));
  action_Neu->setShortcut(tr("Ctrl+N"));
  action_Neu->setText(trUtf8("&Neu"));
  
  action_Oeffnen = new QAction(this);
  action_Oeffnen->setIcon(QIcon(":/images/fileopen.png"));
  action_Oeffnen->setObjectName(QString("action_Oeffnen"));
  action_Oeffnen->setShortcut(tr("Ctrl+O"));
  action_Oeffnen->setText(trUtf8("Ö&ffnen..."));
  
  action_zuletztOffen = new QAction(this);
  action_zuletztOffen->setObjectName(QString("action_zuetztOffen"));
  action_zuletztOffen->setText(trUtf8("&Zuletzt geöffnet"));
  
  // Untermenü für die letzten Dateien
  menu_ZuletztOffen = new QMenu(menubar);
  action_zuletztOffen->setMenu(menu_ZuletztOffen);
  createRecentFilesMenu();
  
  action_Speichern = new QAction(this);
  action_Speichern->setEnabled(false);
  action_Speichern->setIcon(QIcon(":/images/filesave.png"));
  action_Speichern->setObjectName(QString("action_Speichern"));
  action_Speichern->setShortcut(tr("Ctrl+S"));
  action_Speichern->setText(trUtf8("&Speichern"));
  
  action_SpeichernUnter = new QAction(this);
  action_SpeichernUnter->setEnabled(false);
  action_SpeichernUnter->setIcon(QIcon(":/images/filesaveas.png"));
  action_SpeichernUnter->setObjectName(QString("action_SpeichernUnter"));
  action_SpeichernUnter->setText(trUtf8("Speichern &unter..."));
  
  action_AlleSpeichern = new QAction(this);
  action_AlleSpeichern->setEnabled(false);
  action_AlleSpeichern->setIcon(QIcon(":/images/filesaveall.png"));
  action_AlleSpeichern->setObjectName(QString("action_AlleSpeichern"));
  action_AlleSpeichern->setText(trUtf8("&Alle speichern"));
  
  action_Schliessen = new QAction(this);
  action_Schliessen->setEnabled(false);
  action_Schliessen->setIcon(QIcon(":/images/fileclose.png"));
  action_Schliessen->setObjectName(QString("action_Schliessen"));
  action_Schliessen->setText(trUtf8("Schl&ießen"));
  
  action_AlleSchliessen = new QAction(this);
  action_AlleSchliessen->setEnabled(false);
  action_AlleSchliessen->setObjectName(QString("action_AlleSchliessen"));
  action_AlleSchliessen->setText(trUtf8("Alle schließen"));
  
  action_Beenden = new QAction(this);
  action_Beenden->setObjectName(QString("action_Beenden"));
  action_Beenden->setShortcut(tr("Ctrl+B"));
  action_Beenden->setText(trUtf8("&Beenden"));
  
  menu_Datei->addAction(action_Neu);
  menu_Datei->addAction(action_Oeffnen);
  menu_Datei->addAction(action_zuletztOffen);
  menu_Datei->addSeparator();
  menu_Datei->addAction(action_Speichern);
  menu_Datei->addAction(action_SpeichernUnter);
  menu_Datei->addSeparator();
  menu_Datei->addAction(action_AlleSpeichern);
  menu_Datei->addAction(action_Schliessen);
  menu_Datei->addAction(action_AlleSchliessen);
  menu_Datei->addSeparator();
  menu_Datei->addAction(action_Beenden);
  
  /* Menu 'Bearbeiten' einrichten */
  menu_Bearbeiten = new QMenu(menubar);
  menu_Bearbeiten->setObjectName(QString("menu_Bearbeiten"));
  menu_Bearbeiten->setTitle(trUtf8("&Bearbeiten"));
  
  action_Rueckgaengig = new QAction(this);
  action_Rueckgaengig->setEnabled(false);
  action_Rueckgaengig->setIcon(QIcon(":/images/undo.png"));
  action_Rueckgaengig->setObjectName(QString("action_Rueckgaengig"));
  action_Rueckgaengig->setShortcut(tr("Ctrl+Z"));
  action_Rueckgaengig->setText(trUtf8("&Rückgängig"));
  
  action_Wiederherstellen = new QAction(this);
  action_Wiederherstellen->setEnabled(false);
  action_Wiederherstellen->setIcon(QIcon(":/images/redo.png"));
  action_Wiederherstellen->setObjectName(QString("action_Wiederherstellen"));
  action_Wiederherstellen->setShortcut(tr("Ctrl+Shift+Z"));
  action_Wiederherstellen->setText(trUtf8("&Wiederherstellen"));
  
  action_Ausschneiden = new QAction(this);
  action_Ausschneiden->setIcon(QIcon(":/images/editcut.png"));
  action_Ausschneiden->setObjectName(QString("action_Ausschneiden"));
  action_Ausschneiden->setShortcut(tr("Ctrl+X"));
  action_Ausschneiden->setText(trUtf8("&Ausschneiden"));
  
  action_Kopieren = new QAction(this);
  action_Kopieren->setIcon(QIcon(":/images/editcopy.png"));
  action_Kopieren->setObjectName(QString("action_Kopieren"));
  action_Kopieren->setShortcut(tr("Ctrl+C"));
  action_Kopieren->setText(trUtf8("&Kopieren"));
  
  action_Einfuegen = new QAction(this);
  action_Einfuegen->setIcon(QIcon(":/images/editpaste.png"));
  action_Einfuegen->setObjectName(QString("action_Einfuegen"));
  action_Einfuegen->setShortcut(tr("Ctrl+V"));
  action_Einfuegen->setText(trUtf8("&Einfügen"));
  
  action_Suchen = new QAction(this);
  action_Suchen->setIcon(QIcon(":/images/find.png"));
  action_Suchen->setObjectName(QString("action_Suchen"));
  action_Suchen->setShortcut(tr("Ctrl+F"));
  action_Suchen->setText(trUtf8("&Suchen..."));
  
  action_NaechsteSuchen = new QAction(this);
  action_NaechsteSuchen->setObjectName(QString("action_NaechsteSuchen"));
  action_NaechsteSuchen->setShortcut(tr("F3"));
  action_NaechsteSuchen->setText(trUtf8("&Weitersuchen"));
  
  action_LetzteSuchen = new QAction(this);
  action_LetzteSuchen->setObjectName(QString("action_LetzteSuchen"));
  action_LetzteSuchen->setShortcut(tr("Shift+F3"));
  action_LetzteSuchen->setText(trUtf8("&Frühere suchen"));
  
  action_Ersetzen = new QAction(this);
  action_Ersetzen->setObjectName(QString("action_Ersetzen"));
  action_Ersetzen->setShortcut(tr("Ctrl+R"));
  action_Ersetzen->setText(tr("&Ersetzen"));
  
  action_Einstellungen = new QAction(this);
  action_Einstellungen->setObjectName(QString("action_Einstellungen"));
  action_Einstellungen->setText(trUtf8("Ei&nstellungen"));
  
  menu_Bearbeiten->addAction(action_Rueckgaengig);
  menu_Bearbeiten->addAction(action_Wiederherstellen);
  menu_Bearbeiten->addSeparator();
  menu_Bearbeiten->addAction(action_Ausschneiden);
  menu_Bearbeiten->addAction(action_Kopieren);
  menu_Bearbeiten->addAction(action_Einfuegen);
  menu_Bearbeiten->addSeparator();
  menu_Bearbeiten->addAction(action_Suchen);
  menu_Bearbeiten->addAction(action_NaechsteSuchen);
  menu_Bearbeiten->addAction(action_LetzteSuchen);
  menu_Bearbeiten->addAction(action_Ersetzen);
  menu_Bearbeiten->addSeparator();
  menu_Bearbeiten->addAction(action_Einstellungen);
  
  /* Menu 'Erstellen' einrichten */
  menu_Erstellen = new QMenu(menubar);
  menu_Erstellen->setObjectName(QString("menu_Erstellen"));
  menu_Erstellen->setTitle(trUtf8("Erstellen"));
  
  action_kompiliereLatex = new QAction(this);
  action_kompiliereLatex->setIcon(QIcon(":/images/latex.png"));
  action_kompiliereLatex->setObjectName(QString("action_kompiliereLatex"));
  action_kompiliereLatex->setShortcut(tr("Alt+1"));
  action_kompiliereLatex->setText(trUtf8("&Latex"));
  
  action_kompilierePdflatex = new QAction(this);
  action_kompilierePdflatex->setIcon(QIcon(":/images/pdflatex.png"));
  action_kompilierePdflatex->setObjectName(QString("action_kompilierePdflatex"));
  action_kompilierePdflatex->setShortcut(tr("Alt+2"));
  action_kompilierePdflatex->setText(trUtf8("&PdfLatex"));
  
  menu_Erstellen->addAction(action_kompiliereLatex);
  menu_Erstellen->addAction(action_kompilierePdflatex);
  
  /* Untermenus einfuegen */
  menubar->addAction(menu_Datei->menuAction());
  menubar->addAction(menu_Bearbeiten->menuAction());
  menubar->addAction(menu_Erstellen->menuAction());
}

void MainWindow::createRecentFilesMenu() {
  QList<QAction*> tmp = menu_ZuletztOffen->actions();
  for (int i = 0; i < tmp.size(); i++) {
    QAction *a = tmp.at(i);
    if (a == 0) {
      continue;
    }
    
    menu_ZuletztOffen->removeAction(a);
  }
  
  QList<RecentFileAction*> actions = recentFiles.getActions();
  for (int i = 0; i < actions.size(); i++) {
    RecentFileAction *act = actions.at(i);
    if (act == 0) {
      continue;
    }
    
    menu_ZuletztOffen->addAction(act);
    QObject::connect(act, SIGNAL(wasTriggered(QAction *)), this, SLOT(openRecentDocument(QAction *)));
  }
}

/*
 * Die Statusbar erzeugen. 
 */
void MainWindow::createStatusbar() {
  statusbar = new QStatusBar(this);
  statusbar->setObjectName(QString("statusbar"));
  statusbar->setContentsMargins(2,2,4,2);
  this->setStatusBar(statusbar);
}

/*
 * Die Toolbar erzeugen.
 */
void MainWindow::createToolbars() {
  /* Datei */
  toolbarDatei = new QToolBar(trUtf8("Datei"), this);
  toolbarDatei->setObjectName(QString("toolbarDatei"));
  this->addToolBar(Qt::TopToolBarArea, toolbarDatei);
  
  toolbarDatei->addAction(action_Neu);
  toolbarDatei->addAction(action_Oeffnen);
  toolbarDatei->addAction(action_Speichern);
  toolbarDatei->addAction(action_SpeichernUnter);
  toolbarDatei->addAction(action_AlleSpeichern);
  toolbarDatei->addAction(action_Schliessen);
  
  /* Bearbeiten */
  toolbarBearbeiten = new QToolBar(trUtf8("Bearbeiten"), this);
  toolbarBearbeiten->setObjectName(QString("toolbarBearbeiten"));
  this->addToolBar(Qt::TopToolBarArea, toolbarBearbeiten);
  
  toolbarBearbeiten->addAction(action_Rueckgaengig);
  toolbarBearbeiten->addAction(action_Wiederherstellen);
  toolbarBearbeiten->addAction(action_Ausschneiden);
  toolbarBearbeiten->addAction(action_Kopieren);
  toolbarBearbeiten->addAction(action_Einfuegen);
  toolbarBearbeiten->addAction(action_Suchen);
  
  /* Kompilierwerkzeuge */
  toolbarKompilierwerkzeuge = new QToolBar(trUtf8("Erstellen"), this);
  toolbarKompilierwerkzeuge->setObjectName(QString("toolbarKompilierwerkzeuge"));
  this->addToolBar(Qt::TopToolBarArea, toolbarKompilierwerkzeuge);
  
  toolbarKompilierwerkzeuge->addAction(action_kompiliereLatex);
  toolbarKompilierwerkzeuge->addAction(action_kompilierePdflatex);
}

/*
 * Den Arbeitsbereich erzeugen.
 */
void MainWindow::createWorkspace() {
  /* Das zentrale Widget einrichten */
  centralwidget = new QWidget(this);
  centralwidget->setObjectName(QString("centralwidget"));
  this->setCentralWidget(centralwidget);
  
  /* Das Grundlayout fuer die Arbeitsflaeche einrichten */
  hboxLayout = new QHBoxLayout(centralwidget);
  hboxLayout->setObjectName(QString("hboxLayout"));
  hboxLayout->setContentsMargins(0, 0, 0, 0);
  
  /* Splitter zwischen Eingabe und Ausgabe einrichten */
  vSplitter = new QSplitter(centralwidget);
  vSplitter->setObjectName(QString("vSplitter"));
  vSplitter->setOrientation(Qt::Vertical);
  hboxLayout->addWidget(vSplitter);
  
  /* Splitter zwischen Toolbox und Tabs einrichten */
  hSplitter = new QSplitter(vSplitter);
  hSplitter->setObjectName(QString("hSplitter"));
  vSplitter->addWidget(hSplitter);
  
  /* SizePolicy fuer den Splitter setzen */
  QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  sizePolicy.setHorizontalStretch(1);
  sizePolicy.setVerticalStretch(5);
  sizePolicy.setHeightForWidth(hSplitter->sizePolicy().hasHeightForWidth());
  hSplitter->setSizePolicy(sizePolicy);
  hSplitter->setOrientation(Qt::Horizontal);
  
  /* Einrichten der Toolbox */
  toolbox = new QToolBox(hSplitter);
  toolbox->setObjectName(QString("toolbox"));
  hSplitter->addWidget(toolbox);
  
  /* SizePolicy fuer die Toolbox setzen */
  QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
  sizePolicy1.setHorizontalStretch(1);
  sizePolicy1.setVerticalStretch(1);
  sizePolicy1.setHeightForWidth(toolbox->sizePolicy().hasHeightForWidth());
  toolbox->setSizePolicy(sizePolicy1);
  
  /* Seiten in die Toolbox einfuegen */
  page = new QWidget();
  page->setObjectName(QString("page"));
  toolbox->addItem(page, trUtf8("Seite 1"));
  page_2 = new QWidget();
  page_2->setObjectName(QString("page_2"));
  toolbox->addItem(page_2, trUtf8("Seite 2"));
  toolbox->setCurrentIndex(0);
  
  /* Einrichten des Tab */
  tabs = new QTabWidget(hSplitter);
  tabs->setObjectName(QString("tabs"));
  hSplitter->addWidget(tabs);
  
  closeButton = new QToolButton(tabs);
  closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
  tabs->setCornerWidget(closeButton);
  
  /* SizePolicy fuer das Tab setzen */
  QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
  sizePolicy2.setHorizontalStretch(12);
  sizePolicy2.setVerticalStretch(1);
  sizePolicy2.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
  tabs->setSizePolicy(sizePolicy2);
  
  /* Ausgabefeld fuer Compilermeldungen einrichten */
  output = new QTextEdit(vSplitter);
  output->setFont(QFont("monospace", 8));
  output->setObjectName(QString("output"));
  vSplitter->addWidget(output);
  sizePolicy1.setHeightForWidth(output->sizePolicy().hasHeightForWidth());
  output->setSizePolicy(sizePolicy1);
}

void MainWindow::find() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  if (findDialog->exec() == QDialog::Rejected) {
    return;
  }
  
  curInput->find(findDialog->getSearchText(), findDialog->getSearchFlags());
}

void MainWindow::findNext() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  QString searchText = findDialog->getSearchText();
  QTextDocument::FindFlags searchFlags = findDialog->getSearchFlags();
  
  if (searchText.isEmpty() || searchText.isNull()) {
    if (findDialog->exec() == QDialog::Rejected) {
      return;
    }
  }
  
  curInput->find(findDialog->getSearchText(), findDialog->getSearchFlags());
}

void MainWindow::findPrevious() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  QString searchText = findDialog->getSearchText();
  QTextDocument::FindFlags searchFlags = findDialog->getSearchFlags() | QTextDocument::FindBackward;
  
  if (searchText.isEmpty() || searchText.isNull()) {
    if (findDialog->exec() == QDialog::Rejected) {
      return;
    }
  }
  
  curInput->find(findDialog->getSearchText(), findDialog->getSearchFlags() ^ QTextDocument::FindBackward);
}

/*
 * Liefert den aktuell offenen Editor zurück oder 0, falls
 * noch keiner offen ist.
 */
Editor * MainWindow::getCurrentEditor() {
  if (tabs->count() == 0 || editorList.size() == 0) {
    return 0;
  }
  
  return editorList.at(tabs->currentIndex());
}

/*
 * Veranlasst das Neuladen der Einstellungen.
 */
void MainWindow::loadSettings() {
  for (int i = 0; i < editorList.size(); i++) {
    Editor *curInput = editorList.at(i);
    if (curInput == 0) {
      continue;
    }
    
    curInput->loadSettings();
  }
}

/*
 * Ein neues (leeres) Dokument anlegen.
 * Dieser Teil wird später vermutlich
 * überschrieben um Vorlagen auswählen
 * zu können.
 */
void MainWindow::newDocument() {
  newFileCount++;
  QString filename = trUtf8("Unbenannt") + QString::number(newFileCount);
  
  /* Eingabefeld einrichten */
  Editor *input = new Editor();
  
  int index = tabs->addTab(input, filename);
  input->setFocus();
  editorList.push_back(input);
  
  tabs->setCurrentIndex(index);
  reconnectTab(index);
  
  action_Speichern->setEnabled(true);
  action_SpeichernUnter->setEnabled(true);
  action_AlleSpeichern->setEnabled(true);
  action_Schliessen->setEnabled(true);
  action_AlleSchliessen->setEnabled(true);
  action_Einfuegen->setEnabled(true);
}

void MainWindow::openDocument() {
  openDocument(QString());
}

/*
 * Ein vorhandenes Dokument öffnen. Es wird dazu
 * ein Datei-Öffnen-Dialog angezeigt.
 */
void MainWindow::openDocument(QString filename) {
  Editor *input = new Editor(this);
  if (input->openDocument(filename) == true) {
    QString filename = input->getFilename();
    
    recentFiles.addFile(filename);
    
    int index = tabs->addTab(input, filename.mid(filename.lastIndexOf(QDir::separator()) + 1));
    input->setFocus();
    editorList.push_back(input);
    
    tabs->setCurrentIndex(index);
    reconnectTab(index);
    
    action_Speichern->setEnabled(true);
    action_SpeichernUnter->setEnabled(true);
    action_AlleSpeichern->setEnabled(true);
    action_Schliessen->setEnabled(true);
    action_AlleSchliessen->setEnabled(true);
    action_Einfuegen->setEnabled(true);
  } else {
    delete input;
    input = 0;
  }
}

void MainWindow::openRecentDocument(QAction *action) {
  if (action == 0) {
    return;
  }
  
  QString filename = action->text();
  filename = filename.mid(filename.indexOf(QString(":"), 0) + 1).trimmed();
  openDocument(filename);
}

/*
 * Die Anwendung schließen. Dazu zunächst alle Tabs
 * schließen und eventuell ein Speichern bestimmter
 * Dokumente veranlassen, vgl. closeAllTabs().
 */
void MainWindow::quit() {
  closeAllTabs();
  if (tabs->count() > 0) {
    return;
  }
  
  close();
}

/*
 * Wird das Tab gewechselt, so müssen einige Connections
 * neu erstellt werden, damit etwa die Funktionen undo() 
 * und redo() im richtigen Editor ausgeführt werden. Das
 * wird in dieser Funktion erledigt.
 */
void MainWindow::reconnectTab(int newIndex) {
  /* Zunächst alle Menupunkt entkoppeln */
  QObject::disconnect(action_Rueckgaengig, 0, 0, 0);
  QObject::disconnect(action_Wiederherstellen, 0, 0, 0);
  QObject::disconnect(action_Ausschneiden, 0, 0, 0);
  QObject::disconnect(action_Kopieren, 0, 0, 0);
  QObject::disconnect(action_Einfuegen, 0, 0, 0);
  
  /* Dann jeden Editor durchgehen und die wichtigen Verbindungen
  kappen */
  Editor *curInput;
  for (int i = 0; i < editorList.size(); i++) {
    curInput = editorList.at(i);
    if (curInput == 0) {
      continue;
    }
    
    QObject::disconnect(curInput, 0, action_Rueckgaengig, 0);
    QObject::disconnect(curInput, 0, action_Wiederherstellen, 0);
  }
  
  /* Nun den neuen Editor laden */
  curInput = editorList.at(newIndex);
  if (curInput == 0) {
    return;
  }
  
  /* Und für diesen alle wichtigen Verbindungen neu erzeugen */
  QObject::connect(curInput, SIGNAL(undoAvailable(bool)), action_Rueckgaengig, SLOT(setEnabled(bool)));
  QObject::connect(curInput, SIGNAL(redoAvailable(bool)), action_Wiederherstellen, SLOT(setEnabled(bool)));
  QObject::connect(curInput, SIGNAL(copyAvailable(bool)), action_Ausschneiden, SLOT(setEnabled(bool)));
  QObject::connect(curInput, SIGNAL(copyAvailable(bool)), action_Kopieren, SLOT(setEnabled(bool)));
  
  QObject::connect(action_Rueckgaengig, SIGNAL(triggered()), curInput, SLOT(undo()));
  QObject::connect(action_Wiederherstellen, SIGNAL(triggered()), curInput, SLOT(redo()));
  QObject::connect(action_Ausschneiden, SIGNAL(triggered()), curInput, SLOT(cut()));
  QObject::connect(action_Kopieren, SIGNAL(triggered()), curInput, SLOT(copy()));
  QObject::connect(action_Einfuegen, SIGNAL(triggered()), curInput, SLOT(paste()));
  
  QObject::connect(curInput, SIGNAL(newCursorPosition(QString)), statusbar, SLOT(showMessage(QString)));
  
  /* Abschließend können einige Menupunkt wieder aktiviert werden */
  action_Rueckgaengig->setEnabled(curInput->getUndo());
  action_Wiederherstellen->setEnabled(curInput->getRedo());
  action_Ausschneiden->setEnabled(curInput->getCopy());
  action_Kopieren->setEnabled(curInput->getCopy());
  
  statusbar->showMessage(trUtf8("Zeile 1, Spalte 1"));
}

void MainWindow::replace() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  if (replaceDialog->exec() == QDialog::Rejected) {
    return;
  }
  
  curInput->replace(replaceDialog->getSearchText(), replaceDialog->getReplacementText(), replaceDialog->getSearchFlags(), replaceDialog->getPromptOnReplace());
}

/* 
 * Speichert das aktuelle Dokument ab.
 */
void MainWindow::save() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  QString oldFilename = curInput->getFilename();
  if (curInput->save() == true) {
    QString filename = curInput->getFilename();
    
    // Einfügen in die recent Liste
    if (oldFilename != filename) {
      recentFiles.addFile(filename);
    }
    
    filename = filename.mid(filename.lastIndexOf(QDir::separator()) + 1);
    tabs->setTabText(tabs->currentIndex(), filename);
  }
}

/*
 * Speichert alle offenen Dokumente ab.
 */
void MainWindow::saveAll() {
  if (editorList.size() == 0) {
    return;
  }
  
  Editor *curInput;
  for (int i = 0; i < editorList.size(); i++) {
    curInput = editorList.at(i);
    if (curInput == 0) {
      continue;
    }
    
    tabs->setCurrentIndex(i);
    
    QString oldFilename = curInput->getFilename();
    if (curInput->save() == true) {
      QString filename = curInput->getFilename();
      
      // Einfügen in die recent Liste
      if (oldFilename != filename) {
        recentFiles.addFile(filename);
      }
    
      filename = filename.mid(filename.lastIndexOf(QDir::separator()) + 1);
      tabs->setTabText(tabs->currentIndex(), filename);
    }
  }
}

/*
 * Öffnet für das aktuelle Dokument einen 
 * Dateinamen-Auswahldialog, um es unter
 * einem anderen Namen zu speichern.
 */
void MainWindow::saveAs() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  QString oldFilename = curInput->getFilename();
  if (curInput->saveAs() == true) {
    QString filename = curInput->getFilename();
    
    // Einfügen in die recent Liste
    if (oldFilename != filename) {
      recentFiles.addFile(filename);
    }
    
    filename = filename.mid(filename.lastIndexOf(QDir::separator()) + 1);
    tabs->setTabText(tabs->currentIndex(), filename);
  }
}

/*
 * Zeigt den Einstellungsdialog an und veranlsst ggf.
 * ein Neuladen der Einstellungen.
 */
void MainWindow::settings() {
  SettingDialog dlg(this);
  if (dlg.exec() == QDialog::Accepted) {
    loadSettings();
  }
}
