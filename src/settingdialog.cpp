#include "settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) : QDialog(parent) {
  this->setWindowTitle(trUtf8("Einstellungen"));
  
  createDialog();
  createConnections();
  
  loadSettings();
}

void SettingDialog::createDialog() {
  QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  
  /* Das Grundlayout erzeugen */
  vLayout = new QVBoxLayout(this);
  vLayout->setObjectName(QString("vLayout"));
  this->setLayout(vLayout);
  
  hLayout = new QHBoxLayout();
  hLayout->setObjectName(QString("hLayout"));
  
  tabs = new QTabWidget(this);
  tabs->setObjectName(QString("tabs"));
  vLayout->addWidget(tabs);
  
  
  cancel = new QPushButton(this);
  cancel->setObjectName(QString("cancel"));
  cancel->setText(trUtf8("Abbrechen"));
  
  save = new QPushButton(this);
  save->setObjectName(QString("save"));
  save->setText(trUtf8("Speichern"));
  
  hLayout->addStretch();
  hLayout->addWidget(save);
  hLayout->addWidget(cancel);
  
  vLayout->addLayout(hLayout);
  
  /* Das 'Editor' - Tab erzeugen */
  QWidget *editorTab = new QWidget(tabs);
  editorTab->setObjectName(QString("editorTab"));
  
  QVBoxLayout *editorLayout = new QVBoxLayout(editorTab);
  editorLayout->setObjectName(QString("editorLayout"));
  
  /* Schriftgruppe */
  QGridLayout *groupFontLayout = new QGridLayout();
  
  QGroupBox *groupFont = new QGroupBox(editorTab);
  groupFont->setObjectName(QString("groupFont"));
  groupFont->setTitle(trUtf8("Schrift"));
  groupFont->setLayout(groupFontLayout);
  
  QLabel *labelFontName = new QLabel(groupFont);
  labelFontName->setObjectName(QString("labelFontName"));
  labelFontName->setText(trUtf8("Schriftart:"));
  groupFontLayout->addWidget(labelFontName, 0, 0);
  
  fontName = new QFontComboBox(groupFont);
  fontName->setObjectName(QString("fontName"));
  groupFontLayout->addWidget(fontName, 0, 1);
  
  fontSize = new QSpinBox(groupFont);
  fontSize->setObjectName(QString("fontSize"));
  fontSize->setSizePolicy(sizePolicy);
  groupFontLayout->addWidget(fontSize, 0, 2);
  
  QLabel *labelFontColor = new QLabel(groupFont);
  labelFontColor->setObjectName(QString("labelFontColor"));
  labelFontColor->setText(trUtf8("Schriftfarbe:"));
  groupFontLayout->addWidget(labelFontColor, 1, 0);
  
  buttonFontColor = new ColorChooseButton(groupFont);
  buttonFontColor->setIconSize(QSize(16, 16));
  buttonFontColor->setMaximumSize(QSize(24, 24));
  buttonFontColor->setObjectName(QString("buttonFontColor"));
  groupFontLayout->addWidget(buttonFontColor, 1, 1);
  
  /* Tabulatorgruppe */
  QGridLayout *groupTabulatorLayout = new QGridLayout();
  
  QGroupBox *groupTabulator = new QGroupBox(editorTab);
  groupTabulator->setObjectName(QString("groupTabulator"));
  groupTabulator->setTitle(trUtf8("Tabulator"));
  groupTabulator->setLayout(groupTabulatorLayout);
  
  QLabel *labelTabulatorWidth = new QLabel(groupTabulator);
  labelTabulatorWidth->setObjectName(QString("labelTabulatorWidth"));
  labelTabulatorWidth->setText(trUtf8("Tabulatorbreite:"));
  groupTabulatorLayout->addWidget(labelTabulatorWidth, 0, 0);
  
  tabulatorWidth = new QSpinBox(groupTabulator);
  tabulatorWidth->setObjectName(QString("tabulatorWidth"));
  groupTabulatorLayout->addWidget(tabulatorWidth, 0, 1);
  
  /* Alles zusammenbauen */
  editorLayout->addWidget(groupFont);
  editorLayout->addWidget(groupTabulator);
  editorLayout->addStretch();
  
  /* Das 'Erstellen' - Tabs */
  QWidget *erstellenTab = new QWidget(this);
  QVBoxLayout *erstellenLayout = new QVBoxLayout(erstellenTab);
  erstellenTab->setLayout(erstellenLayout);
  
  QGridLayout *erstellenTableLayout = new QGridLayout();
  
  QLabel *latexCommandLabel = new QLabel(erstellenTab);
  latexCommandLabel->setText(trUtf8("Latex:"));
  erstellenTableLayout->addWidget(latexCommandLabel, 0, 0);
  
  latexCommandInput = new QLineEdit(erstellenTab);
  latexCommandInput->setObjectName(QString("latexCommandInput"));
  erstellenTableLayout->addWidget(latexCommandInput, 0, 1);
  
  QLabel *pdflatexCommandLabel = new QLabel(erstellenTab);
  pdflatexCommandLabel->setText(trUtf8("Pdflatex:"));
  erstellenTableLayout->addWidget(pdflatexCommandLabel, 1, 0);
  
  pdflatexCommandInput = new QLineEdit(erstellenTab);
  pdflatexCommandInput->setObjectName(QString("pdflatexCommandInput"));
  erstellenTableLayout->addWidget(pdflatexCommandInput, 1, 1);
  
  erstellenLayout->addLayout(erstellenTableLayout);
  erstellenLayout->addStretch();
  
  /* Das 'Synatx-Highlighting' - Tab */
  QWidget *syntaxTab = new QWidget(this);
  QVBoxLayout *syntaxLayout = new QVBoxLayout(syntaxTab);
  syntaxTab->setLayout(syntaxLayout);
  
  QGridLayout *tableLayout = new QGridLayout();
  
  QLabel *headerForeground = new QLabel(syntaxTab);
  headerForeground->setText(trUtf8("Vordergrund"));
  tableLayout->addWidget(headerForeground, 0, 1, Qt::AlignHCenter);
  
  QLabel *headerBackground = new QLabel(syntaxTab);
  headerBackground->setText(trUtf8("Hintergrund"));
  tableLayout->addWidget(headerBackground, 0, 2, Qt::AlignHCenter);
  
  QLabel *headerBold = new QLabel(syntaxTab);
  headerBold->setText(trUtf8("fett"));
  tableLayout->addWidget(headerBold, 0, 3, Qt::AlignHCenter);
  
  QLabel *headerItalic = new QLabel(syntaxTab);
  headerItalic->setText(trUtf8("kursiv"));
  tableLayout->addWidget(headerItalic, 0, 4, Qt::AlignHCenter);
  
  /* erste Zeile */
  QLabel *commentLabel = new QLabel(syntaxTab);
  commentLabel->setText(trUtf8("Kommentare:"));
  tableLayout->addWidget(commentLabel, 1, 0);
  
  buttonCommentForeground = new ColorChooseButton(syntaxTab);
  buttonCommentForeground->setObjectName(QString("buttonCommentForeground"));
  buttonCommentForeground->setIconSize(QSize(16, 16));
  buttonCommentForeground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(buttonCommentForeground, 1, 1, Qt::AlignHCenter);
  
  buttonCommentBackground = new ColorChooseButton(syntaxTab);
  buttonCommentBackground->setObjectName(QString("buttonCommentBackground"));
  buttonCommentBackground->setIconSize(QSize(16, 16));
  buttonCommentBackground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(buttonCommentBackground, 1, 2, Qt::AlignHCenter);
  
  buttonCommentBold = new QCheckBox(syntaxTab);
  buttonCommentBold->setObjectName(QString("buttonCommentBold"));
  tableLayout->addWidget(buttonCommentBold, 1, 3, Qt::AlignHCenter);
  
  buttonCommentItalic = new QCheckBox(syntaxTab);
  buttonCommentItalic->setObjectName(QString("buttonCommentItalic"));
  tableLayout->addWidget(buttonCommentItalic, 1, 4, Qt::AlignHCenter);
  
  /* zweite Zeile */
  QLabel *latexLabel = new QLabel(syntaxTab);
  latexLabel->setText(trUtf8("Latex-Kommandos:"));
  tableLayout->addWidget(latexLabel, 2, 0);
  
  buttonLatexForeground = new ColorChooseButton(syntaxTab);
  buttonLatexForeground->setObjectName(QString("buttonLatexForeground"));
  buttonLatexForeground->setIconSize(QSize(16, 16));
  buttonLatexForeground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(buttonLatexForeground, 2, 1, Qt::AlignHCenter);
  
  buttonLatexBackground = new ColorChooseButton(syntaxTab);
  buttonLatexBackground->setObjectName(QString("buttonLatexBackground"));
  buttonLatexBackground->setIconSize(QSize(16, 16));
  buttonLatexBackground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(buttonLatexBackground, 2, 2, Qt::AlignHCenter);
  
  buttonLatexBold = new QCheckBox(syntaxTab);
  buttonLatexBold->setObjectName(QString("buttonLatexBold"));
  tableLayout->addWidget(buttonLatexBold, 2, 3, Qt::AlignHCenter);
  
  buttonLatexItalic = new QCheckBox(syntaxTab);
  buttonLatexItalic->setObjectName(QString("buttonLatexItalic"));
  tableLayout->addWidget(buttonLatexItalic, 2, 4, Qt::AlignHCenter);
  
  /* dritte Zeile */
  QLabel *mathLabel = new QLabel(syntaxTab);
  mathLabel->setText(trUtf8("Mathemodus:"));
  tableLayout->addWidget(mathLabel, 3, 0);
  
  buttonMathForeground = new ColorChooseButton(syntaxTab);
  buttonMathForeground->setObjectName(QString("buttonMathForeground"));
  buttonMathForeground->setIconSize(QSize(16, 16));
  buttonMathForeground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(buttonMathForeground, 3, 1, Qt::AlignHCenter);
  
  buttonMathBackground = new ColorChooseButton(syntaxTab);
  buttonMathBackground->setObjectName(QString("buttonMathBackground"));
  buttonMathBackground->setIconSize(QSize(16, 16));
  buttonMathBackground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(buttonMathBackground, 3, 2, Qt::AlignHCenter);
  
  buttonMathBold = new QCheckBox(syntaxTab);
  buttonMathBold->setObjectName(QString("buttonMathBold"));
  tableLayout->addWidget(buttonMathBold, 3, 3, Qt::AlignHCenter);
  
  buttonMathItalic = new QCheckBox(syntaxTab);
  buttonMathItalic->setObjectName(QString("buttonMathItalic"));
  tableLayout->addWidget(buttonMathItalic, 3, 4, Qt::AlignHCenter);
  
  syntaxLayout->addLayout(tableLayout) ;
  syntaxLayout->addStretch();
  
  
  tabs->addTab(editorTab, trUtf8("Editor"));
  tabs->addTab(erstellenTab, trUtf8("Erstellen"));
  tabs->addTab(syntaxTab, trUtf8("Syntax Highlighting"));
}

