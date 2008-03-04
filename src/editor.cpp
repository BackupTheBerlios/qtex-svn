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
  lineMargin = 60;
  
  setObjectName(QString("input"));
  cursor = new QTextCursor(textCursor());
  setTextCursor(*cursor);
  setViewportMargins(lineMargin, 0, 0, 0);
  
	highlighter = new Highlighter(document());
  changeState = false;
  
  QObject::connect(this, SIGNAL(textChanged()), this, SLOT(changed()));
  QObject::connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(setCopy(bool)));
  QObject::connect(this, SIGNAL(redoAvailable(bool)), this, SLOT(setRedo(bool)));
  QObject::connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(setUndo(bool)));
  
  QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(dummy()));
  
  QObject::connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(update())); 
  QObject::connect(this, SIGNAL(textChanged()), this, SLOT(update())); 
  
  loadSettings();
}

/*
 * Interne Methode, um den Veränderungsstatus auf true zu setzen.
 */
void Editor::changed() {
  setChanged(true);
}

void Editor::dummy() {
  int index = textCursor().position();
  QString text = toPlainText().mid(0, index);
  int lastIndex = text.lastIndexOf(QString("\n"));
  
  int line = text.count(QString("\n")) + 1;
  int col = index - lastIndex;
  
  emit newCursorPosition(trUtf8("Zeile ") + QString::number(line) + trUtf8(", Spalte ") + QString::number(col)); 
}

/*
 * Sorgt für das Anzeigen der Zeilennummern
 */
bool Editor::event(QEvent *event) {
  if (event->type() == QEvent::Paint) {
    QPainter painter(this);
    painter.fillRect(0, 0, lineMargin, height(), QColor(255, 255, 210));
    
    int contentsY = verticalScrollBar()->value(); 
    qreal pageBottom = contentsY + viewport()->height(); 
    int lineNumber = 1; 
    QFontMetrics fm = fontMetrics(); 
    int ascent = fontMetrics().ascent() + 1; 
       
    for (QTextBlock block = document()->begin(); block.isValid(); block = block.next(), lineNumber++) { 
      QTextLayout *layout = block.layout(); 
      const QRectF boundingRect = layout->boundingRect(); 
      QPointF position = layout->position(); 
      if ( position.y() + boundingRect.height() < contentsY ) { 
        continue; 
      } 
      
      if ( position.y() > pageBottom ) {
        lineNumber++; 
        break; 
      }
      
      QString str = QString::number(lineNumber); 
      painter.drawText(lineMargin - fm.width(str) - 5, qRound(position.y()) - contentsY + ascent + 1, str); 
    }
  }
  
  return QTextEdit::event(event);
}

bool Editor::find(QString exp, QTextDocument::FindFlags flags) {
  int cursorPos = textCursor().position();
  if (QTextEdit::find(exp, flags)) {
    return true;
  }
  
  if (flags & QTextDocument::FindBackward) {
    if (cursorPos < toPlainText().length()) {
      int ret = QMessageBox::question(0, trUtf8("Text suchen"), trUtf8("Der Anfang des Textes wurde erreicht. Suche am Ende fortsetzen?"), QMessageBox::Yes | QMessageBox::No);
      if (ret == QMessageBox::Yes) {
        setCursorPos(toPlainText().length());
      } else {
        return false;
      }
    } else {
      QMessageBox::information(this, trUtf8("Text suchen"), trUtf8("Der Suchbegriff '") + exp + trUtf8("' wurde nicht gefunden!"));
      return false;
    }
  } else {
    if (cursorPos > 0) {
      int ret = QMessageBox::question(0, trUtf8("Text suchen"), trUtf8("Das Ende des Textes wurde erreicht. Suche am Anfang fortsetzen?"), QMessageBox::Yes | QMessageBox::No);
      if (ret == QMessageBox::Yes) {
        setCursorPos(0);
      } else {
        return false;
      }
    } else {
      QMessageBox::information(this, trUtf8("Text suchen"), trUtf8("Der Suchbegriff '") + exp + trUtf8("' wurde nicht gefunden!"));
      return false;
    }
  }
  
  return find(exp, flags);
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

/*
 * Springt zur angegebenen Zeile des Eingabefeldes. Liefert true,
 * falls der Sprung erfolgreich ausgeführt wurde, ansonsten false
 * (etwa, wenn zu wenig Zeilen vorhanden sind).
 */
bool Editor::gotoLine(int line) {
  int lineCount = this->toPlainText().count(QString("\n")) + 1;
  if (lineCount < line) {
    return false;
  }
  
  int cursorPos = 0;
  QString str = toPlainText();
  for (; line > 1; cursorPos = str.indexOf(QString("\n"), cursorPos) + 1, line--) {
  }
  
  setCursorPos(cursorPos);
  
  return true;
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
  
  /* Highlighting erneuern */
  highlighter->loadHighlighting();
  highlighter->rehighlight();
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

void Editor::replace(QString exp, QString rep, QTextDocument::FindFlags flags, bool bPromptOnReplace) {
  QMessageBox prompt;
  prompt.setIcon(QMessageBox::Question);
  prompt.setWindowTitle(tr("Text ersetzen"));
  prompt.setText(trUtf8("Der Suchbegriff wurde gefunden. Was wollen Sie tun?"));
  prompt.addButton(trUtf8("Ersetzen"), QMessageBox::ActionRole);
  QPushButton *replaceAll = prompt.addButton(trUtf8("Alle ersetzen"), QMessageBox::ActionRole);
  QPushButton *findNext = prompt.addButton(trUtf8("Weitersuchen"), QMessageBox::ActionRole);
  QPushButton *close = prompt.addButton(QMessageBox::Close);
  
  int count = 0;
  while (find(exp, flags)) {
    count++;
    if (bPromptOnReplace) {
      prompt.exec();
      QAbstractButton *clickedButton = prompt.clickedButton();
      if (clickedButton == (QAbstractButton*)close) {
        break;
      } else if (clickedButton == (QAbstractButton*)findNext) {
        continue;
      } else if (clickedButton == (QAbstractButton*)replaceAll) {
        bPromptOnReplace = false;
      }
    }
    
    /* Ersetzung durchführen */
    this->textCursor().removeSelectedText();
    this->textCursor().insertText(rep);
  }
  
  QMessageBox::information(0, tr("Text ersetzen"), tr("Es wurden ") + QString::number(count) + trUtf8(" Ersetzungen durchgeführt."));
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

void Editor::setCursorPos(int pos) {
  QTextCursor tmp = textCursor();
  tmp.setPosition(pos);
  setTextCursor(tmp);
}


