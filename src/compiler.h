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

class Compiler : QThread
{
  Q_OBJECT
  
  public:
    Compiler(QTextEdit *);
    void checkEnvironment();
    void compileLatex(QString);
    void compilePdflatex(QString);
    
  signals:
    void signalOutputReceived(QString);
    void signalError(QString);
    
  private slots:
    void slotCommandNotFound();
    
  private:
    void compile(QString, QString, QStringList);
    void run();
     
    QProcess *m_proc;
    QString m_path, m_command;
    QStringList m_arguments;
    QTextEdit *m_console;
};

#endif
