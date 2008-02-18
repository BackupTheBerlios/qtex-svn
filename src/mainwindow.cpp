#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFlags>
#include <QMessageBox>
#include <QStyle>
#include "mainwindow.h"
#include <stdio.h>

#define BUF_SIZE 1024

MainWindow::MainWindow() : QMainWindow() {
  this->setWindowTitle(tr("QteX"));
  
  createMenus();
  createStatusbar();
  createToolbar();
  createWorkspace();
  createConnections();
  
  newFileCount = 0;
  
  action_Neu->trigger();
}

void MainWindow::closeAllTabs() {
  while (tabs->count() > 0) {
    closeCurrentTab();
  }
}

void MainWindow::closeCurrentTab() {
  if (tabs->currentIndex() >= 0) {
    tabs->removeTab(tabs->currentIndex());
  }
}

void MainWindow::createConnections() {
  /* Menusignale einrichten */
  QObject::connect(action_Neu, SIGNAL(triggered()), this, SLOT(newDocument()));
  QObject::connect(action_Oeffnen, SIGNAL(triggered()), this, SLOT(openDocument()));
  QObject::connect(action_Speichern, SIGNAL(triggered()), this, SLOT(save()));
  QObject::connect(action_SpeichernUnter, SIGNAL(triggered()), this, SLOT(saveAs()));
  QObject::connect(action_AlleSpeichern, SIGNAL(triggered()), this, SLOT(saveAll()));
  QObject::connect(action_Schliessen, SIGNAL(triggered()), this, SLOT(closeCurrentTab()));
  QObject::connect(action_AlleSchliessen, SIGNAL(triggered()), this, SLOT(closeAllTabs()));
  QObject::connect(action_Beenden, SIGNAL(triggered()), this, SLOT(close()));
  
  
  /* Inputfeldsignale einrichten */
  QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(closeCurrentTab()));
}

void MainWindow::createMenus() {
  /* Menuleiste erzeugen */
  menubar = new QMenuBar(this);
  menubar->setObjectName(QString::fromUtf8("menubar"));
  this->setMenuBar(menubar);
  
  /* Menu 'Datei' einrichten */
  menu_Datei = new QMenu(menubar);
  menu_Datei->setObjectName(QString::fromUtf8("menu_Datei"));
  menu_Datei->setTitle(QString::fromUtf8("&Datei"));
  
  action_Neu = new QAction(this);
  action_Neu->setObjectName(QString::fromUtf8("action_Neu"));
  action_Neu->setShortcut(QString::fromUtf8("Ctrl+N"));
  action_Neu->setText(tr("&Neu"));
  
  action_Oeffnen = new QAction(this);
  action_Oeffnen->setObjectName(QString::fromUtf8("action_Oeffnen"));
  action_Oeffnen->setShortcut(QString::fromUtf8("Ctrl+O"));
  action_Oeffnen->setText(QString::fromUtf8("\303\226&ffnen..."));
  
  action_Speichern = new QAction(this);
  action_Speichern->setObjectName(QString::fromUtf8("action_Speichern"));
  action_Speichern->setShortcut(QString::fromUtf8("Ctrl+S"));
  action_Speichern->setText(QString::fromUtf8("&Speichern"));
  
  action_SpeichernUnter = new QAction(this);
  action_SpeichernUnter->setObjectName(QString::fromUtf8("action_SpeichernUnter"));
  action_SpeichernUnter->setText(QString::fromUtf8("Speichern &unter..."));
  
  action_Schliessen = new QAction(this);
  action_Schliessen->setObjectName(QString::fromUtf8("action_Schliessen"));
  action_Schliessen->setText(QString::fromUtf8("Schl&ie\303\237en"));
  
  action_AlleSchliessen = new QAction(this);
  action_AlleSchliessen->setObjectName(QString::fromUtf8("action_AlleSchliessen"));
  action_AlleSchliessen->setText(QString::fromUtf8("Alle schlie\303\237en"));
  
  action_AlleSpeichern = new QAction(this);
  action_AlleSpeichern->setObjectName(QString::fromUtf8("action_AlleSpeichern"));
  action_AlleSpeichern->setText(QString::fromUtf8("&Alle speichern"));
  
  action_Beenden = new QAction(this);
  action_Beenden->setObjectName(QString::fromUtf8("action_Beenden"));
  action_Beenden->setShortcut(QString::fromUtf8("Ctrl+B"));
  action_Beenden->setText(tr("&Beenden"));
  
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
  menu_Bearbeiten->setObjectName(QString::fromUtf8("menu_Bearbeiten"));
  menu_Bearbeiten->setTitle(QString::fromUtf8("&Bearbeiten"));
  
  action_Rueckgaengig = new QAction(this);
  action_Rueckgaengig->setObjectName(QString::fromUtf8("action_Rueckgaengig"));
  action_Rueckgaengig->setShortcut(QString::fromUtf8("Ctrl+R"));
  action_Rueckgaengig->setText(QString::fromUtf8("&R\303\274ckg\303\244ngig"));
  action_Rueckgaengig->setEnabled(false);
  
  action_Wiederherstellen = new QAction(this);
  action_Wiederherstellen->setObjectName(QString::fromUtf8("action_Wiederherstellen"));
  action_Wiederherstellen->setShortcut(QString::fromUtf8("Ctrl+Shift+R"));
  action_Wiederherstellen->setText(tr("&Wiederherstellen"));
  action_Wiederherstellen->setEnabled(false);
  
  menu_Bearbeiten->addAction(action_Rueckgaengig);
  menu_Bearbeiten->addAction(action_Wiederherstellen);
  
  /* Untermenus einfuegen */
  menubar->addAction(menu_Datei->menuAction());
  menubar->addAction(menu_Bearbeiten->menuAction());
}

