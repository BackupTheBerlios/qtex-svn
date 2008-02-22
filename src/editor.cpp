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
  setObjectName(QString("input"));
  cursor = new QTextCursor(textCursor());
  setTextCursor(*cursor);
  
	highlighter = new Highlighter(document());
  changeState = false;
  
  QObject::connect(this, SIGNAL(textChanged()), this, SLOT(changed()));
  QObject::connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(setCopy(bool)));
  QObject::connect(this, SIGNAL(redoAvailable(bool)), this, SLOT(setRedo(bool)));
  QObject::connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(setUndo(bool)));
  
  loadSettings();
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

void Editor::loadSettings() {
  QSettings settings("QteX", "QteX");
  
  /* Schrifteigenschaften setzen */
  settings.beginGroup(QString("font"));
  QString fontname = settings.value(QString("name"), QString("Monospace")).toString();
  int fontsize = settings.value(QString("size"), 10).toInt();
  QColor fontcolor = settings.value(QString("color"), Qt::black).value<QColor>();
  settings.endGroup();
  
  setFont(QFont(fontname, fontsize));
  
  /* !!!!!!!!!!!!!!!!!!!!!!!!!!!
   * !!! ACHTUNG: Workaround !!!
   * !!!!!!!!!!!!!!!!!!!!!!!!!!!
   */
  bool tmp = changeState;
  if (toPlainText().isEmpty()) {
    setTextColor(fontcolor);
  } else {
    QTextCursor tmp = textCursor();
    selectAll();
    setTextColor(fontcolor);
    setTextCursor(tmp);
  }
  setChanged(tmp);
  
  /* Tabulatoreigenschaften setzen */
  settings.beginGroup(QString("tabulator"));
  int width = settings.value(QString("width"), 14).toInt();
  settings.endGroup();
  
  QFontMetrics fm(font());
  setTabStopWidth(width * fm.width(QString(" ")));
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
  
  QString title = trUtf8("Dokument schließen");
  
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

bool Editor::openDocument(QString filename) {
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("recent"));
  
  if (filename.isEmpty() || filename.isNull()) {
    /* Letzten Pfad laden */
    QString path = settings.value(QString("openPath"), QDir::homePath()).toString();
    filename = QFileDialog::getOpenFileName(this->parentWidget(), trUtf8("Datei öffnen"), path);
  }
  
  if (filename.isEmpty() || filename.isNull()) {
    return false;
  }
  
  /* Pfad speichern */
  settings.setValue(QString("openPath"), QFileInfo(filename).absolutePath());
  settings.endGroup();
  
  QFile file(filename);
  if (!file.exists()) {
    QMessageBox::critical(this, trUtf8("Fehler"), trUtf8("Die angeforderte Datei existiert nicht!"));
    return false;
  }
  
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::critical(this, trUtf8("Fehler"), trUtf8("Die angeforderte Datei konnte nicht geöffnet werden!"));
    return false;
  }
  
  QString allData;
  while (!file.atEnd()) {
    QByteArray line = file.readLine();
    allData += QString::fromLatin1(line);
  }
  
  file.close();
  
  setFilename(filename);
  setText(allData);
  
  setChanged(false);
  
  return true;
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
    QMessageBox::critical(this, trUtf8("Fehler"), trUtf8("Die aktuelle Datei ist nicht schreibbar!"));
    return false;
  }
  
  /* Text holen und in char * konvertieren */
  QByteArray data = toPlainText().toLatin1();
  
  /* Versuchen, die Daten zu schreiben */
  if (file.write(data) == -1) {
    QMessageBox::critical(this, trUtf8("Fehler"), trUtf8("Der Schreibvorgang für die aktuelle Datei war nicht erfolgreich!"));
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
  QSettings settings("QteX", "QteX");
  QString filename;
  
  settings.beginGroup(QString("recent"));
  QString path = settings.value(QString("savePath"), QDir::homePath()).toString();
  
  while (true) {
    /* Zunaechst einen Dateinamen holen */
    QFileDialog dlg(this->parentWidget(), trUtf8("Datei speichern"), path);
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setConfirmOverwrite(false);
    
    /* Dialog abgebrochen? Dann nicht speichern */
    if (dlg.exec() == QFileDialog::Rejected) {
      return false;
    }
    
    /* Keine Datei selektiert? Dann nicht speichern */
    if (dlg.selectedFiles().size() == 0) {
      return false;
    }
    
    /* Selektierte Datei holen und QFile erzeugen */
    filename = dlg.selectedFiles().at(0);
    QFile temp(filename);
    
    /* Pfad speichern */
    settings.setValue(QString("savePath"), QFileInfo(filename).absolutePath());
    
    /* Wurde eine andere, bereits existierende Datei gewählt? Dann nachfragen */
    if (temp.exists() && this->filename != filename) {
      QString text("Eine Datei namens '");
      text += filename.mid(filename.lastIndexOf(QDir::separator()) + 1);
      text += trUtf8("' existiert bereits. Überschreiben?");
      
      /* Überschreiben? Wenn ja: raus aus der Schleife. Sonst von vorne */
      int ret = QMessageBox::question(this, trUtf8("Datei speichern"), text, QMessageBox::Yes | QMessageBox::No);
      if (ret == QMessageBox::Yes) {
        break;
      } else {
        continue;
      }
    } else {
      break;
    }
  }
  
  settings.endGroup();

  /* Der gewählte Dateiname muss nichtleer sein */
  if (filename.isEmpty() || filename.isNull()) {
    return false;
  }
  
  /* Die Datei muss schreibbar sein */
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, trUtf8("Fehler"), trUtf8("Die aktuelle Datei ist nicht schreibbar!"));
    return false;
  }
  
  setFilename(filename);
  
  /* Text holen und nach char * konvertieren */
  QByteArray data = toPlainText().toLatin1();
  
  /* Versuchen, die Daten zu schreiben */
  if (file.write(data) == -1) {
    QMessageBox::critical(this, trUtf8("Fehler"), trUtf8("Der Schreibvorgang für die aktuelle Datei war nicht erfolgreich!"));
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

