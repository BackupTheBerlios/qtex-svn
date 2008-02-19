/*
 *  editor.cpp
 *  qtex
 *
 *  Created by Michel Steuwer on 18.02.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <QFileDialog>
#include <QMessageBox>
#include "editor.h"

Editor::Editor( QWidget * parent )
: QTextEdit(parent)
{
	highlighter = new Highlighter(document());
  changeState = false;
  
  QObject::connect(this, SIGNAL(textChanged()), this, SLOT(changed()));
  QObject::connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(setCopy(bool)));
  QObject::connect(this, SIGNAL(redoAvailable(bool)), this, SLOT(setRedo(bool)));
  QObject::connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(setUndo(bool)));
}

/*
 * Interne Methode, um den Veränderungsstatus auf true zu setzen.
 */
void Editor::changed() {
  setChanged(true);
}

/* ============================
 * == Einige getter-Methoden ==
 * ============================
 */

bool Editor::getCopy() {
  return canCopy;
}

QString Editor::getFilename() {
  return filename;
}

bool Editor::getRedo() {
  return canRedo;
}

bool Editor::getUndo() {
  return canUndo;
}

bool Editor::hasChanged() {
  return changeState;
}

/*
 * Hat sich das Dokument seit dem letzten Speichervorgang
 * verändert, so wird abgefragt, ob es erneut gesichert
 * werden soll oder nicht. Der Rückgabewert gibt an, ob
 * die Abfrage abgebrochen wurde (false) oder nicht (true).
 */
bool Editor::maybeSave() {
  if (!hasChanged()) {
    return true;
  }
  
  QString title = trUtf8("Dokument schließen - QteX");
  
  QString text = trUtf8("Das Dokument '");
  if (getFilename().isEmpty() || getFilename().isNull()) {
    text += trUtf8("Unbenannt");
  } else {
    text += getFilename().mid(getFilename().lastIndexOf(QDir::separator()) + 1);
  }
  text += trUtf8("' wurde geändert.\n\nMöchten Sie die Änderungen speichern oder verwerfen?");
  
  int ret = QMessageBox::question(this, title, text, QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
  if (ret == QMessageBox::Cancel) {
    return false;
  } else if (ret == QMessageBox::Discard) {
    return true;
  } else {
    if (save() == false) {
      return false;
    }
    return true;
  }
}

/*
 * Versucht, das aktuelle Dokument abzuspeichern. Der Rückgabewert
 * gibt Auskunft, ob das Speichern erfolgreich war (true) oder fehl-
 * geschlagen ist bzw. abgebrochen wurde (false)
 */
bool Editor::save() {
  /* Wenn kein Dateiname angegeben ist, wurde noch nicht gespeichert */
  if (getFilename().isEmpty() || getFilename().isNull()) {
    return saveAs();
  }
  
  /* Die Datei muss schreibbar sein */
  QFile file(getFilename());
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, trUtf8("Fehler - QteX"), trUtf8("Die aktuelle Datei ist nicht schreibbar!"));
    return false;
  }
  
  /* Text holen und in char * konvertieren */
  QString allData = toPlainText();
  char *buffer = allData.toUtf8().data();
  
  /* Versuchen, die Daten zu schreiben */
  if (file.write(buffer, qstrlen(buffer)) == -1) {
    QMessageBox::critical(this, trUtf8("Fehler - QteX"), trUtf8("Der Schreibvorgang für die aktuelle Datei war nicht erfolgreich!"));
    return false;
  }
  
  file.close();  
  changeState = false;
  
  return true;
}

/*
 * Wie Editor::save(), jedoch mit einem Dialog zum Auswählen
 * des Speicherpfades.
 */
bool Editor::saveAs() {
  /* Zunaechst einen Dateinamen holen */
  QString filename = QFileDialog::getSaveFileName(this->parentWidget(), trUtf8("Datei speichern - QteX"), QDir::homePath());
  
  /* Dieser muss nichtleer sein */
  if (filename.isEmpty() || filename.isNull()) {
    return false;
  }
  
  /* Die Datei muss schreibbar sein */
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, trUtf8("Fehler - QteX"), trUtf8("Die aktuelle Datei ist nicht schreibbar!"));
    return false;
  }
  
  setFilename(filename);
  
  /* Text holen und nach char * konvertieren */
  QString allData = toPlainText();
  char *buffer = allData.toUtf8().data();
  
  /* Versuchen, die Daten zu schreiben */
  if (file.write(buffer, qstrlen(buffer)) == -1) {
    QMessageBox::critical(this, trUtf8("Fehler - QteX"), trUtf8("Der Schreibvorgang für die aktuelle Datei war nicht erfolgreich!"));
    return false;
  }
  
  file.close();
  changeState = false;
  
  return true;
}

/* ============================
 * == Einige setter-Methoden ==
 * ============================
 */
void Editor::setChanged(bool bChanged) {
  changeState = bChanged;
}

void Editor::setCopy(bool bCopy) {
  canCopy = bCopy;
}

void Editor::setFilename(QString filename) {
  this->filename = filename;
}

void Editor::setRedo(bool bRedo) {
  canRedo = bRedo;
}

void Editor::setUndo(bool bUndo) {
  canUndo = bUndo;
}

