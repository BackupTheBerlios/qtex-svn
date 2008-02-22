#ifndef __RECENTFILEMANAGER_H__
#define __RECENTFILEMANAGER_H__

#include <QObject>
#include <QList>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QString>

#include "recentfileaction.h"

class RecentFileManager : public QObject
{
  Q_OBJECT
  
  public:
    RecentFileManager();
    void addFile(QString);
    QList<RecentFileAction *> getActions();
    
  signals:
    void update();
    
  private:
    int capacity;
    QList<RecentFileAction *> recentFileActions;
    QList<QString> recentFiles;
};

#endif
