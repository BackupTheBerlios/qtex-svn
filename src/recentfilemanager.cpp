#include "recentfilemanager.h"

RecentFileManager::RecentFileManager() {
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("recent"));
  m_capacity = settings.value(QString("size"), 10).toInt();
  settings.endGroup();
  
  importRecentFiles();
}

void RecentFileManager::addFile(QString filename) {
  int pos = m_recentFiles.indexOf(filename);
  if (pos > 0) {
    m_recentFiles.move(pos, 0);
  } else if (pos == -1) {
    m_recentFiles.insert(0, filename);
  }
  
  while (m_recentFiles.size() > m_capacity) {
    m_recentFiles.removeLast();
  }
  
  exportRecentFiles();
  emit signalUpdate();
}

void RecentFileManager::exportRecentFiles() {
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("recent"));
  settings.beginWriteArray(QString("files"));
  
  for (int i = 0; i < m_recentFiles.size(); i++) {
    settings.setArrayIndex(i);
    settings.setValue(QString("filename"), m_recentFiles.at(i));
  }
  
  settings.endArray();
  settings.endGroup();
}

QList<RecentFileAction *> RecentFileManager::getActions() {
  int size = m_recentFileActions.size();
  if (size > 0) {
    for (int i = 0; i < size; i++) {
      RecentFileAction *act = m_recentFileActions.at(i);
      if (act == 0) {
        continue;
      }
      
      disconnect(act, 0, 0, 0);
      delete act;
      act = 0;
    }
    
    m_recentFileActions.clear();
  }
  
  size = m_recentFiles.size();
  if (size == 0) {
    QList<RecentFileAction *> ret;
    return ret;
  }
  
  for (int i = 0; i < size; i++) {
    RecentFileAction *act = new RecentFileAction(0);
    act->setObjectName(QString("act") + QString::number(i+1));
    act->setText(QString::number(i+1) + QString(": ") + m_recentFiles.at(i));
    
    m_recentFileActions.insert(i, act);
  }
  
  return m_recentFileActions;
}

void RecentFileManager::importRecentFiles() {
  m_recentFiles.clear();
  
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("recent"));
  int size = settings.beginReadArray(QString("files"));
  
  for (int i = 0; i < size; i++) {
    settings.setArrayIndex(i);
    m_recentFiles.insert(i, settings.value(QString("filename"), QString()).toString());
  }
  
  settings.endArray();
  settings.endGroup();
  
  for (int i = 0; i < m_recentFiles.size(); i++) {
    QString tmp = m_recentFiles.at(i);
    if (tmp.isNull() || tmp.isEmpty()) {
      m_recentFiles.removeAt(i);
      i--;
    }
  }
}
