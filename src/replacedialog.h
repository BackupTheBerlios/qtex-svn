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
#include <QShowEvent>
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
    int exec();
    
  protected:
    void closeEvent(QCloseEvent);
    
  private slots:
    void replaceButtonClicked();
    void toggleReplaceButton(QString);
    
  private:
    void createDialog();
    
    QCheckBox *backward;
    QCheckBox *caseSensitive;
    QCheckBox *promptOnReplace;
    QCheckBox *wholeWords;
    QLineEdit *replaceTextInput;
    QLineEdit *searchTextInput;
    QPushButton *close;
    QPushButton *replace;
    QString replaceText;
    QString searchText;
};

#endif
