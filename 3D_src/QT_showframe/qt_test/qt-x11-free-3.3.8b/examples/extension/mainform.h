/****************************************************************************
** Form interface generated from reading ui file 'mainform.ui'
**
** Created: 五  3月 20 16:22:58 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MAINFORM_H
#define MAINFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;

class MainForm : public QDialog
{
    Q_OBJECT

public:
    MainForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~MainForm();

    QPushButton* optionsPushButton;
    QPushButton* quitPushButton;

public slots:
    virtual void optionsDlg();
    virtual void quit();

protected:
    bool log_errors;
    bool log_warnings;
    bool log_actions;
    bool logging;
    bool sessions;
    QString log_filename;

    QVBoxLayout* MainFormLayout;

protected slots:
    virtual void languageChange();

private:
    virtual void init();

};

#endif // MAINFORM_H
