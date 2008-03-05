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
    int slotExec();
    
  protected:
    void closeEvent(QCloseEvent);
    
  private slots:
    void slotSearchButtonClicked();
    void slotToggleSearchButton(QString);
    
  private:
    void createDialog();
    
    QCheckBox *m_backward;
    QCheckBox *m_caseSensitive;
    QCheckBox *m_wholeWords;
    QLineEdit *m_searchTextInput;
    QPushButton *m_close;
    QPushButton *m_search;
    QString m_searchText;
};

#endif