void SettingDialog::createConnections() {
  QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
  QObject::connect(save, SIGNAL(clicked()), this, SLOT(saveSettings()));
}

void SettingDialog::loadSettings() {
  QSettings settings("QteX", "QteX");
  
  /* Schrifteinstellungen laden */
  settings.beginGroup(QString("font"));
  
  fontName->setEditText(settings.value(QString("name"), QString("Monospace")).toString());
  fontSize->setValue(settings.value(QString("size"), 10).toInt());
  
  QColor fontColor = settings.value(QString("color"), Qt::black).value<QColor>();
  buttonFontColor->setColor(fontColor);
  
  settings.endGroup();
  
  /* Tabulatoreinstellungen laden */
  settings.beginGroup(QString("tabulator"));
  this->tabulatorWidth->setValue(settings.value(QString("width"), 2).toInt());
  settings.endGroup();
  
  /* Erstellen-Einstellungen laden */
  settings.beginGroup(QString("compiler"));
  latexCommandInput->setText(settings.value(QString("latex"), QString("latex")).toString());
  pdflatexCommandInput->setText(settings.value(QString("pdflatex"), QString("pdflatex")).toString());
  settings.endGroup();
  
  /* Synatx-Highlighting-Einstellungen laden */
  settings.beginGroup(QString("syntaxhighlighting"));
  buttonCommentForeground->setColor(settings.value(QString("commentForeground"), Qt::red).value<QColor>());
  buttonCommentBackground->setColor(settings.value(QString("commentBackground"), Qt::white).value<QColor>());
  int state = settings.value(QString("commentBold"), Qt::Unchecked).toInt();
  if (state == 0) {
    buttonCommentBold->setCheckState(Qt::Unchecked);
  } else {
    buttonCommentBold->setCheckState(Qt::Checked);
  }
  state = settings.value(QString("commentItalic"), Qt::Checked).toInt();
  if (state == 0) {
    buttonCommentItalic->setCheckState(Qt::Unchecked);
  } else {
    buttonCommentItalic->setCheckState(Qt::Checked);
  }
  
  buttonLatexForeground->setColor(settings.value(QString("latexForeground"), Qt::darkMagenta).value<QColor>());
  buttonLatexBackground->setColor(settings.value(QString("latexBackground"), Qt::white).value<QColor>());
  state = settings.value(QString("latexBold"), Qt::Checked).toInt();
  if (state == 0) {
    buttonLatexBold->setCheckState(Qt::Unchecked);
  } else {
    buttonLatexBold->setCheckState(Qt::Checked);
  }
  state = settings.value(QString("latexItalic"), Qt::Unchecked).toInt();
  if (state == 0) {
    buttonLatexItalic->setCheckState(Qt::Unchecked);
  } else {
    buttonLatexItalic->setCheckState(Qt::Checked);
  }
  
  buttonMathForeground->setColor(settings.value(QString("mathForeground"), Qt::black).value<QColor>());
  buttonMathBackground->setColor(settings.value(QString("mathBackground"), Qt::yellow).value<QColor>());
  state = settings.value(QString("mathBold"), Qt::Unchecked).toInt();
  if (state == 0) {
    buttonMathBold->setCheckState(Qt::Unchecked);
  } else {
    buttonMathBold->setCheckState(Qt::Checked);
  }
  state = settings.value(QString("mathItalic"), Qt::Unchecked).toInt();
  if (state == 0) {
    buttonMathItalic->setCheckState(Qt::Unchecked);
  } else {
    buttonMathItalic->setCheckState(Qt::Checked);
  }
  settings.endGroup();
}

