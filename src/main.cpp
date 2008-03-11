#include <QApplication>

#include "mainwindow.h"
#include "languagechooser.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  
  /* Sprache auswählen lassen */ 
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("editor"));
  
  if (settings.contains(QString("language"))) {
    QString suffix = settings.value(QString("language"), "en").toString();
    LanguageChooser::setLanguage(suffix);
  } else {
    LanguageChooser *lang = new LanguageChooser(0);
    if (lang->exec() == QDialog::Accepted) {
      LanguageChooser::setLanguage(lang->getLanguageCode());
    }
  }
  
  settings.endGroup();
  
  /* Hauptfenster */
  MainWindow *wnd = new MainWindow();
  wnd->showMaximized();
  
  return app.exec();
}


