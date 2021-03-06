#include "settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("Settings"));
  
  createDialog();
  createConnections();
  
  loadSettings();
  
  m_languageChanged = false;
}

void SettingDialog::createDialog() {
  QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  
  /* Das Grundlayout erzeugen */
  m_vLayout = new QVBoxLayout(this);
  m_vLayout->setObjectName(QString("m_vLayout"));
  setLayout(m_vLayout);
  
  m_hLayout = new QHBoxLayout();
  m_hLayout->setObjectName(QString("m_hLayout"));
  
  m_tabs = new QTabWidget(this);
  m_tabs->setObjectName(QString("m_tabs"));
  m_vLayout->addWidget(m_tabs);
  
  
  m_cancel = new QPushButton(this);
  m_cancel->setObjectName(QString("m_cancel"));
  m_cancel->setText(tr("Cancel"));
  
  m_save = new QPushButton(this);
  m_save->setObjectName(QString("m_save"));
  m_save->setText(tr("Save"));
  
  m_hLayout->addStretch();
  m_hLayout->addWidget(m_save);
  m_hLayout->addWidget(m_cancel);
  
  m_vLayout->addLayout(m_hLayout);
  
  /* Das 'Editor' - Tab erzeugen */
  QWidget *editorTab = new QWidget(m_tabs);
  editorTab->setObjectName(QString("editorTab"));
  
  QVBoxLayout *editorLayout = new QVBoxLayout(editorTab);
  editorLayout->setObjectName(QString("editorLayout"));
  
  QLabel *labelLanguages = new QLabel(m_tabs);
  labelLanguages->setObjectName(QString("labelLanguage"));
  labelLanguages->setText(tr("Language:"));
  
  m_languages = new QComboBox(m_tabs);
  m_languages->setObjectName(QString("m_languages"));
  m_languages->addItems(LanguageChooser::getLanguages());
  QString suffix = "(" + LanguageChooser::getCurrentLanguageCode() + ")";
  for (int i = 0; i < m_languages->count(); i++) {
    if (m_languages->itemText(i).endsWith(suffix)) {
      m_languages->setCurrentIndex(i);
      break;
    } else {
      continue;
    }
  }
  
  QLabel *labelCodecs = new QLabel(m_tabs);
  labelCodecs->setObjectName(QString("labelCodecs"));
  labelCodecs->setText(tr("Default encoding:"));
  
  m_codecs = new QComboBox(m_tabs);
  m_codecs->setObjectName(QString("m_codecs"));
  m_codecs->addItems(CodecAction::getAvailableCodecs());
  for (int i = 0; i < m_codecs->count(); i++) {
    if (m_codecs->itemText(i) == CodecAction::getDefaultCodec()) {
      m_codecs->setCurrentIndex(i);
      break;
    } else {
      continue;
    }
  }
  
  /* Schriftgruppe */
  QGridLayout *groupFontLayout = new QGridLayout();
  
  QGroupBox *groupFont = new QGroupBox(editorTab);
  groupFont->setObjectName(QString("groupFont"));
  groupFont->setTitle(tr("Font"));
  groupFont->setLayout(groupFontLayout);
  
  QLabel *labelFontName = new QLabel(groupFont);
  labelFontName->setObjectName(QString("labelFontName"));
  labelFontName->setText(tr("Name:"));
  groupFontLayout->addWidget(labelFontName, 0, 0);
  
  m_fontName = new QFontComboBox(groupFont);
  m_fontName->setObjectName(QString("m_fontName"));
  groupFontLayout->addWidget(m_fontName, 0, 1);
  
  QLabel *labelFontSize = new QLabel(groupFont);
  labelFontSize->setObjectName(QString("labelFontName"));
  labelFontSize->setText(tr("Size:"));
  groupFontLayout->addWidget(labelFontSize, 1, 0);
  
  m_fontSize = new QSpinBox(groupFont);
  m_fontSize->setObjectName(QString("m_fontSize"));
  m_fontSize->setSizePolicy(sizePolicy);
  groupFontLayout->addWidget(m_fontSize, 1, 1);
  
  QLabel *labelFontColor = new QLabel(groupFont);
  labelFontColor->setObjectName(QString("labelFontColor"));
  labelFontColor->setText(tr("Color:"));
  groupFontLayout->addWidget(labelFontColor, 2, 0);
  
  m_buttonFontColor = new ColorChooseButton(groupFont);
  m_buttonFontColor->setIconSize(QSize(16, 16));
  m_buttonFontColor->setMaximumSize(QSize(24, 24));
  m_buttonFontColor->setObjectName(QString("m_buttonFontColor"));
  groupFontLayout->addWidget(m_buttonFontColor, 2, 1);
  
  /* Tabulatorgruppe */
  QGridLayout *groupTabulatorLayout = new QGridLayout();
  
  QGroupBox *groupTabulator = new QGroupBox(editorTab);
  groupTabulator->setObjectName(QString("groupTabulator"));
  groupTabulator->setTitle(tr("Tab"));
  groupTabulator->setLayout(groupTabulatorLayout);
  
  QLabel *labelTabulatorWidth = new QLabel(groupTabulator);
  labelTabulatorWidth->setObjectName(QString("labelTabulatorWidth"));
  labelTabulatorWidth->setText(tr("Tabwidth:"));
  groupTabulatorLayout->addWidget(labelTabulatorWidth, 0, 0);
  
  m_tabulatorWidth = new QSpinBox(groupTabulator);
  m_tabulatorWidth->setObjectName(QString("m_tabulatorWidth"));
  groupTabulatorLayout->addWidget(m_tabulatorWidth, 0, 1);
  
  /* Alles zusammenbauen */
  editorLayout->addWidget(labelLanguages);
  editorLayout->addWidget(m_languages);
  editorLayout->addWidget(labelCodecs);
  editorLayout->addWidget(m_codecs);
  editorLayout->addWidget(groupFont);
  editorLayout->addWidget(groupTabulator);
  editorLayout->addStretch();
  
  /* Das 'Erstellen' - Tabs */
  QWidget *erstellenTab = new QWidget(this);
  QVBoxLayout *erstellenLayout = new QVBoxLayout(erstellenTab);
  erstellenTab->setLayout(erstellenLayout);
  
  QGridLayout *erstellenTableLayout = new QGridLayout();
  
  QLabel *latexCommandLabel = new QLabel(erstellenTab);
  latexCommandLabel->setText(tr("Latex:"));
  erstellenTableLayout->addWidget(latexCommandLabel, 0, 0);
  
  m_latexCommandInput = new QLineEdit(erstellenTab);
  m_latexCommandInput->setObjectName(QString("m_latexCommandInput"));
  erstellenTableLayout->addWidget(m_latexCommandInput, 0, 1);
  
  QLabel *pdflatexCommandLabel = new QLabel(erstellenTab);
  pdflatexCommandLabel->setText(tr("Pdflatex:"));
  erstellenTableLayout->addWidget(pdflatexCommandLabel, 1, 0);
  
  m_pdflatexCommandInput = new QLineEdit(erstellenTab);
  m_pdflatexCommandInput->setObjectName(QString("m_pdflatexCommandInput"));
  erstellenTableLayout->addWidget(m_pdflatexCommandInput, 1, 1);
  
  erstellenLayout->addLayout(erstellenTableLayout);
  erstellenLayout->addStretch();
  
  /* Das 'Synatx-Highlighting' - Tab */
  QWidget *syntaxTab = new QWidget(this);
  QVBoxLayout *syntaxLayout = new QVBoxLayout(syntaxTab);
  syntaxTab->setLayout(syntaxLayout);
  
  QGridLayout *tableLayout = new QGridLayout();
  
  QLabel *headerForeground = new QLabel(syntaxTab);
  headerForeground->setText(tr("Foreground"));
  tableLayout->addWidget(headerForeground, 0, 1, Qt::AlignHCenter);
  
  QLabel *headerBackground = new QLabel(syntaxTab);
  headerBackground->setText(tr("Background"));
  tableLayout->addWidget(headerBackground, 0, 2, Qt::AlignHCenter);
  
  QLabel *headerBold = new QLabel(syntaxTab);
  headerBold->setText(tr("bold"));
  tableLayout->addWidget(headerBold, 0, 3, Qt::AlignHCenter);
  
  QLabel *headerItalic = new QLabel(syntaxTab);
  headerItalic->setText(tr("italic"));
  tableLayout->addWidget(headerItalic, 0, 4, Qt::AlignHCenter);
  
  /* erste Zeile */
  QLabel *commentLabel = new QLabel(syntaxTab);
  commentLabel->setText(tr("Commentary:"));
  tableLayout->addWidget(commentLabel, 1, 0);
  
  m_buttonCommentForeground = new ColorChooseButton(syntaxTab);
  m_buttonCommentForeground->setObjectName(QString("m_buttonCommentForeground"));
  m_buttonCommentForeground->setIconSize(QSize(16, 16));
  m_buttonCommentForeground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(m_buttonCommentForeground, 1, 1, Qt::AlignHCenter);
  
  m_buttonCommentBackground = new ColorChooseButton(syntaxTab);
  m_buttonCommentBackground->setObjectName(QString("m_buttonCommentBackground"));
  m_buttonCommentBackground->setIconSize(QSize(16, 16));
  m_buttonCommentBackground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(m_buttonCommentBackground, 1, 2, Qt::AlignHCenter);
  
  m_buttonCommentBold = new QCheckBox(syntaxTab);
  m_buttonCommentBold->setObjectName(QString("m_buttonCommentBold"));
  tableLayout->addWidget(m_buttonCommentBold, 1, 3, Qt::AlignHCenter);
  
  m_buttonCommentItalic = new QCheckBox(syntaxTab);
  m_buttonCommentItalic->setObjectName(QString("m_buttonCommentItalic"));
  tableLayout->addWidget(m_buttonCommentItalic, 1, 4, Qt::AlignHCenter);
  
  /* zweite Zeile */
  QLabel *latexLabel = new QLabel(syntaxTab);
  latexLabel->setText(tr("Latexcommand:"));
  tableLayout->addWidget(latexLabel, 2, 0);
  
  m_buttonLatexForeground = new ColorChooseButton(syntaxTab);
  m_buttonLatexForeground->setObjectName(QString("m_buttonLatexForeground"));
  m_buttonLatexForeground->setIconSize(QSize(16, 16));
  m_buttonLatexForeground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(m_buttonLatexForeground, 2, 1, Qt::AlignHCenter);
  
  m_buttonLatexBackground = new ColorChooseButton(syntaxTab);
  m_buttonLatexBackground->setObjectName(QString("m_buttonLatexBackground"));
  m_buttonLatexBackground->setIconSize(QSize(16, 16));
  m_buttonLatexBackground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(m_buttonLatexBackground, 2, 2, Qt::AlignHCenter);
  
  m_buttonLatexBold = new QCheckBox(syntaxTab);
  m_buttonLatexBold->setObjectName(QString("m_buttonLatexBold"));
  tableLayout->addWidget(m_buttonLatexBold, 2, 3, Qt::AlignHCenter);
  
  m_buttonLatexItalic = new QCheckBox(syntaxTab);
  m_buttonLatexItalic->setObjectName(QString("m_buttonLatexItalic"));
  tableLayout->addWidget(m_buttonLatexItalic, 2, 4, Qt::AlignHCenter);
  
  /* dritte Zeile */
  QLabel *mathLabel = new QLabel(syntaxTab);
  mathLabel->setText(tr("Mathmode:"));
  tableLayout->addWidget(mathLabel, 3, 0);
  
  m_buttonMathForeground = new ColorChooseButton(syntaxTab);
  m_buttonMathForeground->setObjectName(QString("m_buttonMathForeground"));
  m_buttonMathForeground->setIconSize(QSize(16, 16));
  m_buttonMathForeground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(m_buttonMathForeground, 3, 1, Qt::AlignHCenter);
  
  m_buttonMathBackground = new ColorChooseButton(syntaxTab);
  m_buttonMathBackground->setObjectName(QString("m_buttonMathBackground"));
  m_buttonMathBackground->setIconSize(QSize(16, 16));
  m_buttonMathBackground->setMaximumSize(QSize(24, 24));
  tableLayout->addWidget(m_buttonMathBackground, 3, 2, Qt::AlignHCenter);
  
  m_buttonMathBold = new QCheckBox(syntaxTab);
  m_buttonMathBold->setObjectName(QString("m_buttonMathBold"));
  tableLayout->addWidget(m_buttonMathBold, 3, 3, Qt::AlignHCenter);
  
  m_buttonMathItalic = new QCheckBox(syntaxTab);
  m_buttonMathItalic->setObjectName(QString("m_buttonMathItalic"));
  tableLayout->addWidget(m_buttonMathItalic, 3, 4, Qt::AlignHCenter);
  
  syntaxLayout->addLayout(tableLayout) ;
  syntaxLayout->addStretch();
  
  
  m_tabs->addTab(editorTab, tr("Editor"));
  m_tabs->addTab(erstellenTab, tr("Build"));
  m_tabs->addTab(syntaxTab, tr("Syntax Highlighting"));
}

