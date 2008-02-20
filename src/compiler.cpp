#include <unistd.h>

#include <QDir>
#include <QProcess>
#include <QStringList>
#include "compiler.h"

Compiler::Compiler(QTextEdit *console) {
  this->console = console;
}

STATUS Compiler::compileLatex(QString filename) {
  console->clear();
  
  QString path = filename.mid(0, filename.lastIndexOf(QDir::separator()) + 1);
  filename = filename.mid(path.length());  
  QStringList arguments;
  arguments << "-interaction=nonstopmode" << filename;
  
  QProcess *proc = new QProcess(this);
  proc->setWorkingDirectory(path);
  proc->setEnvironment(QProcess::systemEnvironment());
  
  proc->start(QString("latex"), arguments);
  proc->waitForFinished();
  
  QString allData = proc->readAll();
  console->setText(allData);
  
  QTextCursor cursor = console->textCursor();
  cursor.setPosition(console->toPlainText().size());
  console->setTextCursor(cursor);
}

STATUS Compiler::compilePdflatex(QString filename) {
  console->clear();
  
  QString path = filename.mid(0, filename.lastIndexOf(QDir::separator()) + 1);
  filename = filename.mid(path.length());  
  QStringList arguments;
  arguments << "-interaction=nonstopmode" << filename;
  
  QProcess *proc = new QProcess(this);
  proc->setWorkingDirectory(path);
  proc->setEnvironment(QProcess::systemEnvironment());
  
  proc->start(QString("pdflatex"), arguments);
  proc->waitForFinished();
  
  QString allData = proc->readAll();
  console->setText(allData);
  
  QTextCursor cursor = console->textCursor();
  cursor.setPosition(console->toPlainText().size());
  console->setTextCursor(cursor);
}
