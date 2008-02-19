#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFlags>
#include <QMessageBox>
#include <QStyle>
#include "mainwindow.h"
#include <stdio.h>

#define APP_TITLE "QteX"
#define BUF_SIZE 1024

MainWindow::MainWindow() : QMainWindow() {
  this->setWindowTitle(trUtf8(APP_TITLE));
  
  createMenus();
  createStatusbar();
  createToolbar();
  createWorkspace();
  createConnections();
  
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
    action_Ausschneiden->setEnabled(false);
    action_Kopieren->setEnabled(false);
    action_Einfuegen->setEnabled(false);
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

  /* Inputfeldsignale einrichten */
  QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(closeCurrentTab()));
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
  action_Neu->setObjectName(QString("action_Neu"));
  action_Neu->setShortcut(QString("Ctrl+N"));
  action_Neu->setText(trUtf8("&Neu"));
  
  action_Oeffnen = new QAction(this);
  action_Oeffnen->setObjectName(QString("action_Oeffnen"));
  action_Oeffnen->setShortcut(QString("Ctrl+O"));
  action_Oeffnen->setText(trUtf8("Ö&ffnen..."));
  
  action_Speichern = new QAction(this);
  action_Speichern->setEnabled(false);
  action_Speichern->setObjectName(QString("action_Speichern"));
  action_Speichern->setShortcut(QString("Ctrl+S"));
  action_Speichern->setText(trUtf8("&Speichern"));
  
  action_SpeichernUnter = new QAction(this);
  action_SpeichernUnter->setEnabled(false);
  action_SpeichernUnter->setObjectName(QString("action_SpeichernUnter"));
  action_SpeichernUnter->setText(trUtf8("Speichern &unter..."));
  
  action_AlleSpeichern = new QAction(this);
  action_AlleSpeichern->setEnabled(false);
  action_AlleSpeichern->setObjectName(QString("action_AlleSpeichern"));
  action_AlleSpeichern->setText(trUtf8("&Alle speichern"));
  
  action_Schliessen = new QAction(this);
  action_Schliessen->setEnabled(false);
  action_Schliessen->setObjectName(QString("action_Schliessen"));
  action_Schliessen->setText(trUtf8("Schl&ießen"));
  
  action_AlleSchliessen = new QAction(this);
  action_AlleSchliessen->setEnabled(false);
  action_AlleSchliessen->setObjectName(QString("action_AlleSchliessen"));
  action_AlleSchliessen->setText(trUtf8("Alle schließen"));
  
  action_Beenden = new QAction(this);
  action_Beenden->setObjectName(QString("action_Beenden"));
  action_Beenden->setShortcut(QString("Ctrl+B"));
  action_Beenden->setText(trUtf8("&Beenden"));
  
  menu_Datei->addAction(action_Neu);
  menu_Datei->addAction(action_Oeffnen);
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
  action_Rueckgaengig->setObjectName(QString("action_Rueckgaengig"));
  action_Rueckgaengig->setShortcut(QString("Ctrl+R"));
  action_Rueckgaengig->setText(trUtf8("&Rückgängig"));
  action_Rueckgaengig->setEnabled(false);
  
  action_Wiederherstellen = new QAction(this);
  action_Wiederherstellen->setObjectName(QString("action_Wiederherstellen"));
  action_Wiederherstellen->setShortcut(QString("Ctrl+Shift+R"));
  action_Wiederherstellen->setText(trUtf8("&Wiederherstellen"));
  action_Wiederherstellen->setEnabled(false);
  
  action_Ausschneiden = new QAction(this);
  action_Ausschneiden->setObjectName(QString("action_Ausschneiden"));
  action_Ausschneiden->setShortcut(QString("Ctrl+X"));
  action_Ausschneiden->setText(trUtf8("&Ausschneiden"));
  
  action_Kopieren = new QAction(this);
  action_Kopieren->setObjectName(QString("action_Kopieren"));
  action_Kopieren->setShortcut(QString("Ctrl+C"));
  action_Kopieren->setText(trUtf8("&Kopieren"));
  
  action_Einfuegen = new QAction(this);
  action_Einfuegen->setObjectName(QString("action_Einfuegen"));
  action_Einfuegen->setShortcut(QString("Ctrl+V"));
  action_Einfuegen->setText(trUtf8("&Einfügen"));
  
  menu_Bearbeiten->addAction(action_Rueckgaengig);
  menu_Bearbeiten->addAction(action_Wiederherstellen);
  menu_Bearbeiten->addSeparator();
  menu_Bearbeiten->addAction(action_Ausschneiden);
  menu_Bearbeiten->addAction(action_Kopieren);
  menu_Bearbeiten->addAction(action_Einfuegen);
  
  /* Untermenus einfuegen */
  menubar->addAction(menu_Datei->menuAction());
  menubar->addAction(menu_Bearbeiten->menuAction());
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
void MainWindow::createToolbar() {
  toolbar = new QToolBar(this);
  toolbar->setObjectName(QString("toolbar"));
  this->addToolBar(Qt::TopToolBarArea, toolbar);
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
  output->setObjectName(QString("output"));
  vSplitter->addWidget(output);
  sizePolicy1.setHeightForWidth(output->sizePolicy().hasHeightForWidth());
  output->setSizePolicy(sizePolicy1);
}

