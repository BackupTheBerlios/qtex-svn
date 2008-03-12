#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
  setModal(true);
  setWindowTitle(tr("About QteX"));
  
  createDialog();
  
  //setFixedSize(size());
}

void AboutDialog::createDialog() {
  QHBoxLayout *layout = new QHBoxLayout();
  layout->setSpacing(10);
  setLayout(layout);
  
  QLabel *labelIcon = new QLabel(this);
  labelIcon->setPixmap(QPixmap(":/images/qtexlogo.png"));
  
  QLabel *labelText = new QLabel(this);
  labelText->setWordWrap(true);
  labelText->setText(tr("<b>QteX 0.2</b><br /><br />QteX is a LaTeX-Editor, written in C++ using Qt. QteX is open source under the terms of the GPL v2, i.e. the source code may be viewed, used, changed and published by anyone, as long as the result is provided under the terms of the GPL v2 too.<br /><br /><br /><b>Developers:</b><br /><br />Sebastian Albers<br />Michael Schaefer<br />Michel Steuwer<br /><br /> Copyright &copy; 2008"));
  
  layout->addWidget(labelIcon, 0, Qt::AlignTop);
  layout->addWidget(labelText, 0, Qt::AlignTop);
}
