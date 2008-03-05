#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QString>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  
  /* Übersetzungsdateien */
  QString translatorFileName = QLatin1String("qt_de_DE"); 
  QTranslator *translator = new QTranslator(&app); 
  if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) 
    app.installTranslator(translator);
    
  QTranslator *translator2 = new QTranslator(&app);
  translator2->load("QteX_de");
  app.installTranslator(translator2);
  
  /* Hauptfenster */
  MainWindow *wnd = new MainWindow();
  wnd->showMaximized();
  
  return app.exec();
}


