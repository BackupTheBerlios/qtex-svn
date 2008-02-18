/*
 *  editor.cpp
 *  qtex
 *
 *  Created by Michel Steuwer on 18.02.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <QFileDialog>
#include "editor.h"

Editor::Editor( QWidget * parent )
: QTextEdit(parent)
{
	highlighter = new Highlighter(document());
  changeState = false;
  
  QObject::connect(this, SIGNAL(textChanged()), this, SLOT(changed()));
}

void Editor::changed() {
  setChanged(true);
}

QString Editor::getFilename() {
  return filename;
}

bool Editor::hasChanged() {
  return changeState;
}

void Editor::save() {
  if (getFilename().isEmpty() || getFilename().isNull()) {
    saveAs();
    return;
  }
  
  /* hier speichern implementieren */
}

void Editor::saveAs() {
  QString filename = QFileDialog::getSaveFileName(this->parentWidget(), QString::fromUtf8("Datei speichern - QteX"), QDir::homePath());
}

void Editor::setChanged(bool bChanged) {
  changeState = bChanged;
}

void Editor::setFilename(QString filename) {
  this->filename = filename;
}

