#include "logconsole.h"

LogConsole::LogConsole(QWidget *parent) : QTextEdit(parent) {
  setReadOnly(true);
  
  m_errorFormat.setForeground(Qt::red);
  m_badBoxFormat.setForeground(Qt::darkGray);
}

void LogConsole::keyPressEvent(QKeyEvent *event) {
  event->ignore();
}

void LogConsole::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    QTextBlock block = cursorForPosition(event->pos()).block();
    if (block.isValid()) {
      QString text = block.text().trimmed();
      if (text.endsWith(tr(" (No line found)"))) {
        return;
      }
      
      int startIndex = text.indexOf(QString(":")) + 2, endIndex = text.indexOf(QString(":"), startIndex);
      int line = text.mid(startIndex, endIndex - startIndex).trimmed().toInt();
      m_editor->gotoLine(line);
    }
  }
}

void LogConsole::parse(QString cmd, QString str, int exitCode) {
  int errorCount = 0, badBoxCount = 0;
  
  QString file;
  if (m_editor != 0) {
    file = m_editor->getFilename();
    file = "." + file.mid(file.lastIndexOf(QDir::separator()));
  }
  
  clear();
  append("[" + cmd + "]" + tr(" Finished with exitcode ") + QString::number(exitCode));
  
  QTextDocument doc(str);
  for (QTextBlock block = doc.begin(); block.isValid(); block = block.next()) {
    QString text = block.text();
    
    if (text.startsWith(QString("!"))) {
      setCurrentCharFormat(m_errorFormat);
      append(file + ": " + parseError(block));
      errorCount++;
    } else if (text.startsWith(QString("Underfull")) || text.startsWith(QString("Overfull"))) {
      setCurrentCharFormat(m_badBoxFormat);
      append(file + ": " + parseBadbox(block));
      badBoxCount++;
    }
  }
  
  setCurrentCharFormat(m_normalFormat);
  QString endMessage;
  if (errorCount == 0) {
    endMessage = tr("Done. ") + QString::number(errorCount) + tr(" Error(s), ") + QString::number(badBoxCount) + tr(" BadBox(es).");
  } else {
    endMessage = QString::number(errorCount) + tr(" Error(s), ") + QString::number(badBoxCount) + tr(" BadBox(es).");
  }
  
  append(endMessage);
}

QString LogConsole::parseBadbox(QTextBlock block) {
  QString text = block.text().trimmed();
  int index = text.lastIndexOf(QString("at line")) ;
  
  QString message = text.mid(0, index - 1).trimmed();
  QString line = text.mid(text.indexOf(QString(" "), index + 4)).trimmed();
  if (line.contains(QString("--"))) {
    line = line.mid(0, line.indexOf(QString("--")));
  }
  
  return line + ": " + message;
}

QString LogConsole::parseError(QTextBlock block) {
  QString errorCommand, errorLine, errorMessage = block.text().mid(1).trimmed();
  
  for (block = block.next(); block.isValid(); block = block.next()) {
    QString text = block.text().trimmed();
    if (text.startsWith("l.")) {
      errorLine = text.mid(2, text.indexOf(QString(" ")) - 2).trimmed();
      errorCommand = text.mid(2 + errorLine.length()).trimmed();
      break;
    }
  }
  
  if (errorLine.isEmpty() || errorLine.isNull()) {
    return errorMessage + tr(" (No line found)");
  }
  
  return errorLine + ": " + errorMessage + " " + errorCommand;
}

void LogConsole::setCurrentEditor(Editor *editor) {
  m_editor = editor;
}