void SettingDialog::createConnections() {
  connect(m_cancel, SIGNAL(clicked()), this, SLOT(reject()));
  connect(m_save, SIGNAL(clicked()), this, SLOT(slotSaveSettings()));
  connect(m_languages, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLanguageChanged()));
}

void SettingDialog::loadSettings() {
  QSettings settings("QteX", "QteX");
  
  settings.beginGroup(QString("editor"));
  //m_languages->setCurrentText(MainWindow::langua)
  settings.endGroup();
  
  /* Schrifteinstellungen laden */
  settings.beginGroup(QString("font"));
  
  m_fontName->setEditText(settings.value(QString("name"), QString("Monospace")).toString());
  m_fontSize->setValue(settings.value(QString("size"), 10).toInt());
  
  QColor fontColor = settings.value(QString("color"), Qt::black).value<QColor>();
  m_buttonFontColor->setColor(fontColor);
  
  settings.endGroup();
  
  /* Tabulatoreinstellungen laden */
  settings.beginGroup(QString("tabulator"));
  m_tabulatorWidth->setValue(settings.value(QString("width"), 2).toInt());
  settings.endGroup();
  
  /* Erstellen-Einstellungen laden */
  settings.beginGroup(QString("compiler"));
  m_latexCommandInput->setText(settings.value(QString("latex"), QString("latex")).toString());
  m_pdflatexCommandInput->setText(settings.value(QString("pdflatex"), QString("pdflatex")).toString());
  settings.endGroup();
  
  /* Synatx-Highlighting-Einstellungen laden */
  settings.beginGroup(QString("syntaxhighlighting"));
  m_buttonCommentForeground->setColor(settings.value(QString("commentForeground"), Qt::red).value<QColor>());
  m_buttonCommentBackground->setColor(settings.value(QString("commentBackground"), Qt::white).value<QColor>());
  int state = settings.value(QString("commentBold"), Qt::Unchecked).toInt();
  if (state == 0) {
    m_buttonCommentBold->setCheckState(Qt::Unchecked);
  } else {
    m_buttonCommentBold->setCheckState(Qt::Checked);
  }
  state = settings.value(QString("commentItalic"), Qt::Checked).toInt();
  if (state == 0) {
    m_buttonCommentItalic->setCheckState(Qt::Unchecked);
  } else {
    m_buttonCommentItalic->setCheckState(Qt::Checked);
  }
  
  m_buttonLatexForeground->setColor(settings.value(QString("latexForeground"), Qt::darkMagenta).value<QColor>());
  m_buttonLatexBackground->setColor(settings.value(QString("latexBackground"), Qt::white).value<QColor>());
  state = settings.value(QString("latexBold"), Qt::Checked).toInt();
  if (state == 0) {
    m_buttonLatexBold->setCheckState(Qt::Unchecked);
  } else {
    m_buttonLatexBold->setCheckState(Qt::Checked);
  }
  state = settings.value(QString("latexItalic"), Qt::Unchecked).toInt();
  if (state == 0) {
    m_buttonLatexItalic->setCheckState(Qt::Unchecked);
  } else {
    m_buttonLatexItalic->setCheckState(Qt::Checked);
  }
  
  m_buttonMathForeground->setColor(settings.value(QString("mathForeground"), Qt::black).value<QColor>());
  m_buttonMathBackground->setColor(settings.value(QString("mathBackground"), Qt::yellow).value<QColor>());
  state = settings.value(QString("mathBold"), Qt::Unchecked).toInt();
  if (state == 0) {
    m_buttonMathBold->setCheckState(Qt::Unchecked);
  } else {
    m_buttonMathBold->setCheckState(Qt::Checked);
  }
  state = settings.value(QString("mathItalic"), Qt::Unchecked).toInt();
  if (state == 0) {
    m_buttonMathItalic->setCheckState(Qt::Unchecked);
  } else {
    m_buttonMathItalic->setCheckState(Qt::Checked);
  }
  settings.endGroup();
}

