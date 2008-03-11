#ifndef __SETTINGDIALOG_H__
#define __SETTINGDIALOG_H__

#include <QApplication>
#include <QCheckBox>
#include <QColor>
#include <QColorDialog>
#include <QComboBox>
#include <QCoreApplication>
#include <QDialog>
#include <QDir>
#include <QFontComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QSettings>
#include <QSize>
#include <QSpinBox>
#include <QStringList>
#include <QTabWidget>
#include <QTranslator>
#include <QVBoxLayout>
#include <QWidget>

#include "colorchoosebutton.h"
#include "languagechooser.h"

class SettingDialog : public QDialog 
{
  Q_OBJECT
  
  public:
    SettingDialog(QWidget *);
    
  private slots:
    void slotLanguageChanged();
    void slotSaveSettings();
    
  private:
    void createConnections();
    void createDialog();
    void loadSettings();
    
    bool m_languageChanged;
    QCheckBox *m_buttonCommentBold, *m_buttonCommentItalic;
    QCheckBox *m_buttonLatexBold, *m_buttonLatexItalic;
    QCheckBox *m_buttonMathBold, *m_buttonMathItalic;
    QComboBox *m_languages;
    QFontComboBox *m_fontName;
    QHBoxLayout *m_hLayout;
    QLineEdit *m_latexCommandInput, *m_pdflatexCommandInput;
    QPixmap *m_fontColorPixmap;
    QPushButton *m_cancel, *m_save;
    ColorChooseButton *m_buttonFontColor;
    ColorChooseButton *m_buttonCommentForeground, *m_buttonCommentBackground;
    ColorChooseButton *m_buttonLatexForeground, *m_buttonLatexBackground;
    ColorChooseButton *m_buttonMathForeground, *m_buttonMathBackground;
    QSpinBox *m_fontSize;
    QSpinBox *m_tabulatorWidth;
    QTabWidget *m_tabs;
    QVBoxLayout *m_vLayout;
  
};

#endif
