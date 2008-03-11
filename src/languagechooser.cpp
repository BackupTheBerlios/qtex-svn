#include "languagechooser.h"

LanguageChooser::LanguageChooser(QWidget *parent) : QDialog(parent) {
  setModal(true);
  setWindowTitle(tr("Language"));
  
  createDialog();

  connect(m_cancel, SIGNAL(clicked()), this, SLOT(reject()));
  connect(m_select, SIGNAL(clicked()), this, SLOT(accept()));

  language = tr("English (en)");
}

void LanguageChooser::closeEvent(QCloseEvent *event) {
  event->ignore();
  reject();
}

void LanguageChooser::createDialog() {
  QVBoxLayout *layout = new QVBoxLayout(this);
  setLayout(layout);
  
  QLabel *labelLanguages = new QLabel(this);
  labelLanguages->setText(tr("Please select a language for the program:"));
  layout->addWidget(labelLanguages);
  
  m_languages = new QComboBox(this);
  m_languages->setObjectName(QString("m_languages"));
  m_languages->addItems(LanguageChooser::getLanguages());
  layout->addWidget(m_languages);
  
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  
  m_select = new QPushButton(this);
  m_select->setObjectName(QString("m_select"));
  m_select->setText(tr("Select"));
  
  m_cancel = new QPushButton(this);
  m_cancel->setObjectName(QString("m_cancel"));
  m_cancel->setText(tr("Cancel"));
  
  buttonLayout->addStretch();
  buttonLayout->addWidget(m_select);
  buttonLayout->addWidget(m_cancel);
  
  layout->addLayout(buttonLayout);
}

QString LanguageChooser::getCurrentLanguageCode() {
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("editor"));
  QString suffix = settings.value(QString("language"), "en").toString(); 
  settings.endGroup();
  
  return suffix;
}

QString LanguageChooser::getLanguageCode() {
  QString item = m_languages->currentText();
  item = item.mid(item.lastIndexOf(")") - 2, 2);
  
  return item;
}

QStringList LanguageChooser::getLanguages() {
  QStringList filter;
  filter << "*.qm";
  QDir dir(QCoreApplication::applicationDirPath());
  QStringList entries = dir.entryList(filter);
  
  QStringList files;
  QTranslator *t = new QTranslator(qApp);
  foreach (QString entry, entries) {
    if (!t->load(entry, dir.absolutePath())) {
      continue;
    }
    QString lang = t->translate("LanguageChooser", "English (en)");
    if (!lang.isEmpty() && !lang.isNull()) {
      files << lang;
    }
  }
  
  files << "English (en)";
  files.sort();
  
  return files;
}

void LanguageChooser::setLanguage(QString suffix) {
  if (suffix == "en") {
    return;
  }
  
  QString qtTranslatorFile = "qt_" + suffix;
  QString qtexTranslatorFile = "QteX_" + suffix;
  
  QTranslator *qtTranslator = new QTranslator(qApp);
  QTranslator *qtexTranslator = new QTranslator(qApp);
  
  if (qtTranslator->load(qtTranslatorFile, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
    qApp->installTranslator(qtTranslator);
  }
  
  if (qtexTranslator->load(qtexTranslatorFile, QCoreApplication::applicationDirPath())) {
    qApp->installTranslator(qtexTranslator);
  }
  
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("editor"));
  settings.setValue(QString("language"), suffix);
  settings.endGroup();
}