void MainWindow::createStatusbar() {
  /* Statusbar einrichten */
  statusbar = new QStatusBar(this);
  statusbar->setObjectName(QString::fromUtf8("statusbar"));
  statusbar->setContentsMargins(2,2,4,2);
  this->setStatusBar(statusbar);
  
  /* Labels fuer die Statusleiste einrichten */
  cursorPosition = new QLabel(statusbar);
  cursorPosition->setMinimumWidth(100);
  cursorPosition->setText("Blablubb");
  statusbar->addPermanentWidget(cursorPosition);
}

void MainWindow::createToolbar() {
  /* Toolbar einrichten */
  toolbar = new QToolBar(this);
  toolbar->setObjectName(QString::fromUtf8("toolbar"));
  this->addToolBar(Qt::TopToolBarArea, toolbar);
}

void MainWindow::createWorkspace() {
  /* Das zentrale Widget einrichten */
  centralwidget = new QWidget(this);
  centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
  this->setCentralWidget(centralwidget);
  
  /* Das Grundlayout fuer die Arbeitsflaeche einrichten */
  hboxLayout = new QHBoxLayout(centralwidget);
  hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
  hboxLayout->setContentsMargins(0, 0, 0, 0);
  
  /* Splitter zwischen Eingabe und Ausgabe einrichten */
  vSplitter = new QSplitter(centralwidget);
  vSplitter->setObjectName(QString::fromUtf8("vSplitter"));
  vSplitter->setOrientation(Qt::Vertical);
  hboxLayout->addWidget(vSplitter);
  
  /* Splitter zwischen Toolbox und Tabs einrichten */
  hSplitter = new QSplitter(vSplitter);
  hSplitter->setObjectName(QString::fromUtf8("hSplitter"));
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
  toolbox->setObjectName(QString::fromUtf8("toolbox"));
  hSplitter->addWidget(toolbox);
  
  /* SizePolicy fuer die Toolbox setzen */
  QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
  sizePolicy1.setHorizontalStretch(1);
  sizePolicy1.setVerticalStretch(1);
  sizePolicy1.setHeightForWidth(toolbox->sizePolicy().hasHeightForWidth());
  toolbox->setSizePolicy(sizePolicy1);
  
  /* Seiten in die Toolbox einfuegen */
  page = new QWidget();
  page->setObjectName(QString::fromUtf8("page"));
  toolbox->addItem(page, QString::fromUtf8("Seite 1"));
  page_2 = new QWidget();
  page_2->setObjectName(QString::fromUtf8("page_2"));
  toolbox->addItem(page_2, QString::fromUtf8("Seite 2"));
  toolbox->setCurrentIndex(0);
  
  /* Einrichten des Tab */
  tabs = new QTabWidget(hSplitter);
  tabs->setObjectName(QString::fromUtf8("tabs"));
  hSplitter->addWidget(tabs);
  
  closeButton = new QToolButton(tabs);
  closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
  tabs->setCornerWidget(closeButton);
  
  /* SizePolicy fuer das Tab setzen */
  QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
  sizePolicy2.setHorizontalStretch(2);
  sizePolicy2.setVerticalStretch(1);
  sizePolicy2.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
  tabs->setSizePolicy(sizePolicy2);
  
  /* Ausgabefeld fuer Compilermeldungen einrichten */
  output = new QTextEdit(vSplitter);
  output->setObjectName(QString::fromUtf8("output"));
  vSplitter->addWidget(output);
  sizePolicy1.setHeightForWidth(output->sizePolicy().hasHeightForWidth());
  output->setSizePolicy(sizePolicy1);
}

