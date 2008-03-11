#include "finddialog.h"

FindDialog::FindDialog(QWidget *parent) : QDialog(parent) {
  setModal(true);
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  setWindowTitle(tr("Find"));
  hide();
  
  createDialog();
  
  /* eigene Slots */
  connect(m_search, SIGNAL(clicked()), this, SLOT(slotSearchButtonClicked()));
  connect(m_searchTextInput, SIGNAL(textChanged(QString)), this, SLOT(slotToggleSearchButton(QString)));
  
  /* vorhandene Slots */
  connect(m_close, SIGNAL(clicked()), this, SLOT(reject()));
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
  labelSearchText->setText(tr("Search term:"));
  
  m_searchTextInput = new QLineEdit(this);
  m_searchTextInput->setFocus();
  m_searchTextInput->setObjectName(QString("m_searchTextInput"));
  m_searchTextInput->setText(m_searchText);
  m_searchTextInput->selectAll();
  
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
  
  /* Buttons */
  m_search = new QPushButton(this);
  m_search->setEnabled(false);
  m_search->setObjectName(QString("m_search"));
  m_search->setText(tr("Find"));
  
  m_close = new QPushButton(this);
  m_close->setObjectName(QString("m_close"));
  m_close->setText(tr("Close"));
  
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch();
  buttonLayout->addWidget(m_search);
  buttonLayout->addWidget(m_close);
  
  /* zusammenbauen */
  layout->addWidget(labelSearchText);
  layout->addWidget(m_searchTextInput);
  layout->addWidget(groupFlags);
  layout->addLayout(buttonLayout);
}

QTextDocument::FindFlags FindDialog::getSearchFlags() {
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

QString FindDialog::getSearchText() {
  return m_searchText;
}

int FindDialog::slotExec() {
  m_searchTextInput->setFocus();
  m_searchTextInput->selectAll();
  return QDialog::exec();
}

void FindDialog::slotSearchButtonClicked() {
  m_searchText = m_searchTextInput->text();
  accept();
}

void FindDialog::slotToggleSearchButton(QString text) {
  if (text.isEmpty() || text.isNull()) {
    m_search->setEnabled(false);
  } else {
    m_search->setEnabled(true);
  }
}
