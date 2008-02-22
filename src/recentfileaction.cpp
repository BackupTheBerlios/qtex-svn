#include "recentfileaction.h"

RecentFileAction::RecentFileAction(QWidget *parent) : QAction(parent) {
  QObject::connect(this, SIGNAL(triggered()), this, SLOT(dummy()));
}

/*
 * Dummy-Methode, um das triggered() - Signal abzufangen
 * und ein eigenes Signal mit this-Zeiger zu werfen.
 */
void RecentFileAction::dummy() {
  emit wasTriggered(this);
}
