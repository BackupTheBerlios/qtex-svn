#ifndef __SETTINGDIALOG_H__
#define __SETTINGDIALOG_H__

#include <QColor>
#include <QColorDialog>
#include <QDialog>
#include <QFontComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QSettings>
#include <QSize>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

class SettingDialog : public QDialog 
{
  Q_OBJECT
  
  public:
    SettingDialog(QWidget *);
    
  private slots:
    void getFontColor();
    void saveSettings();
    
  private:
    void createConnections();
    void createDialog();
    void loadSettings();
    
    QColor fontColor;
    QFontComboBox *fontName;
    QHBoxLayout *hLayout;
    QPixmap *fontColorPixmap;
    QPushButton *cancel;
    QPushButton *save;
    QPushButton *buttonFontColor;
    QSpinBox *fontSize;
    QSpinBox *tabulatorWidth;
    QTabWidget *tabs;
    QVBoxLayout *vLayout;
  
};

#endif
