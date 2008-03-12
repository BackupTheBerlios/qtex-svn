#ifndef __ABOUTDIALOG_H__
#define __ABOUTDIALOG_H__

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>

class AboutDialog : public QDialog
{
  Q_OBJECT
  
  public:
    AboutDialog(QWidget *parent = 0);
    
  private:
    void createDialog();
};

#endif
