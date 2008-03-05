#ifndef __REPLACEDIALOG_H__
#define __REPLACEDIALOG_H__

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

class ReplaceDialog : public QDialog 
{
  Q_OBJECT
  
  public:
    ReplaceDialog(QWidget *parent = 0);
    bool getPromptOnReplace();
    QString getReplacementText();
    QTextDocument::FindFlags getSearchFlags();
    QString getSearchText();
    
  public slots:
    int slotExec();
    
  protected:
    void closeEvent(QCloseEvent);
    
  private slots:
    void slotReplaceButtonClicked();
    void slotToggleReplaceButton(QString);
    
  private:
    void createDialog();
    
    QCheckBox *m_backward, *m_caseSensitive, *m_promptOnReplace, *m_wholeWords;
    QLineEdit *m_replaceTextInput, *m_searchTextInput;
    QPushButton *m_close, *m_replace;
    QString m_replaceText, m_searchText;
};

#endif
