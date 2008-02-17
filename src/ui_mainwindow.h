/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Sun Feb 17 21:44:52 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>

class Ui_MainWindow
{
public:
    QAction *action_Neu;
    QAction *action_Oeffnen;
    QAction *action_Beenden;
    QWidget *centralwidget;
    QHBoxLayout *hboxLayout;
    QSplitter *vSplitter;
    QSplitter *hSplitter;
    QToolBox *toolbox;
    QWidget *page;
    QWidget *page_2;
    QTabWidget *tabs;
    QWidget *tab;
    QHBoxLayout *hboxLayout1;
    QTextEdit *input;
    QTextEdit *output;
    QMenuBar *menubar;
    QMenu *menu_Datei;
    QStatusBar *statusbar;
    QToolBar *toolbar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(598, 388);
    action_Neu = new QAction(MainWindow);
    action_Neu->setObjectName(QString::fromUtf8("action_Neu"));
    action_Oeffnen = new QAction(MainWindow);
    action_Oeffnen->setObjectName(QString::fromUtf8("action_Oeffnen"));
    action_Beenden = new QAction(MainWindow);
    action_Beenden->setObjectName(QString::fromUtf8("action_Beenden"));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    hboxLayout = new QHBoxLayout(centralwidget);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    vSplitter = new QSplitter(centralwidget);
    vSplitter->setObjectName(QString::fromUtf8("vSplitter"));
    vSplitter->setOrientation(Qt::Vertical);
    hSplitter = new QSplitter(vSplitter);
    hSplitter->setObjectName(QString::fromUtf8("hSplitter"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(5);
    sizePolicy.setHeightForWidth(hSplitter->sizePolicy().hasHeightForWidth());
    hSplitter->setSizePolicy(sizePolicy);
    hSplitter->setOrientation(Qt::Horizontal);
    toolbox = new QToolBox(hSplitter);
    toolbox->setObjectName(QString::fromUtf8("toolbox"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(1);
    sizePolicy1.setVerticalStretch(1);
    sizePolicy1.setHeightForWidth(toolbox->sizePolicy().hasHeightForWidth());
    toolbox->setSizePolicy(sizePolicy1);
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    page->setGeometry(QRect(0, 0, 109, 172));
    toolbox->addItem(page, QApplication::translate("MainWindow", "Page 1", 0, QApplication::UnicodeUTF8));
    page_2 = new QWidget();
    page_2->setObjectName(QString::fromUtf8("page_2"));
    page_2->setGeometry(QRect(0, 0, 96, 26));
    toolbox->addItem(page_2, QApplication::translate("MainWindow", "Page 2", 0, QApplication::UnicodeUTF8));
    hSplitter->addWidget(toolbox);
    tabs = new QTabWidget(hSplitter);
    tabs->setObjectName(QString::fromUtf8("tabs"));
    QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy2.setHorizontalStretch(4);
    sizePolicy2.setVerticalStretch(1);
    sizePolicy2.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
    tabs->setSizePolicy(sizePolicy2);
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    hboxLayout1 = new QHBoxLayout(tab);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    hboxLayout1->setContentsMargins(0, 0, 0, 0);
    input = new QTextEdit(tab);
    input->setObjectName(QString::fromUtf8("input"));

    hboxLayout1->addWidget(input);

    tabs->addTab(tab, QString());
    hSplitter->addWidget(tabs);
    vSplitter->addWidget(hSplitter);
    output = new QTextEdit(vSplitter);
    output->setObjectName(QString::fromUtf8("output"));
    sizePolicy1.setHeightForWidth(output->sizePolicy().hasHeightForWidth());
    output->setSizePolicy(sizePolicy1);
    vSplitter->addWidget(output);

    hboxLayout->addWidget(vSplitter);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 598, 26));
    menu_Datei = new QMenu(menubar);
    menu_Datei->setObjectName(QString::fromUtf8("menu_Datei"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);
    toolbar = new QToolBar(MainWindow);
    toolbar->setObjectName(QString::fromUtf8("toolbar"));
    MainWindow->addToolBar(Qt::TopToolBarArea, toolbar);

    menubar->addAction(menu_Datei->menuAction());
    menu_Datei->addAction(action_Neu);
    menu_Datei->addAction(action_Oeffnen);
    menu_Datei->addAction(action_Beenden);

    retranslateUi(MainWindow);
    QObject::connect(action_Beenden, SIGNAL(triggered()), MainWindow, SLOT(close()));

    toolbox->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QteX", 0, QApplication::UnicodeUTF8));
    action_Neu->setText(QApplication::translate("MainWindow", "&Neu", 0, QApplication::UnicodeUTF8));
    action_Neu->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
    action_Oeffnen->setText(QApplication::translate("MainWindow", "\303\226&ffnen", 0, QApplication::UnicodeUTF8));
    action_Oeffnen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
    action_Beenden->setText(QApplication::translate("MainWindow", "&Beenden", 0, QApplication::UnicodeUTF8));
    action_Beenden->setShortcut(QApplication::translate("MainWindow", "Ctrl+B", 0, QApplication::UnicodeUTF8));
    toolbox->setItemText(toolbox->indexOf(page), QApplication::translate("MainWindow", "Page 1", 0, QApplication::UnicodeUTF8));
    toolbox->setItemText(toolbox->indexOf(page_2), QApplication::translate("MainWindow", "Page 2", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
    menu_Datei->setTitle(QApplication::translate("MainWindow", "&Datei", 0, QApplication::UnicodeUTF8));
    toolbar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

#endif // UI_MAINWINDOW_H
