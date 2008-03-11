#ifndef __LANGUAGECHOOSER_H__
#define __LANGUAGECHOOSER_H__

#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QCoreApplication>
#include <QDialog>
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QLibraryInfo>
#include <QPushButton>
#include <QSettings>
#include <QStringList>
#include <QTranslator>
#include <QVBoxLayout>
#include <QWidget>

class LanguageChooser : public QDialog
{
  Q_OBJECT
  
  public:
    LanguageChooser(QWidget *parent = 0);
    static QString getCurrentLanguageCode();
    QString getLanguageCode();
    static QStringList getLanguages();
    static void setLanguage(QString);
    
  protected:
    void closeEvent(QCloseEvent *);
    
  private:
    void createDialog();
    
    QComboBox *m_languages;
    QPushButton *m_cancel, *m_select;
    QString language;
};

#endif
