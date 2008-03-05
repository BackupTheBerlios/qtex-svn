#include "recentfileaction.h"

RecentFileAction::RecentFileAction(QWidget *parent) : QAction(parent) {
  connect(this, SIGNAL(triggered()), this, SLOT(slotWasTriggered()));
}

/*
 * Dummy-Methode, um das triggered() - Signal abzufangen
 * und ein eigenes Signal mit this-Zeiger zu werfen.
 */
void RecentFileAction::slotWasTriggered() {
  emit signalWasTriggered(this);
}
