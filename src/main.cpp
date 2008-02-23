#include "src/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  
  MainWindow *wnd = new MainWindow();
  wnd->showMaximized();
  
  return app.exec();
}


