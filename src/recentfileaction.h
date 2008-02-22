#ifndef __RECENTFILEACTION_H__
#define __RECENTFILEACTION_H__

#include <QAction>

class RecentFileAction : public QAction
{
  Q_OBJECT
  
  public:
    RecentFileAction(QWidget *parent = 0);
    
  signals:
    void wasTriggered(QAction *);
    
  private slots:
    void dummy();
    
  
};

#endif
