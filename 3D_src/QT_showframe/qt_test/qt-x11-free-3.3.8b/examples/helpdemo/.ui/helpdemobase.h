/****************************************************************************
** Form interface generated from reading ui file 'helpdemobase.ui'
**
** Created: 五  3月 20 16:22:58 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HELPDEMOBASE_H
#define HELPDEMOBASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QFrame;
class QCheckBox;
class QLabel;
class QLineEdit;
class QTextEdit;

class HelpDemoBase : public QWidget
{
    Q_OBJECT

public:
    HelpDemoBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~HelpDemoBase();

    QPushButton* closeButton;
    QFrame* frame5;
    QPushButton* openQAButton;
    QPushButton* displayButton;
    QCheckBox* checkHide;
    QPushButton* closeQAButton;
    QLabel* textLabel3;
    QLineEdit* leFileName;
    QCheckBox* checkOnlyExampleDoc;
    QTextEdit* textEdit1;

public slots:
    virtual void displayPage();
    virtual void openAssistant();
    virtual void closeAssistant();
    virtual void setAssistantArguments();

protected:
    QGridLayout* HelpDemoBaseLayout;
    QSpacerItem* spacer2;
    QHBoxLayout* layout2;
    QSpacerItem* spacer1;
    QGridLayout* frame5Layout;

protected slots:
    virtual void languageChange();

};

#endif // HELPDEMOBASE_H
