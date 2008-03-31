#include "mainwindow.h"

#define BUF_SIZE 1024

MainWindow::MainWindow() : QMainWindow() {
  setWindowIcon(QIcon(":/images/qtexlogo.png"));
  setWindowTitle(tr("QteX"));
  
  createMenus();
  createStatusbar();
  createToolbars();
  createWorkspace();
  createConnections();

  m_compiler = new Compiler(m_log);
  m_compiler->checkEnvironment();
  
  m_findDialog = new FindDialog(this);
  m_replaceDialog = new ReplaceDialog(this);
  
  m_newFileCount = 0;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  slotCloseAllTabs();
  if (m_tabs->count() > 0) {
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

void MainWindow::createCodecMenu() {
  m_codecGroup = new QActionGroup(m_menuEncoding);
  m_codecGroup->setObjectName(QString("m_codecGroup"));

  QStringList codecs, descriptions;
  codecs = CodecAction::getAvailableCodecs();
  descriptions = CodecAction::getAvailableDescriptions();
  
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("editor"));
  QString defaultCodec = settings.value(QString("defaultCodec"), CodecAction::getDefaultCodec()).toString();
  settings.endGroup();
  
  CodecAction *action;
  QString name;
  for (int i = 0; i < codecs.size(); i++) {
    name = codecs.at(i);
    
    action = new CodecAction(0, descriptions.at(i), codecs.at(i));
    action->setCheckable(true);
    m_codecGroup->addAction(action);
    
    connect(action, SIGNAL(signalWasTriggered(CodecAction *)), this, SLOT(slotChangeCodec(CodecAction *)));
    
    if (defaultCodec == codecs.at(i)) {
      action->setChecked(true);
    }
  }
  
  m_menuEncoding->addActions(m_codecGroup->actions());
}

/*
 * Bestimmte unveränderliche Verbindungen erzeugen.
 */
void MainWindow::createConnections() {
  connect(m_tabs, SIGNAL(currentChanged(int)), this, SLOT(slotReconnectTab(int)));
  
  /* Menusignale einrichten */
  connect(m_actionNew, SIGNAL(triggered()), this, SLOT(slotNewDocument()));
  connect(m_actionOpen, SIGNAL(triggered()), this, SLOT(slotOpenDocument()));
  connect(m_actionSave, SIGNAL(triggered()), this, SLOT(slotSave()));
  connect(m_actionSaveAs, SIGNAL(triggered()), this, SLOT(slotSaveAs()));
  connect(m_actionSaveAll, SIGNAL(triggered()), this, SLOT(slotSaveAll()));
  connect(m_actionClose, SIGNAL(triggered()), this, SLOT(slotCloseCurrentTab()));
  connect(m_actionCloseAll, SIGNAL(triggered()), this, SLOT(slotCloseAllTabs()));
  connect(m_actionQuit, SIGNAL(triggered()), this, SLOT(slotQuit()));
  
  connect(m_actionFind, SIGNAL(triggered()), this, SLOT(slotFind()));
  connect(m_actionFindNext, SIGNAL(triggered()), this, SLOT(slotFindNext()));
  connect(m_actionFindPrevious, SIGNAL(triggered()), this, SLOT(slotFindPrevious()));
  connect(m_actionReplace, SIGNAL(triggered()), this, SLOT(slotReplace()));
  connect(m_actionSettings, SIGNAL(triggered()), this, SLOT(slotSettings()));
  
  connect(m_actionCompileLatex, SIGNAL(triggered()), this, SLOT(slotCompileLatex()));
  connect(m_actionCompilePdflatex, SIGNAL(triggered()), this, SLOT(slotCompilePdflatex()));
  
  //connect(m_codecGroup, SIGNAL(triggered(QAction *)), this, SLOT(slotChangeCodec(QAction *)));
  
  connect(m_actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
  connect(m_actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  /* Weitere Signale einrichten */
  connect(m_closeButton, SIGNAL(clicked()), this, SLOT(slotCloseCurrentTab()));
  connect(&m_recentFiles, SIGNAL(signalUpdate()), this, SLOT(slotCreateRecentFilesMenu()));
}

/*
 * Die Menustruktur erzeugen.
 */
void MainWindow::createMenus() {
  /* Menuleiste erzeugen */
  m_menubar = new QMenuBar(this);
  m_menubar->setObjectName(QString("m_menubar"));
  setMenuBar(m_menubar);
  
  /* Menu 'Datei' einrichten */
  m_menuFile = new QMenu(m_menubar);
  m_menuFile->setObjectName(QString("m_menuFile"));
  m_menuFile->setTitle(tr("&File"));
  
  m_actionNew = new QAction(this);
  m_actionNew->setIcon(QIcon(":/images/filenew.png"));
  m_actionNew->setObjectName(QString("m_actionNew"));
  m_actionNew->setShortcut(tr("Ctrl+N"));
  m_actionNew->setText(tr("&New"));
  
  m_actionOpen = new QAction(this);
  m_actionOpen->setIcon(QIcon(":/images/fileopen.png"));
  m_actionOpen->setObjectName(QString("m_actionOpen"));
  m_actionOpen->setShortcut(tr("Ctrl+O"));
  m_actionOpen->setText(tr("&Open..."));
  
  m_actionRecentlyOpen = new QAction(this);
  m_actionRecentlyOpen->setObjectName(QString("m_actionRecentlyOpen"));
  m_actionRecentlyOpen->setText(tr("&Recently open"));
  
  // Untermenü für die letzten Dateien
  m_menuRecentlyOpen = new QMenu(m_menubar);
  m_actionRecentlyOpen->setMenu(m_menuRecentlyOpen);
  slotCreateRecentFilesMenu();
  
  m_actionSave = new QAction(this);
  m_actionSave->setEnabled(false);
  m_actionSave->setIcon(QIcon(":/images/filesave.png"));
  m_actionSave->setObjectName(QString("m_actionSave"));
  m_actionSave->setShortcut(tr("Ctrl+S"));
  m_actionSave->setText(tr("&Save"));
  
  m_actionSaveAs = new QAction(this);
  m_actionSaveAs->setEnabled(false);
  m_actionSaveAs->setIcon(QIcon(":/images/filesaveas.png"));
  m_actionSaveAs->setObjectName(QString("m_actionSaveAs"));
  m_actionSaveAs->setText(tr("Save &as..."));
  
  m_actionSaveAll = new QAction(this);
  m_actionSaveAll->setEnabled(false);
  m_actionSaveAll->setIcon(QIcon(":/images/filesaveall.png"));
  m_actionSaveAll->setObjectName(QString("m_actionSaveAll"));
  m_actionSaveAll->setText(tr("Save a&ll"));
  
  m_actionClose = new QAction(this);
  m_actionClose->setEnabled(false);
  m_actionClose->setIcon(QIcon(":/images/fileclose.png"));
  m_actionClose->setObjectName(QString("m_actionClose"));
  m_actionClose->setText(tr("&Close"));
  
  m_actionCloseAll = new QAction(this);
  m_actionCloseAll->setEnabled(false);
  m_actionCloseAll->setObjectName(QString("m_actionCloseAll"));
  m_actionCloseAll->setText(tr("Close All"));
  
  m_actionQuit = new QAction(this);
  m_actionQuit->setObjectName(QString("m_actionQuit"));
  m_actionQuit->setShortcut(tr("Ctrl+Q"));
  m_actionQuit->setText(tr("&Quit"));
  
  m_menuFile->addAction(m_actionNew);
  m_menuFile->addAction(m_actionOpen);
  m_menuFile->addAction(m_actionRecentlyOpen);
  m_menuFile->addSeparator();
  m_menuFile->addAction(m_actionSave);
  m_menuFile->addAction(m_actionSaveAs);
  m_menuFile->addSeparator();
  m_menuFile->addAction(m_actionSaveAll);
  m_menuFile->addAction(m_actionClose);
  m_menuFile->addAction(m_actionCloseAll);
  m_menuFile->addSeparator();
  m_menuFile->addAction(m_actionQuit);
  
  /* Menu 'Bearbeiten' einrichten */
  m_menuEdit = new QMenu(m_menubar);
  m_menuEdit->setObjectName(QString("m_menuEdit"));
  m_menuEdit->setTitle(tr("&Edit"));
  
  m_actionUndo = new QAction(this);
  m_actionUndo->setEnabled(false);
  m_actionUndo->setIcon(QIcon(":/images/undo.png"));
  m_actionUndo->setObjectName(QString("m_actionUndo"));
  m_actionUndo->setShortcut(tr("Ctrl+Z"));
  m_actionUndo->setText(tr("&Undo"));
  
  m_actionRedo = new QAction(this);
  m_actionRedo->setEnabled(false);
  m_actionRedo->setIcon(QIcon(":/images/redo.png"));
  m_actionRedo->setObjectName(QString("m_actionRedo"));
  m_actionRedo->setShortcut(tr("Ctrl+Shift+Z"));
  m_actionRedo->setText(tr("&Redo"));
  
  m_actionCut = new QAction(this);
  m_actionCut->setIcon(QIcon(":/images/editcut.png"));
  m_actionCut->setObjectName(QString("m_actionCut"));
  m_actionCut->setShortcut(tr("Ctrl+X"));
  m_actionCut->setText(tr("Cu&t"));
  
  m_actionCopy = new QAction(this);
  m_actionCopy->setIcon(QIcon(":/images/editcopy.png"));
  m_actionCopy->setObjectName(QString("m_actionCopy"));
  m_actionCopy->setShortcut(tr("Ctrl+C"));
  m_actionCopy->setText(tr("&Copy"));
  
  m_actionPaste = new QAction(this);
  m_actionPaste->setIcon(QIcon(":/images/editpaste.png"));
  m_actionPaste->setObjectName(QString("m_actionPaste"));
  m_actionPaste->setShortcut(tr("Ctrl+V"));
  m_actionPaste->setText(tr("&Paste"));
  
  m_actionFind = new QAction(this);
  m_actionFind->setIcon(QIcon(":/images/find.png"));
  m_actionFind->setObjectName(QString("m_actionFind"));
  m_actionFind->setShortcut(tr("Ctrl+F"));
  m_actionFind->setText(tr("&Find..."));
  
  m_actionFindNext = new QAction(this);
  m_actionFindNext->setObjectName(QString("m_actionFindNext"));
  m_actionFindNext->setShortcut(tr("F3"));
  m_actionFindNext->setText(tr("Find &next"));
  
  m_actionFindPrevious = new QAction(this);
  m_actionFindPrevious->setObjectName(QString("m_actionFindPrevious"));
  m_actionFindPrevious->setShortcut(tr("Shift+F3"));
  m_actionFindPrevious->setText(tr("Find pr&evious"));
  
  m_actionReplace = new QAction(this);
  m_actionReplace->setObjectName(QString("m_actionReplace"));
  m_actionReplace->setShortcut(tr("Ctrl+R"));
  m_actionReplace->setText(tr("&Replace..."));
  
  m_actionSettings = new QAction(this);
  m_actionSettings->setObjectName(QString("m_actionSettings"));
  m_actionSettings->setText(tr("&Settings..."));
  
  m_menuEdit->addAction(m_actionUndo);
  m_menuEdit->addAction(m_actionRedo);
  m_menuEdit->addSeparator();
  m_menuEdit->addAction(m_actionCut);
  m_menuEdit->addAction(m_actionCopy);
  m_menuEdit->addAction(m_actionPaste);
  m_menuEdit->addSeparator();
  m_menuEdit->addAction(m_actionFind);
  m_menuEdit->addAction(m_actionFindNext);
  m_menuEdit->addAction(m_actionFindPrevious);
  m_menuEdit->addAction(m_actionReplace);
  m_menuEdit->addSeparator();
  m_menuEdit->addAction(m_actionSettings);
  
  /* Menu 'Erstellen' einrichten */
  m_menuBuild = new QMenu(m_menubar);
  m_menuBuild->setObjectName(QString("m_menuBuild"));
  m_menuBuild->setTitle(tr("&Build"));
  
  m_actionCompileLatex = new QAction(this);
  m_actionCompileLatex->setIcon(QIcon(":/images/latex.png"));
  m_actionCompileLatex->setObjectName(QString("m_actionCompileLatex"));
  m_actionCompileLatex->setShortcut(tr("Alt+1"));
  m_actionCompileLatex->setText(tr("&Latex"));
  
  m_actionCompilePdflatex = new QAction(this);
  m_actionCompilePdflatex->setIcon(QIcon(":/images/pdflatex.png"));
  m_actionCompilePdflatex->setObjectName(QString("m_actionCompilePdflatex"));
  m_actionCompilePdflatex->setShortcut(tr("Alt+2"));
  m_actionCompilePdflatex->setText(tr("&PdfLatex"));
  
  m_menuBuild->addAction(m_actionCompileLatex);
  m_menuBuild->addAction(m_actionCompilePdflatex);
  
  /* Menu 'Encoding' einrichten */
  m_menuEncoding = new QMenu(m_menubar);
  m_menuEncoding->setObjectName(QString("m_menuEncoding"));
  m_menuEncoding->setTitle(tr("En&coding"));
  createCodecMenu();
  
  /* Menu 'About' einrichten */
  m_menuAbout = new QMenu(m_menubar);
  m_menuAbout->setObjectName(QString("m_menuAbout"));
  m_menuAbout->setTitle(tr("&About"));
  
  m_actionAbout = new QAction(this);
  m_actionAbout->setObjectName(QString("m_actionAbout"));
  m_actionAbout->setText(tr("&About QteX..."));
  
  m_actionAboutQt = new QAction(this);
  m_actionAboutQt->setObjectName(QString("m_actionAboutQt"));
  m_actionAboutQt->setText(tr("About &Qt..."));
  
  m_menuAbout->addAction(m_actionAbout);
  m_menuAbout->addAction(m_actionAboutQt);
  
  /* Untermenus einfuegen */
  m_menubar->addAction(m_menuFile->menuAction());
  m_menubar->addAction(m_menuEdit->menuAction());
  m_menubar->addAction(m_menuBuild->menuAction());
  m_menubar->addAction(m_menuEncoding->menuAction());
  m_menubar->addAction(m_menuAbout->menuAction());
}

/*
 * Die Statusbar erzeugen. 
 */
void MainWindow::createStatusbar() {
  m_statusbar = new QStatusBar(this);
  m_statusbar->setObjectName(QString("m_statusbar"));
  m_statusbar->setContentsMargins(2,2,4,2);
  setStatusBar(m_statusbar);
}

/*
 * Die Toolbar erzeugen.
 */
void MainWindow::createToolbars() {
  /* Datei */
  m_toolbarFile = new QToolBar(tr("File"), this);
  m_toolbarFile->setObjectName(QString("m_toolbarFile"));
  this->addToolBar(Qt::TopToolBarArea, m_toolbarFile);
  
  m_toolbarFile->addAction(m_actionNew);
  m_toolbarFile->addAction(m_actionOpen);
  m_toolbarFile->addAction(m_actionSave);
  m_toolbarFile->addAction(m_actionSaveAs);
  m_toolbarFile->addAction(m_actionSaveAll);
  m_toolbarFile->addAction(m_actionClose);
  
  /* Bearbeiten */
  m_toolbarEdit = new QToolBar(tr("Edit"), this);
  m_toolbarEdit->setObjectName(QString("m_toolbarEdit"));
  this->addToolBar(Qt::TopToolBarArea, m_toolbarEdit);
  
  m_toolbarEdit->addAction(m_actionUndo);
  m_toolbarEdit->addAction(m_actionRedo);
  m_toolbarEdit->addAction(m_actionCut);
  m_toolbarEdit->addAction(m_actionCopy);
  m_toolbarEdit->addAction(m_actionPaste);
  m_toolbarEdit->addAction(m_actionFind);
  
  /* Kompilierwerkzeuge */
  m_toolbarBuild = new QToolBar(tr("Build"), this);
  m_toolbarBuild->setObjectName(QString("m_toolbarBuild"));
  this->addToolBar(Qt::TopToolBarArea, m_toolbarBuild);
  
  m_toolbarBuild->addAction(m_actionCompileLatex);
  m_toolbarBuild->addAction(m_actionCompilePdflatex);
}

/*
 * Den Arbeitsbereich erzeugen.
 */
void MainWindow::createWorkspace() {
  /* Das zentrale Widget einrichten */
  m_centralwidget = new QWidget(this);
  m_centralwidget->setObjectName(QString("m_centralwidget"));
  this->setCentralWidget(m_centralwidget);
  
  /* Das Grundlayout fuer die Arbeitsflaeche einrichten */
  m_hboxLayout = new QHBoxLayout(m_centralwidget);
  m_hboxLayout->setObjectName(QString("m_hboxLayout"));
  m_hboxLayout->setContentsMargins(0, 0, 0, 0);
  
  /* Splitter zwischen Eingabe und Ausgabe einrichten */
  m_vSplitter = new QSplitter(m_centralwidget);
  m_vSplitter->setObjectName(QString("m_vSplitter"));
  m_vSplitter->setOrientation(Qt::Vertical);
  m_hboxLayout->addWidget(m_vSplitter);
  
  /* Splitter zwischen Toolbox und Tabs einrichten */
  m_hSplitter = new QSplitter(m_vSplitter);
  m_hSplitter->setObjectName(QString("m_hSplitter"));
  m_vSplitter->addWidget(m_hSplitter);
  
  /* SizePolicy fuer den Splitter setzen */
  QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  sizePolicy.setHorizontalStretch(1);
  sizePolicy.setVerticalStretch(5);
  sizePolicy.setHeightForWidth(m_hSplitter->sizePolicy().hasHeightForWidth());
  m_hSplitter->setSizePolicy(sizePolicy);
  m_hSplitter->setOrientation(Qt::Horizontal);
  
  /* Einrichten der Toolbox */
  m_toolbox = new QToolBox(m_hSplitter);
  m_toolbox->setObjectName(QString("m_toolbox"));
  m_hSplitter->addWidget(m_toolbox);
  
  /* SizePolicy fuer die Toolbox setzen */
  QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
  sizePolicy1.setHorizontalStretch(1);
  sizePolicy1.setVerticalStretch(1);
  sizePolicy1.setHeightForWidth(m_toolbox->sizePolicy().hasHeightForWidth());
  m_toolbox->setSizePolicy(sizePolicy1);
  
  /* Seiten in die Toolbox einfuegen */
  m_page = new QWidget();
  m_page->setObjectName(QString("m_page"));
  m_toolbox->addItem(m_page, tr("Page 1"));
  m_page2 = new QWidget();
  m_page2->setObjectName(QString("m_page_2"));
  m_toolbox->addItem(m_page2, tr("Page 2"));
  m_toolbox->setCurrentIndex(0);
  
  /* Einrichten des Tab */
  m_tabs = new QTabWidget(m_hSplitter);
  m_tabs->setObjectName(QString("m_tabs"));
  m_hSplitter->addWidget(m_tabs);
  
  m_closeButton = new QToolButton(m_tabs);
  m_closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
  m_tabs->setCornerWidget(m_closeButton);
  
  /* SizePolicy fuer das Tab setzen */
  QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
  sizePolicy2.setHorizontalStretch(12);
  sizePolicy2.setVerticalStretch(1);
  sizePolicy2.setHeightForWidth(m_tabs->sizePolicy().hasHeightForWidth());
  m_tabs->setSizePolicy(sizePolicy2);
  
  /* Ausgabefeld fuer Compilermeldungen einrichten */
  m_log = new LogConsole(m_vSplitter);
  m_log->setFont(QFont("monospace", 8));
  m_log->setObjectName(QString("m_log"));
  m_vSplitter->addWidget(m_log);
  sizePolicy1.setHeightForWidth(m_log->sizePolicy().hasHeightForWidth());
  m_log->setSizePolicy(sizePolicy1);
}

/*
 * Führt die gewünschte Suche durch. Werte für type:
 * 0 = find(), 1 = findNext(), 2 = findPrevious() 
 */
void MainWindow::find(int type) {
  if (type < 0 || type > 2) {
    return;
  }
  
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  if (type == 0) {
    if (m_findDialog->slotExec() == QDialog::Rejected) {
      return;
    }
  } else { // type == 1 oder type == 2
    QString str = m_findDialog->getSearchText();
    if (str.isEmpty() || str.isNull()) {
      if (m_findDialog->slotExec() == QDialog::Rejected) {
        return;
      }
    }
  }
  
  QString searchText = m_findDialog->getSearchText();
  QTextDocument::FindFlags searchFlags = m_findDialog->getSearchFlags();
  
  if (type == 2) {
    searchFlags ^= QTextDocument::FindBackward;
  }
  
  curInput->find(searchText, searchFlags);
}

/*
 * Liefert den aktuell offenen Editor zurück oder 0, falls
 * noch keiner offen ist.
 */
Editor * MainWindow::getCurrentEditor() {
  if (m_tabs->count() == 0 || m_editorList.size() == 0) {
    return 0;
  }
  
  return m_editorList.at(m_tabs->currentIndex());
}

/*
 * Veranlasst das Neuladen der Einstellungen.
 */
void MainWindow::loadSettings() {
  for (int i = 0; i < m_editorList.size(); i++) {
    Editor *curInput = m_editorList.at(i);
    if (curInput == 0) {
      continue;
    }
    
    curInput->loadSettings();
  }
}

/*
 * Ein vorhandenes Dokument öffnen. Es wird dazu
 * ein Datei-Öffnen-Dialog angezeigt.
 */
void MainWindow::openDocument(QString filename) {
  Editor *input = new Editor(this);
  if (input->openDocument(filename) == true) {
    QString filename = input->getFilename();
    
    m_recentFiles.addFile(filename);
    
    int index = m_tabs->addTab(input, filename.mid(filename.lastIndexOf(QDir::separator()) + 1));
    input->setFocus();
    m_editorList.push_back(input);
    
    m_tabs->setCurrentIndex(index);
    slotReconnectTab(index);
    
    //m_actionSave->setEnabled(true);
    m_actionSaveAs->setEnabled(true);
    m_actionSaveAll->setEnabled(true);
    m_actionClose->setEnabled(true);
    m_actionCloseAll->setEnabled(true);
    m_actionPaste->setEnabled(true);
  } else {
    delete input;
    input = 0;
  }
}

void MainWindow::slotAbout() {
  AboutDialog *dlg = new AboutDialog(this);
  dlg->exec();
}

void MainWindow::slotChangeCodec(CodecAction *action) {
  if (action == 0) {
    return;
  }
  
  /*QString name = action->text();
  if (name.isNull() || name.isEmpty()) {
    return;
  }
  
  QString codecName = name.mid(name.lastIndexOf(QString("(")) + 1);
  codecName = codecName.mid(0, codecName.length() - 1);*/
  
  QTextCodec *codec = QTextCodec::codecForName(action->getCodec().toLatin1());
  if (codec == 0) {
    return;
  }
  
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  curInput->slotChangeCodec(codec);
}

/*
 * Alle Tabs schließen. Sobald für ein Tab in einem
 * eventuellen Speichervorgang 'Abbrechen' gewählt
 * wird, werden keine weiteren Tabs geschlossen.
 */ 
void MainWindow::slotCloseAllTabs() {
  int tabCount = m_tabs->count();
  while (tabCount > 0) {
    int oldTabCount = m_tabs->count();
    slotCloseCurrentTab();
    if (m_tabs->count() == oldTabCount) {
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
void MainWindow::slotCloseCurrentTab() {
  int curIndex = m_tabs->currentIndex();
  if (curIndex >= 0) {
    Editor *curInput = m_editorList.at(curIndex);
    if (curInput == 0) {
      m_tabs->removeTab(m_tabs->currentIndex());
      return;
    }
    
    if (curInput->maybeSave() == true) {
      m_editorList.removeAt(curIndex);
      m_tabs->removeTab(curIndex);
    }
  }
  
  /* Alle Tabs geschlossen */
  if (m_tabs->count() == 0) {
    m_actionSave->setEnabled(false);
    m_actionSaveAs->setEnabled(false);
    m_actionSaveAll->setEnabled(false);
    m_actionClose->setEnabled(false);
    m_actionCloseAll->setEnabled(false);
    m_actionUndo->setEnabled(false);
    m_actionRedo->setEnabled(false);
    m_actionCut->setEnabled(false);
    m_actionCopy->setEnabled(false);
    m_actionPaste->setEnabled(false);
  } else {
    Editor *curInput = getCurrentEditor();
    if (curInput == 0) {
      return;
    }
    
    curInput->setFocus();
  }
}

/*
 * Das aktuelle Dokument mit latex kompilieren lassen.
 * Dazu MUSS das Dokument zunächst gespeichert werden.
 */
void MainWindow::slotCompileLatex() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  /* Das Dokument MUSS vor dem Kompilieren gespeichert werden */
  if (curInput->save() == true) {
    m_compiler->compileLatex(curInput->getFilename());
  }
}

/*
 * Das aktuelle Dokument mit pdflatex kompilieren lassen.
 * Dazu MUSS das Dokument zunächst gespeichert werden.
 */
void MainWindow::slotCompilePdflatex() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  /* Das Dokument MUSS vor dem Kompilieren gespeichert werden */
  if (curInput->save() == true) {
    m_compiler->compilePdflatex(curInput->getFilename());
  }
}

void MainWindow::slotContentChanged(Editor *curInput) {
  if (curInput == 0) {
    return;
  }
  
  int index = m_tabs->indexOf(curInput);
  if (index == -1) {
    return;
  }
  
  if (curInput->hasChanged()) {
    m_actionSave->setEnabled(true);
    QString str = m_tabs->tabText(index);
    if (!str.startsWith(QString("* "))) {
      m_tabs->setTabText(index, QString("* ") + str);
    }
  } else {
    m_actionSave->setEnabled(false);
    QString str = m_tabs->tabText(index);
    if (str.startsWith(QString("* "))) {
      m_tabs->setTabText(index, str.mid(2));
    }
  }
}

void MainWindow::slotCreateRecentFilesMenu() {
  QList<QAction*> tmp = m_menuRecentlyOpen->actions();
  for (int i = 0; i < tmp.size(); i++) {
    QAction *a = tmp.at(i);
    if (a == 0) {
      continue;
    }
    
    m_menuRecentlyOpen->removeAction(a);
  }
  
  QList<RecentFileAction*> actions = m_recentFiles.getActions();
  for (int i = 0; i < actions.size(); i++) {
    RecentFileAction *act = actions.at(i);
    if (act == 0) {
      continue;
    }
    
    m_menuRecentlyOpen->addAction(act);
    connect(act, SIGNAL(signalWasTriggered(QAction *)), this, SLOT(slotOpenRecentDocument(QAction *)));
  }
}

void MainWindow::slotFind() {
  find(0);
}

void MainWindow::slotFindNext() {
  find(1);
}

void MainWindow::slotFindPrevious() {
  find(2);
}

/*
 * Ein neues (leeres) Dokument anlegen.
 * Dieser Teil wird später vermutlich
 * überschrieben um Vorlagen auswählen
 * zu können.
 */
void MainWindow::slotNewDocument() {
  m_newFileCount++;
  QString filename = tr("Unnamed") + QString::number(m_newFileCount);
  
  /* Eingabefeld einrichten */
  Editor *input = new Editor();
  
  int index = m_tabs->addTab(input, filename);
  input->setFocus();
  m_editorList.push_back(input);
  
  m_tabs->setCurrentIndex(index);
  slotReconnectTab(index);
  input->setChanged(true);
  
  //m_actionSave->setEnabled(true);
  m_actionSaveAs->setEnabled(true);
  m_actionSaveAll->setEnabled(true);
  m_actionClose->setEnabled(true);
  m_actionCloseAll->setEnabled(true);
  m_actionPaste->setEnabled(true);
}

void MainWindow::slotOpenDocument() {
  openDocument(QString());
}

void MainWindow::slotOpenRecentDocument(QAction *action) {
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
void MainWindow::slotQuit() {
  slotCloseAllTabs();
  if (m_tabs->count() > 0) {
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
void MainWindow::slotReconnectTab(int newIndex) {
  /* Zunächst alle Menupunkt entkoppeln */
  disconnect(m_actionUndo, 0, 0, 0);
  disconnect(m_actionRedo, 0, 0, 0);
  disconnect(m_actionCut, 0, 0, 0);
  disconnect(m_actionCopy, 0, 0, 0);
  disconnect(m_actionPaste, 0, 0, 0);
  
  /* Dann jeden Editor durchgehen und die wichtigen Verbindungen
  kappen */
  Editor *curInput;
  for (int i = 0; i < m_editorList.size(); i++) {
    curInput = m_editorList.at(i);
    if (curInput == 0) {
      continue;
    }
    
    disconnect(curInput, 0, m_actionUndo, 0);
    disconnect(curInput, 0, m_actionRedo, 0);
    disconnect(curInput, SIGNAL(signalContentChanged(Editor *)), 0, 0);
  }
  
  /* Nun den neuen Editor laden */
  curInput = m_editorList.at(newIndex);
  if (curInput == 0) {
    return;
  }
  
  m_log->setCurrentEditor(curInput);
  
  /* Und für diesen alle wichtigen Verbindungen neu erzeugen */
  connect(curInput, SIGNAL(signalContentChanged(Editor *)), this, SLOT(slotContentChanged(Editor *)));
  connect(curInput, SIGNAL(undoAvailable(bool)), m_actionUndo, SLOT(setEnabled(bool)));
  connect(curInput, SIGNAL(redoAvailable(bool)), m_actionRedo, SLOT(setEnabled(bool)));
  connect(curInput, SIGNAL(copyAvailable(bool)), m_actionCut, SLOT(setEnabled(bool)));
  connect(curInput, SIGNAL(copyAvailable(bool)), m_actionCopy, SLOT(setEnabled(bool)));
  
  connect(m_actionUndo, SIGNAL(triggered()), curInput, SLOT(undo()));
  connect(m_actionRedo, SIGNAL(triggered()), curInput, SLOT(redo()));
  connect(m_actionCut, SIGNAL(triggered()), curInput, SLOT(cut()));
  connect(m_actionCopy, SIGNAL(triggered()), curInput, SLOT(copy()));
  connect(m_actionPaste, SIGNAL(triggered()), curInput, SLOT(paste()));
  
  connect(curInput, SIGNAL(signalCursorPositionChanged(QString)), m_statusbar, SLOT(showMessage(QString)));
  
  /* Abschließend können einige Menupunkt wieder aktiviert werden */
  m_actionUndo->setEnabled(curInput->getUndo());
  m_actionRedo->setEnabled(curInput->getRedo());
  m_actionCut->setEnabled(curInput->getCopy());
  m_actionCopy->setEnabled(curInput->getCopy());
  
  m_statusbar->showMessage(tr("Line 1, Column 1"));
}

void MainWindow::slotReplace() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  if (m_replaceDialog->slotExec() == QDialog::Rejected) {
    return;
  }
  
  curInput->replace(m_replaceDialog->getSearchText(), m_replaceDialog->getReplacementText(), m_replaceDialog->getSearchFlags(), m_replaceDialog->getPromptOnReplace());
}

/* 
 * Speichert das aktuelle Dokument ab.
 */
void MainWindow::slotSave() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  QString oldFilename = curInput->getFilename();
  if (curInput->save() == true) {
    QString filename = curInput->getFilename();
    
    // Einfügen in die recent Liste
    if (oldFilename != filename) {
      m_recentFiles.addFile(filename);
    }
    
    filename = filename.mid(filename.lastIndexOf(QDir::separator()) + 1);
    m_tabs->setTabText(m_tabs->currentIndex(), filename);
  }
}

/*
 * Speichert alle offenen Dokumente ab.
 */
void MainWindow::slotSaveAll() {
  if (m_editorList.size() == 0) {
    return;
  }
  
  Editor *curInput;
  for (int i = 0; i < m_editorList.size(); i++) {
    curInput = m_editorList.at(i);
    if (curInput == 0) {
      continue;
    }
    
    m_tabs->setCurrentIndex(i);
    
    QString oldFilename = curInput->getFilename();
    if (curInput->save() == true) {
      QString filename = curInput->getFilename();
      
      // Einfügen in die recent Liste
      if (oldFilename != filename) {
        m_recentFiles.addFile(filename);
      }
    
      filename = filename.mid(filename.lastIndexOf(QDir::separator()) + 1);
      m_tabs->setTabText(m_tabs->currentIndex(), filename);
    }
  }
}

/*
 * Öffnet für das aktuelle Dokument einen 
 * Dateinamen-Auswahldialog, um es unter
 * einem anderen Namen zu speichern.
 */
void MainWindow::slotSaveAs() {
  Editor *curInput = getCurrentEditor();
  if (curInput == 0) {
    return;
  }
  
  QString oldFilename = curInput->getFilename();
  if (curInput->saveAs() == true) {
    QString filename = curInput->getFilename();
    
    // Einfügen in die recent Liste
    if (oldFilename != filename) {
      m_recentFiles.addFile(filename);
    }
    
    filename = filename.mid(filename.lastIndexOf(QDir::separator()) + 1);
    m_tabs->setTabText(m_tabs->currentIndex(), filename);
  }
}

/*
 * Zeigt den Einstellungsdialog an und veranlsst ggf.
 * ein Neuladen der Einstellungen.
 */
void MainWindow::slotSettings() {
  SettingDialog dlg(this);
  if (dlg.exec() == QDialog::Accepted) {
    loadSettings();
  }
}