void MainWindow::maybeSave() {
  if (editorList.size() == 0) {
    return;
  }
  
  Editor *curInput = editorList.at(tabs->currentIndex());
  if (curInput == 0) {
    return;
  }
  
  QString title = QString::fromUtf8("Dokument schlie\303\237en - QteX");
  QString text = QString::fromUtf8("Das Dokument '");
  text += tabs->tabText(tabs->currentIndex());
  text += QString::fromUtf8("' wurde ge\303\244ndert.\n\nM&\303\226chten Sie die \303\204nderungen speichern oder verwerfen?");
  
  int ret = QMessageBox::question(this, title, text, QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
  if (ret == QMessageBox::Cancel) {
    return;
  } else if (ret == QMessageBox::Discard) {
    closeCurrentTab();
    return;
  } else {
    save();
    return;
  }
}

void MainWindow::newDocument() {
  newFileCount++;
  
  /* Eingabefeld einrichten */
  Editor *input = new Editor();
  input->setObjectName(QString::fromUtf8("input"));
  tabs->addTab(input, QString::fromUtf8("Unbenannt") + QString::number(newFileCount));  
  editorList.push_back(input);
  
  QObject::connect(input, SIGNAL(undoAvailable(bool)), action_Rueckgaengig, SLOT(setEnabled(bool)));
  QObject::connect(action_Rueckgaengig, SIGNAL(triggered()), input, SLOT(undo()));
  QObject::connect(input, SIGNAL(redoAvailable(bool)), action_Wiederherstellen, SLOT(setEnabled(bool)));
  QObject::connect(action_Wiederherstellen, SIGNAL(triggered()), input, SLOT(redo()));
}

void MainWindow::openDocument() {
  QString filename = QFileDialog::getOpenFileName(this, QString::fromUtf8("Datei \303\266ffnen"));
  if (filename.isEmpty()) {
    statusbar->showMessage(QString::fromUtf8("Keine Datei ge\303\266ffnet."), 4000);
    return;
  } else if (filename.isNull()) {
    statusbar->showMessage(QString::fromUtf8("Keine Datei ge\303\266ffnet."), 4000);
    return;
  }
  
  openDocument(filename);
}

void MainWindow::openDocument(QString filename) {
  QFile file(filename);
  if (!file.exists()) {
    statusbar->showMessage(QString::fromUtf8("Datei konnte nicht ge\303\266ffnet werden."), 4000);
    return;
  }
  
  if (!file.open(QIODevice::ReadOnly)) {
    statusbar->showMessage(QString::fromUtf8("Datei konnte nicht ge\303\266ffnet werden."), 4000);
    return;
  }
  
  QString allData = QString::fromUtf8(file.readAll());
  file.close();

  /* Eingabefeld einrichten */
  Editor *input = new Editor();
  input->setObjectName(QString::fromUtf8("input"));
  input->setText(allData);
  
  int index = tabs->addTab(input, filename.mid(filename.lastIndexOf(QDir::separator()) + 1));
  tabs->setCurrentIndex(index);
    
  editorList.push_back(input);
}

void MainWindow::save() {
  if (editorList.size() == 0) {
    return;
  }
  
  Editor *curInput = editorList.at(tabs->currentIndex());
  if (curInput == 0) {
    return;
  }
  
  curInput->save();
}

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
    
    curInput->save();
  }
}

void MainWindow::saveAs() {
  if (editorList.size() == 0) {
    return;
  }
  
  Editor *curInput = editorList.at(tabs->currentIndex());
  if (curInput == 0) {
    return;
  }
  
  curInput->saveAs();
}
