#ifndef __RECENTFILEACTION_H__
#define __RECENTFILEACTION_H__

#include <QAction>

class RecentFileAction : public QAction
{
  Q_OBJECT
  
  public:
    RecentFileAction(QWidget *parent = 0);
    
  signals:
    void signalWasTriggered(QAction *);
    
  private slots:
    void slotWasTriggered();
    
  
};

#endif
