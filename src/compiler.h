#ifndef __COMPILER_H__
#define __COMPILER_H__

#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTextEdit>
#include <QThread>

#include "logconsole.h"

class Compiler : QThread
{
  Q_OBJECT
  
  public:
    Compiler(LogConsole *);
    static bool checkCommand(QString);
    void checkEnvironment();
    void compileLatex(QString);
    void compilePdflatex(QString);
    
  signals:
    void signalOutputReceived(QString, QString, int);
    void signalError(QString);
    
  private slots:
    void slotCommandNotFound();
    
  private:
    void compile(QString, QString, QStringList);
    void run();
     
    LogConsole *m_log;
    QProcess *m_proc;
    QString m_path, m_command;
    QStringList m_arguments;
};

#endif
