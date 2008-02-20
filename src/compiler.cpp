#include "compiler.h"

Compiler::Compiler(QTextEdit *console) {
  this->console = console;
  QObject::connect(this, SIGNAL(outputReceived(QString)), console, SLOT(setText(QString)));
}

void Compiler::compileLatex(QString filename) {
  path = filename.mid(0, filename.lastIndexOf(QDir::separator()) + 1);
  filename = filename.mid(path.length());
  
  arguments.clear();
  arguments << "-interaction=nonstopmode" << filename;
  
  command = "latex";
  
  start();
}

void Compiler::compilePdflatex(QString filename) {
  path = filename.mid(0, filename.lastIndexOf(QDir::separator()) + 1);
  filename = filename.mid(path.length());
  
  arguments.clear();
  arguments << "-interaction=nonstopmode" << filename;
  
  command = "pdflatex";
  
  start();
}

void Compiler::run() {
  QProcess *proc = new QProcess(); 
  proc->setWorkingDirectory(path);
  proc->setEnvironment(QProcess::systemEnvironment());
  
  proc->start(command, arguments);
  proc->waitForFinished();
  
  QString allData = proc->readAll();
  emit outputReceived(allData);
}
