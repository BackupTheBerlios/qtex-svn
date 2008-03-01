#include "finddialog.h"

FindDialog::FindDialog(QWidget *parent) : QDialog(parent) {
  setModal(true);
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  setWindowTitle(trUtf8("Text suchen"));
  hide();
  
  createDialog();
  
  QObject::connect(close, SIGNAL(clicked()), this, SLOT(reject()));
  QObject::connect(search, SIGNAL(clicked()), this, SLOT(searchButtonClicked()));
  QObject::connect(searchTextInput, SIGNAL(textChanged(QString)), this, SLOT(toggleSearchButton(QString)));
}

void FindDialog::closeEvent(QCloseEvent event) {
  event.ignore();
  hide();
}

void FindDialog::createDialog() {
  QVBoxLayout *layout = new QVBoxLayout(this);
  setLayout(layout);
  
  /* Suchtext */
  QLabel *labelSearchText = new QLabel(this);
  labelSearchText->setText(trUtf8("Suchen nach:"));
  
  searchTextInput = new QLineEdit(this);
  searchTextInput->setFocus();
  searchTextInput->setObjectName(QString("searchTextInput"));
  searchTextInput->setText(searchText);
  searchTextInput->selectAll();
  
  /* Flags */
  QGroupBox *groupFlags = new QGroupBox(this);
  groupFlags->setTitle(trUtf8("Sucheinstellungen"));
  QHBoxLayout *groupFlagsLayout = new QHBoxLayout(groupFlags);
  groupFlags->setLayout(groupFlagsLayout);
  
  caseSensitive = new QCheckBox(groupFlags);
  caseSensitive->setObjectName(QString("caseSensitive"));
  caseSensitive->setText(trUtf8("Groß- und Kleinschreibung unterscheiden"));
  groupFlagsLayout->addWidget(caseSensitive);
  
  wholeWords = new QCheckBox(groupFlags);
  wholeWords->setObjectName(QString("wholeWord"));
  wholeWords->setText(trUtf8("nur ganze Wörter"));
  groupFlagsLayout->addWidget(wholeWords);
  
  /* Buttons */
  search = new QPushButton(this);
  search->setEnabled(false);
  search->setObjectName(QString("search"));
  search->setText(trUtf8("Suchen"));
  
  close = new QPushButton(this);
  close->setObjectName(QString("close"));
  close->setText(trUtf8("Schließen"));
  
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch();
  buttonLayout->addWidget(search);
  buttonLayout->addWidget(close);
  
  /* zusammenbauen */
  layout->addWidget(labelSearchText);
  layout->addWidget(searchTextInput);
  layout->addWidget(groupFlags);
  layout->addLayout(buttonLayout);
}

int FindDialog::exec() {
  searchTextInput->setFocus();
  searchTextInput->selectAll();
  return QDialog::exec();
}

QTextDocument::FindFlags FindDialog::getSearchFlags() {
  QTextDocument::FindFlags flags = 0;
  
  if (caseSensitive->isChecked()) {
    flags |= QTextDocument::FindCaseSensitively;
  }
  
  if (wholeWords->isChecked()) {
    flags |= QTextDocument::FindWholeWords;
  }
  
  return flags;
}

QString FindDialog::getSearchText() {
  return searchText;
}

void FindDialog::searchButtonClicked() {
  searchText = searchTextInput->text();
  accept();
}

void FindDialog::toggleSearchButton(QString text) {
  if (text.isEmpty() || text.isNull()) {
    search->setEnabled(false);
  } else {
    search->setEnabled(true);
  }
}
