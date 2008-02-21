#ifndef __COMPILER_H__
#define __COMPILER_H__

#include <QDir>
#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QTextEdit>
#include <QThread>

class Compiler : QThread
{
  Q_OBJECT
  
  public:
    Compiler(QTextEdit *);
    void compileLatex(QString);
    void compilePdflatex(QString);
    
  signals:
    void outputReceived(QString);
    void error(QString);
    
  private slots:
    void promptForCommand();
    
  private:
    void compile(QString, QString, QStringList);
    void run();
     
    QProcess *proc;
    QTextEdit *console;
    QString path, command;
    QStringList arguments;
};

#endif