void SettingDialog::saveSettings() {
  QColor fontColor = buttonFontColor->getColor();
  QSettings settings("QteX", "QteX");
  
  /* Schrifteinstellungen */
  settings.beginGroup(QString("font"));
  settings.setValue(QString("name"), fontName->currentText());
  settings.setValue(QString("size"), fontSize->value());
  settings.setValue(QString("color"), fontColor);
  settings.endGroup();
  
  /* Tabulatoreinstellungen */
  settings.beginGroup(QString("tabulator"));
  settings.setValue(QString("width"), tabulatorWidth->value());
  settings.endGroup();
  
  /* Erstellen-Einstellungen */
  settings.beginGroup(QString("compiler"));
  settings.setValue(QString("latex"), latexCommandInput->text());
  settings.setValue(QString("pdflatex"), pdflatexCommandInput->text());
  settings.endGroup();
  
  /* Synatxhighlighting-Einstellungen */
  settings.beginGroup(QString("syntaxhighlighting"));
  settings.setValue(QString("commentForeground"), buttonCommentForeground->getColor());
  settings.setValue(QString("commentBackground"), buttonCommentBackground->getColor());
  settings.setValue(QString("commentBold"), buttonCommentBold->checkState());
  settings.setValue(QString("commentItalic"), buttonCommentItalic->checkState());
  
  settings.setValue(QString("latexForeground"), buttonLatexForeground->getColor());
  settings.setValue(QString("latexBackground"), buttonLatexBackground->getColor());
  settings.setValue(QString("latexBold"), buttonLatexBold->checkState());
  settings.setValue(QString("latexItalic"), buttonLatexItalic->checkState());
  
  settings.setValue(QString("mathForeground"), buttonMathForeground->getColor());
  settings.setValue(QString("mathBackground"), buttonMathBackground->getColor());
  settings.setValue(QString("mathBold"), buttonMathBold->checkState());
  settings.setValue(QString("mathItalic"), buttonMathItalic->checkState());
  settings.endGroup();
  
  accept();
}
