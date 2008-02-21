#include "settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) : QDialog(parent) {
  createDialog();
  createConnections();
  
  qRegisterMetaType<QColor>("QColor");
  loadSettings();
}

void SettingDialog::createDialog() {
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
  groupFontLayout->addWidget(fontSize, 0, 2);
  
  QLabel *labelFontColor = new QLabel(groupFont);
  labelFontColor->setObjectName(QString("labelFontColor"));
  labelFontColor->setText(trUtf8("Schriftfarbe:"));
  groupFontLayout->addWidget(labelFontColor, 1, 0);
  
  buttonFontColor = new QPushButton(groupFont);
  buttonFontColor->setIconSize(QSize(16, 16));
  buttonFontColor->setMaximumSize(QSize(24, 24));
  buttonFontColor->setObjectName(QString("buttonFontColor"));
  groupFontLayout->addWidget(buttonFontColor, 1, 1);
  
  fontColorPixmap = new QPixmap(16, 16);
  fontColorPixmap->fill(fontColor);
  buttonFontColor->setIcon(QIcon(*fontColorPixmap));
  
  editorLayout->addWidget(groupFont);
  editorLayout->addStretch();
  
  tabs->addTab(editorTab, trUtf8("Editor"));
}

void SettingDialog::createConnections() {
  QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
  QObject::connect(save, SIGNAL(clicked()), this, SLOT(saveSettings()));
  
  QObject::connect(buttonFontColor, SIGNAL(clicked()), this, SLOT(getFontColor()));
}

void SettingDialog::getFontColor() {
  QColor newColor = QColorDialog::getColor(fontColor);
  if (!newColor.isValid()) {
    return;
  }
  
  fontColor = newColor;
  fontColorPixmap->fill(fontColor);
  buttonFontColor->setIcon(QIcon(*fontColorPixmap));
}

void SettingDialog::loadSettings() {
  QSettings settings("QteX", "QteX");
  
  /* Schrifteinstellungen laden */
  settings.beginGroup(QString("font"));
  
  fontName->setEditText(settings.value("name", QString("Monospace")).toString());
  fontSize->setValue(settings.value("size", 10).toInt());
  
  fontColor = settings.value("color", Qt::black).value<QColor>();
  fontColorPixmap->fill(fontColor);
  buttonFontColor->setIcon(QIcon(*fontColorPixmap));
  
  settings.endGroup();
}

void SettingDialog::saveSettings() {
  QSettings settings("QteX", "QteX");
  
  /* Schrifteinstellungen */
  settings.beginGroup(QString("font"));
  settings.setValue(QString("name"), fontName->currentText());
  settings.setValue(QString("size"), fontSize->value());
  settings.setValue(QString("color"), fontColor);
  settings.endGroup();
  
  accept();
}
