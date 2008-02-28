#include <QApplication>
#include <QInputDialog>
#include "compiler.h"

Compiler::Compiler(QTextEdit *console) {
  this->console = console;
  proc = new QProcess();
  
  qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
  qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
  
  QObject::connect(this, SIGNAL(outputReceived(QString)), console, SLOT(setText(QString)));
  QObject::connect(proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(commandNotFound()));
}

void Compiler::checkEnvironment() {
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("compiler"));
  QString latex = settings.value(QString("latex"), QString("latex")).toString();
  QString pdflatex = settings.value(QString("pdflatex"), QString("pdflatex")).toString();
  settings.endGroup();
  
  QProcess *p = new QProcess();
  p->setEnvironment(QProcess::systemEnvironment());
  
  QString missingPrograms;
  QStringList arguments;
  
  arguments << latex << "-version";  
  p->start(latex, arguments);
  p->waitForStarted();
  if (p->error() == QProcess::FailedToStart) {
    missingPrograms += QString("Latex\n");
  }
  p->terminate();
  
  arguments.clear();
  arguments << pdflatex << "-version";
  p->start(pdflatex, arguments);
  p->waitForStarted();
  if (p->error() == QProcess::FailedToStart) {
    missingPrograms += QString("Pdflatex\n");
  }
  p->terminate();
  
  if (missingPrograms.isNull() || missingPrograms.isEmpty()) {
    return;
  }
  
  QMessageBox::critical(0, trUtf8("Fehler"), trUtf8("Folgende Programme wurden in der aktuellen Umgebung nicht gefunden:\n\n") + missingPrograms + trUtf8("\nBitte tragen Sie die exakten Pfade in den Einstellungen nach!"));
}

void Compiler::compileLatex(QString filename) {
  path = filename.mid(0, filename.lastIndexOf(QDir::separator()) + 1);
  filename = filename.mid(path.length());
  
  arguments.clear();
  arguments << "-interaction=nonstopmode" << filename;
  
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("compiler"));
  command = settings.value(QString("latex"), QString("latex")).toString();
  settings.endGroup();
  
  start();
}

void Compiler::compilePdflatex(QString filename) {
  path = filename.mid(0, filename.lastIndexOf(QDir::separator()) + 1);
  filename = filename.mid(path.length());
  
  arguments.clear();
  arguments << "-interaction=nonstopmode" << filename;
  
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("compiler"));
  command = settings.value(QString("pdflatex"), QString("pdflatex")).toString();
  settings.endGroup();
  
  start();
}

void Compiler::commandNotFound() {
  this->terminate();
  emit outputReceived(trUtf8("Befehl nicht gefunden: ") + this->command);
  
  QString title = trUtf8("Befehl nicht gefunden");
  QString message = trUtf8("Der Befehl '") + command + trUtf8("' wurde nicht gefunden! Bitte setzen Sie in den Einstellungen den richtigen Pfad!");
  QMessageBox::critical(0, title, message);
}

void Compiler::run() { 
  proc->setWorkingDirectory(path);
  proc->setEnvironment(QProcess::systemEnvironment());
  
  proc->start(command, arguments);
  proc->waitForFinished();
  
  QString allData = proc->readAll();
  if (!allData.isEmpty() && !allData.isNull()) {
    emit outputReceived(allData);
  }
}
