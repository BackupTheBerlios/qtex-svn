#include "replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) : QDialog(parent) {
  setModal(true);
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  setWindowTitle(tr("Replace"));
  hide();
  
  createDialog();
  
  connect(m_close, SIGNAL(clicked()), this, SLOT(reject()));
  connect(m_replace, SIGNAL(clicked()), this, SLOT(slotReplaceButtonClicked()));
  connect(m_searchTextInput, SIGNAL(textChanged(QString)), this, SLOT(slotToggleReplaceButton(QString)));
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
  labelSearchText->setText(tr("Search term:"));
  
  m_searchTextInput = new QLineEdit(this);
  m_searchTextInput->setFocus();
  m_searchTextInput->setObjectName(QString("m_searchTextInput"));
  m_searchTextInput->setText(m_searchText);
  m_searchTextInput->selectAll();
  
  /* Ersatztext */
  QLabel *labelReplaceText = new QLabel(this);
  labelReplaceText->setText(tr("Replacement:"));
  
  m_replaceTextInput = new QLineEdit(this);
  m_replaceTextInput->setFocus();
  m_replaceTextInput->setObjectName(QString("m_replaceTextInput"));
  m_replaceTextInput->setText(m_replaceText);
  
  /* Flags */
  QGroupBox *groupFlags = new QGroupBox(this);
  groupFlags->setTitle(tr("Options"));
  QVBoxLayout *groupFlagsLayout = new QVBoxLayout(groupFlags);
  groupFlags->setLayout(groupFlagsLayout);
  
  m_caseSensitive = new QCheckBox(groupFlags);
  m_caseSensitive->setObjectName(QString("m_caseSensitive"));
  m_caseSensitive->setText(tr("Case sensitive"));
  groupFlagsLayout->addWidget(m_caseSensitive);
  
  m_wholeWords = new QCheckBox(groupFlags);
  m_wholeWords->setObjectName(QString("m_wholeWord"));
  m_wholeWords->setText(tr("Whole words"));
  groupFlagsLayout->addWidget(m_wholeWords);
  
  m_backward = new QCheckBox(groupFlags);
  m_backward->setObjectName(QString("m_backward"));
  m_backward->setText(tr("Find backwards"));
  groupFlagsLayout->addWidget(m_backward);
  
  m_promptOnReplace = new QCheckBox(groupFlags);
  m_promptOnReplace->setObjectName(QString("m_promptOnReplace"));
  m_promptOnReplace->setText(tr("Prompt on replace"));
  groupFlagsLayout->addWidget(m_promptOnReplace);
  
  /* Buttons */
  m_replace = new QPushButton(this);
  m_replace->setEnabled(false);
  m_replace->setObjectName(QString("m_replace"));
  m_replace->setText(tr("Replace"));
  
  m_close = new QPushButton(this);
  m_close->setObjectName(QString("m_close"));
  m_close->setText(tr("Close"));
  
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch();
  buttonLayout->addWidget(m_replace);
  buttonLayout->addWidget(m_close);
  
  /* zusammenbauen */
  layout->addWidget(labelSearchText);
  layout->addWidget(m_searchTextInput);
  layout->addWidget(labelReplaceText);
  layout->addWidget(m_replaceTextInput);
  layout->addWidget(groupFlags);
  layout->addLayout(buttonLayout);
}

bool ReplaceDialog::getPromptOnReplace() {
  return m_promptOnReplace->isChecked();
}

QString ReplaceDialog::getReplacementText() {
  return m_replaceText;
}

QTextDocument::FindFlags ReplaceDialog::getSearchFlags() {
  QTextDocument::FindFlags flags = 0;
  
  if (m_caseSensitive->isChecked()) {
    flags |= QTextDocument::FindCaseSensitively;
  }
  
  if (m_wholeWords->isChecked()) {
    flags |= QTextDocument::FindWholeWords;
  }
  
  if (m_backward->isChecked()) {
    flags |= QTextDocument::FindBackward;
  }
  
  return flags;
}

QString ReplaceDialog::getSearchText() {
  return m_searchText;
}

int ReplaceDialog::slotExec() {
  m_searchTextInput->setFocus();
  m_searchTextInput->selectAll();
  return QDialog::exec();
}

void ReplaceDialog::slotReplaceButtonClicked() {
  m_replaceText = m_replaceTextInput->text();
  m_searchText = m_searchTextInput->text();
  accept();
}

void ReplaceDialog::slotToggleReplaceButton(QString text) {
  if (text.isEmpty() || text.isNull()) {
    m_replace->setEnabled(false);
  } else {
    m_replace->setEnabled(true);
  }
}

