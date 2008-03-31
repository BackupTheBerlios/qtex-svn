#ifndef __CODECACTION_H__
#define __CODECACTION_H__

#include <QAction>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QWidget>

class CodecAction : public QAction
{
  Q_OBJECT
  
  public:
    CodecAction(QWidget *, QString, QString);
    static QStringList getAvailableCodecs(); 
    static QStringList getAvailableDescriptions();
    QString getCodec();
    static QString getDefaultCodec();
    QString getDescription();
    
  signals:
    void signalWasTriggered(CodecAction *);
    
  private slots:
    void slotWasTriggered();
    
  private:
    QString m_Codec, m_Description;
};

#endif
