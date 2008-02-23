#include "recentfilemanager.h"

RecentFileManager::RecentFileManager() {
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("recent"));
  capacity = settings.value(QString("size"), 10).toInt();
  settings.endGroup();
  
  importRecentFiles();
}

void RecentFileManager::addFile(QString filename) {
  int pos = recentFiles.indexOf(filename);
  if (pos > 0) {
    recentFiles.move(pos, 0);
  } else if (pos == -1) {
    recentFiles.insert(0, filename);
  }
  
  while (recentFiles.size() > capacity) {
    recentFiles.removeLast();
  }
  
  exportRecentFiles();
  emit update();
}

void RecentFileManager::exportRecentFiles() {
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("recent"));
  settings.beginWriteArray(QString("files"));
  
  for (int i = 0; i < recentFiles.size(); i++) {
    settings.setArrayIndex(i);
    settings.setValue(QString("filename"), recentFiles.at(i));
  }
  
  settings.endArray();
  settings.endGroup();
}

void RecentFileManager::importRecentFiles() {
  recentFiles.clear();
  
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("recent"));
  int size = settings.beginReadArray(QString("files"));
  
  for (int i = 0; i < size; i++) {
    settings.setArrayIndex(i);
    recentFiles.insert(i, settings.value(QString("filename"), QString()).toString());
  }
  
  settings.endArray();
  settings.endGroup();
  
  for (int i = 0; i < recentFiles.size(); i++) {
    QString tmp = recentFiles.at(i);
    if (tmp.isNull() || tmp.isEmpty()) {
      recentFiles.removeAt(i);
      i--;
    }
  }
}

QList<RecentFileAction *> RecentFileManager::getActions() {
  int size = recentFileActions.size();
  if (size > 0) {
    for (int i = 0; i < size; i++) {
      RecentFileAction *act = recentFileActions.at(i);
      if (act == 0) {
        continue;
      }
      
      QObject::disconnect(act, 0, 0, 0);
      delete act;
      act = 0;
    }
    
    recentFileActions.clear();
  }
  
  size = recentFiles.size();
  if (size == 0) {
    QList<RecentFileAction *> ret;
    return ret;
  }
  
  for (int i = 0; i < size; i++) {
    RecentFileAction *act = new RecentFileAction(0);
    act->setObjectName(QString("act") + QString::number(i+1));
    act->setText(QString::number(i+1) + QString(": ") + recentFiles.at(i));
    
    recentFileActions.insert(i, act);
  }
  
  return recentFileActions;
}
