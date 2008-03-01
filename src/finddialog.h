#ifndef __FINDDIALOG_H__
#define __FINDDIALOG_H__

#include <QCheckBox>
#include <QCloseEvent>
#include <QDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QShowEvent>
#include <QSizePolicy>
#include <QString>
#include <QTextDocument>
#include <QVBoxLayout>

class FindDialog : public QDialog 
{
  Q_OBJECT
  
  public:
    FindDialog(QWidget *parent = 0);
    QTextDocument::FindFlags getSearchFlags();
    QString getSearchText();
    
  public slots:
    int exec();
    
  protected:
    void closeEvent(QCloseEvent);
    
  private slots:
    void searchButtonClicked();
    void toggleSearchButton(QString);
    
  private:
    void createDialog();
    
    QCheckBox *caseSensitive;
    QCheckBox *wholeWords;
    QLineEdit *searchTextInput;
    QPushButton *close;
    QPushButton *search;
    QString searchText;
};

#endif
