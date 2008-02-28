#ifndef __SETTINGDIALOG_H__
#define __SETTINGDIALOG_H__

#include <QCheckBox>
#include <QColor>
#include <QColorDialog>
#include <QDialog>
#include <QFontComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QSettings>
#include <QSize>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "colorchoosebutton.h"

class SettingDialog : public QDialog 
{
  Q_OBJECT
  
  public:
    SettingDialog(QWidget *);
    
  private slots:
    void saveSettings();
    
  private:
    void createConnections();
    void createDialog();
    void loadSettings();
    
    QCheckBox *buttonCommentBold, *buttonCommentItalic;
    QCheckBox *buttonLatexBold, *buttonLatexItalic;
    QCheckBox *buttonMathBold, *buttonMathItalic;
    QFontComboBox *fontName;
    QHBoxLayout *hLayout;
    QLineEdit *latexCommandInput, *pdflatexCommandInput;
    QPixmap *fontColorPixmap;
    QPushButton *cancel;
    QPushButton *save;
    ColorChooseButton *buttonFontColor;
    ColorChooseButton *buttonCommentForeground, *buttonCommentBackground;
    ColorChooseButton *buttonLatexForeground, *buttonLatexBackground;
    ColorChooseButton *buttonMathForeground, *buttonMathBackground;
    QSpinBox *fontSize;
    QSpinBox *tabulatorWidth;
    QTabWidget *tabs;
    QVBoxLayout *vLayout;
  
};

#endif
