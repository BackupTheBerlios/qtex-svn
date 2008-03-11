#include "compiler.h"

Compiler::Compiler(QTextEdit *console) {
  m_console = console;
  m_proc = new QProcess();
  
  qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
  qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
  
  connect(this, SIGNAL(signalOutputReceived(QString)), m_console, SLOT(setText(QString)));
  connect(m_proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(slotCommandNotFound()));
}

void Compiler::checkEnvironment() {
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("compiler"));
  QString latex = settings.value(QString("latex"), QString("latex")).toString();
  QString pdflatex = settings.value(QString("pdflatex"), QString("pdflatex")).toString();
  settings.endGroup();
  
  QString missingPrograms;
  
  /* Test auf latex */
  QProcess *pLatex = new QProcess();
  pLatex->setEnvironment(QProcess::systemEnvironment());
  pLatex->start(latex, QStringList("-version"));
  pLatex->waitForFinished(1000);
  if (pLatex->error() == QProcess::FailedToStart) {
    missingPrograms += QString("Latex\n");
  }
  pLatex->terminate();
  
  /* Test auf pdflatex */
  QProcess *pPdflatex = new QProcess();
  pPdflatex->setEnvironment(QProcess::systemEnvironment());
  pPdflatex->start(pdflatex, QStringList("-version"));
  pPdflatex->waitForFinished(1000);
  if (pPdflatex->error() == QProcess::FailedToStart) {
    missingPrograms += QString("Pdflatex\n");
  }
  pPdflatex->terminate();
  
  if (missingPrograms.isNull() || missingPrograms.isEmpty()) {
    return;
  }
  
  QMessageBox::critical(0, tr("Error"), tr("The following programs were not found in the current environment:\n\n") + missingPrograms + tr("\nPlease enter the paths in the settings!"));
}

void Compiler::compileLatex(QString filename) {
  m_path = filename.mid(0, filename.lastIndexOf(QDir::separator()) + 1);
  filename = filename.mid(m_path.length());
  
  m_arguments.clear();
  m_arguments << "-interaction=nonstopmode" << filename;
  
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("compiler"));
  m_command = settings.value(QString("latex"), QString("latex")).toString();
  settings.endGroup();
  
  start();
}

void Compiler::compilePdflatex(QString filename) {
  m_path = filename.mid(0, filename.lastIndexOf(QDir::separator()) + 1);
  filename = filename.mid(m_path.length());
  
  m_arguments.clear();
  m_arguments << "-interaction=nonstopmode" << filename;
  
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("compiler"));
  m_command = settings.value(QString("pdflatex"), QString("pdflatex")).toString();
  settings.endGroup();
  
  start();
}

void Compiler::run() { 
  m_proc->setWorkingDirectory(m_path);
  m_proc->setEnvironment(QProcess::systemEnvironment());
  
  m_proc->start(m_command, m_arguments);
  m_proc->waitForFinished();
  
  QString allData = m_proc->readAll();
  if (!allData.isEmpty() && !allData.isNull()) {
    emit signalOutputReceived(allData);
  }
}

void Compiler::slotCommandNotFound() {
  terminate();
  emit signalOutputReceived(tr("Command not found: ") + m_command);
  
  QString title = tr("Command not found");
  QString message = tr("The command '") + m_command + tr("' was not found! Plase enter the correct path in the settings!");
  QMessageBox::critical(0, title, message);
}
