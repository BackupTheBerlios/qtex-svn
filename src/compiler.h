#ifndef __COMPILER_H__
#define __COMPILER_H__

#include <QObject>
#include <QProcess>
#include <QTextEdit>

enum STATUS {
  COMPILER_ERROR, MISSING_COMMAND, MISSING_TEXT, SUCCESS,  
};

class Compiler : public QObject 
{
  Q_OBJECT
  
  public:
    Compiler(QTextEdit *);
    STATUS compileLatex(QString);
    STATUS compilePdflatex(QString);
    
  private:
    QTextEdit *console;
};

#endif
