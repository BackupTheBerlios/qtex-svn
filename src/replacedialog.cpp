#include "replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) : QDialog(parent) {
  setModal(true);
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  setWindowTitle(trUtf8("Text ersetzen"));
  hide();
  
  createDialog();
  
  QObject::connect(close, SIGNAL(clicked()), this, SLOT(reject()));
  QObject::connect(replace, SIGNAL(clicked()), this, SLOT(replaceButtonClicked()));
  QObject::connect(searchTextInput, SIGNAL(textChanged(QString)), this, SLOT(toggleReplaceButton(QString)));
}

void ReplaceDialog::closeEvent(QCloseEvent event) {
  event.ignore();
  hide();
}

void ReplaceDialog::createDialog() {
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
  
  /* Ersatztext */
  QLabel *labelReplaceText = new QLabel(this);
  labelReplaceText->setText(trUtf8("Ersetzen durch:"));
  
  replaceTextInput = new QLineEdit(this);
  replaceTextInput->setFocus();
  replaceTextInput->setObjectName(QString("replaceTextInput"));
  replaceTextInput->setText(replaceText);
  
  /* Flags */
  QGroupBox *groupFlags = new QGroupBox(this);
  groupFlags->setTitle(trUtf8("Einstellungen"));
  QVBoxLayout *groupFlagsLayout = new QVBoxLayout(groupFlags);
  groupFlags->setLayout(groupFlagsLayout);
  
  caseSensitive = new QCheckBox(groupFlags);
  caseSensitive->setObjectName(QString("caseSensitive"));
  caseSensitive->setText(trUtf8("Groß- und Kleinschreibung unterscheiden"));
  groupFlagsLayout->addWidget(caseSensitive);
  
  wholeWords = new QCheckBox(groupFlags);
  wholeWords->setObjectName(QString("wholeWord"));
  wholeWords->setText(trUtf8("nur ganze Wörter"));
  groupFlagsLayout->addWidget(wholeWords);
  
  backward = new QCheckBox(groupFlags);
  backward->setObjectName(QString("backward"));
  backward->setText(trUtf8("rückwärts suchen"));
  groupFlagsLayout->addWidget(backward);
  
  promptOnReplace = new QCheckBox(groupFlags);
  promptOnReplace->setObjectName(QString("promptOnReplace"));
  promptOnReplace->setText(trUtf8("vor dem Ersetzen nachfragen"));
  groupFlagsLayout->addWidget(promptOnReplace);
  
  /* Buttons */
  replace = new QPushButton(this);
  replace->setEnabled(false);
  replace->setObjectName(QString("replace"));
  replace->setText(trUtf8("Ersetzen"));
  
  close = new QPushButton(this);
  close->setObjectName(QString("close"));
  close->setText(trUtf8("Schließen"));
  
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch();
  buttonLayout->addWidget(replace);
  buttonLayout->addWidget(close);
  
  /* zusammenbauen */
  layout->addWidget(labelSearchText);
  layout->addWidget(searchTextInput);
  layout->addWidget(labelReplaceText);
  layout->addWidget(replaceTextInput);
  layout->addWidget(groupFlags);
  layout->addLayout(buttonLayout);
}

int ReplaceDialog::exec() {
  searchTextInput->setFocus();
  searchTextInput->selectAll();
  return QDialog::exec();
}

bool ReplaceDialog::getPromptOnReplace() {
  return promptOnReplace->isChecked();
}

QString ReplaceDialog::getReplacementText() {
  return replaceText;
}

QTextDocument::FindFlags ReplaceDialog::getSearchFlags() {
  QTextDocument::FindFlags flags = 0;
  
  if (caseSensitive->isChecked()) {
    flags |= QTextDocument::FindCaseSensitively;
  }
  
  if (wholeWords->isChecked()) {
    flags |= QTextDocument::FindWholeWords;
  }
  
  if (backward->isChecked()) {
    flags |= QTextDocument::FindBackward;
  }
  
  return flags;
}

QString ReplaceDialog::getSearchText() {
  return searchText;
}

void ReplaceDialog::replaceButtonClicked() {
  replaceText = replaceTextInput->text();
  searchText = searchTextInput->text();
  accept();
}

void ReplaceDialog::toggleReplaceButton(QString text) {
  if (text.isEmpty() || text.isNull()) {
    replace->setEnabled(false);
  } else {
    replace->setEnabled(true);
  }
}