/*
 * Ein neues (leeres) Dokument anlegen.
 * Dieser Teil wird später vermutlich
 * überschrieben um Vorlagen auswählen
 * zu können.
 */
void MainWindow::newDocument() {
  newFileCount++;
  
  /* Eingabefeld einrichten */
  Editor *input = new Editor();
  input->setObjectName(QString("input"));
  input->setFont(QFont("monospace", 10));
  int index = tabs->addTab(input, trUtf8("Unbenannt") + QString::number(newFileCount));  
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

/*
 * Ein vorhandenes Dokument öffnen. Es wird dazu
 * ein Datei-Öffnen-Dialog angezeigt.
 */
void MainWindow::openDocument() {
  QString filename = QFileDialog::getOpenFileName(this, trUtf8("Datei öffnen"));
  if (filename.isEmpty() || filename.isNull()) {
    statusbar->showMessage(trUtf8("Keine Datei geöffnet."), 4000);
    return;
  }
  
  openDocument(filename);
}

/*
 * Ein vorhandenes Dokument öffnen. Es wird versucht,
 * das durch den Dateinamen übergebene Dokument zu laden.
 */
void MainWindow::openDocument(QString filename) {
  QFile file(filename);
  if (file.exists()) {
    QMessageBox::critical(this, trUtf8("Fehler - QteX"), trUtf8("Die angeforderte Datei existiert nicht!"));
    return;
  }
  
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::critical(this, trUtf8("Fehler - QteX"), trUtf8("Die angeforderte Datei konnte nicht geöffnet werden!"));
    return;
  }
  
  QString allData = QString::fromUtf8(file.readAll());
  file.close();

  /* Eingabefeld einrichten */
  Editor *input = new Editor();
  input->setObjectName(QString("input"));
  input->setText(allData);  
  int index = tabs->addTab(input, filename.mid(filename.lastIndexOf(QDir::separator()) + 1));    
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
  
  /* Abschließend können einige Menupunkt wieder aktiviert werden */
  action_Rueckgaengig->setEnabled(curInput->getUndo());
  action_Wiederherstellen->setEnabled(curInput->getRedo());
  action_Ausschneiden->setEnabled(curInput->getCopy());
  action_Kopieren->setEnabled(curInput->getCopy());
}

/* 
 * Speichert das aktuelle Dokument ab.
 */
void MainWindow::save() {
  if (editorList.size() == 0) {
    return;
  }
  
  Editor *curInput = editorList.at(tabs->currentIndex());
  if (curInput == 0) {
    return;
  }
  
  if (curInput->save() == true) {
    QString filename = curInput->getFilename();
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
    
    if (curInput->save() == true) {
      QString filename = curInput->getFilename();
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
  if (editorList.size() == 0) {
    return;
  }
  
  Editor *curInput = editorList.at(tabs->currentIndex());
  if (curInput == 0) {
    return;
  }
  
  if (curInput->saveAs() == true) {
    QString filename = curInput->getFilename();
    filename = filename.mid(filename.lastIndexOf(QDir::separator()) + 1);
    tabs->setTabText(tabs->currentIndex(), filename);
  }
}
