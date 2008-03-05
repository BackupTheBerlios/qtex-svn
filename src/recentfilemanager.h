#ifndef __RECENTFILEMANAGER_H__
#define __RECENTFILEMANAGER_H__

#include <QObject>
#include <QList>
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
    void signalUpdate();
    
  private:
    void exportRecentFiles();
    void importRecentFiles();
    
    int m_capacity;
    QList<RecentFileAction *> m_recentFileActions;
    QList<QString> m_recentFiles;
};

#endif