void SettingDialog::slotLanguageChanged() {
  if (!m_languageChanged) {
    QMessageBox::information(this, tr("Language"), tr("You need to restart in order to use a new language."));
  }
  
  m_languageChanged = true;
}

void SettingDialog::slotSaveSettings() {
  QString latex = m_latexCommandInput->text();
  if (!Compiler::checkCommand(latex)) {
    return;
  }
  
  QString pdflatex = m_pdflatexCommandInput->text();
  if (!Compiler::checkCommand(pdflatex)) {
    return;
  }
  
  QColor fontColor = m_buttonFontColor->getColor();
  QSettings settings("QteX", "QteX");
  
  settings.beginGroup(QString("editor"));
  
  QString lang = m_languages->currentText();
  lang = lang.mid(lang.lastIndexOf(")") - 2, 2);
  settings.setValue(QString("language"), lang);
  
  QString codec = m_codecs->currentText();
  settings.setValue(QString("encoding"), codec);
  
  settings.endGroup();
  
  /* Schrifteinstellungen */
  settings.beginGroup(QString("font"));
  settings.setValue(QString("name"), m_fontName->currentText());
  settings.setValue(QString("size"), m_fontSize->value());
  settings.setValue(QString("color"), fontColor);
  settings.endGroup();
  
  /* Tabulatoreinstellungen */
  settings.beginGroup(QString("tabulator"));
  settings.setValue(QString("width"), m_tabulatorWidth->value());
  settings.endGroup();
  
  /* Erstellen-Einstellungen */
  settings.beginGroup(QString("compiler"));
  settings.setValue(QString("latex"), latex);
  settings.setValue(QString("pdflatex"), pdflatex);
  settings.endGroup();
  
  /* Synatxhighlighting-Einstellungen */
  settings.beginGroup(QString("syntaxhighlighting"));
  settings.setValue(QString("commentForeground"), m_buttonCommentForeground->getColor());
  settings.setValue(QString("commentBackground"), m_buttonCommentBackground->getColor());
  settings.setValue(QString("commentBold"), m_buttonCommentBold->checkState());
  settings.setValue(QString("commentItalic"), m_buttonCommentItalic->checkState());
  
  settings.setValue(QString("latexForeground"), m_buttonLatexForeground->getColor());
  settings.setValue(QString("latexBackground"), m_buttonLatexBackground->getColor());
  settings.setValue(QString("latexBold"), m_buttonLatexBold->checkState());
  settings.setValue(QString("latexItalic"), m_buttonLatexItalic->checkState());
  
  settings.setValue(QString("mathForeground"), m_buttonMathForeground->getColor());
  settings.setValue(QString("mathBackground"), m_buttonMathBackground->getColor());
  settings.setValue(QString("mathBold"), m_buttonMathBold->checkState());
  settings.setValue(QString("mathItalic"), m_buttonMathItalic->checkState());
  settings.endGroup();
  
  accept();
}
