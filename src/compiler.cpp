#include <QApplication>
#include <QInputDialog>
#include "compiler.h"

Compiler::Compiler(QTextEdit *console) {
  this->console = console;
  proc = new QProcess();
  
  qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
  qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
  
  QObject::connect(this, SIGNAL(outputReceived(QString)), console, SLOT(setText(QString)));
  QObject::connect(proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(promptForCommand()));
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

void Compiler::promptForCommand() {
  this->terminate();
  emit outputReceived(trUtf8("Befehl nicht gefunden: ") + this->command);
  
  bool ok;
  QString title = trUtf8("Befehl nicht gefunden");
  QString message = trUtf8("Der Befehl '") + command + trUtf8("' wurde nicht gefunden! ");
  message += trUtf8("Bitte geben Sie den vollständigen Pfad an:");
  QString command = QInputDialog::getText(this->console->parentWidget(), title, message, QLineEdit::Normal, QString(), &ok);
  if (ok && !command.isEmpty() && !command.isNull()) {
    this->command = command;
    start();
  }
}

void Compiler::run() {
  //QProcess *proc = new QProcess(); 
  proc->setWorkingDirectory(path);
  proc->setEnvironment(QProcess::systemEnvironment());
  
  proc->start(command, arguments);
  proc->waitForFinished();
  
  QString allData = proc->readAll();
  if (!allData.isEmpty() && !allData.isNull()) {
    emit outputReceived(allData);
  }
}
