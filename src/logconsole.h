#ifndef __LOGCONSOLE_H__
#define __LOGCONSOLE_H__

#include <QDir>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QString>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextEdit>

#include "editor.h"

class LogConsole : public QTextEdit
{
  Q_OBJECT
  
  public:
    LogConsole(QWidget *parent = 0);
    void setCurrentEditor(Editor *);
    
  public slots:
    void parse(QString, QString, int);
    
  protected:
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    
  private:
    QString parseBadbox(QTextBlock);
    QString parseError(QTextBlock);
    
    Editor *m_editor;
    QTextCharFormat m_errorFormat, m_badBoxFormat, m_normalFormat;
};

#endif
