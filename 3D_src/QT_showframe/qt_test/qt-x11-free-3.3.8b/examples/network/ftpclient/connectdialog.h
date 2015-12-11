/****************************************************************************
** Form interface generated from reading ui file 'connectdialog.ui'
**
** Created: 五  3月 20 16:23:04 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConnectDialog();

    QFrame* Frame3;
    QLabel* TextLabel2;
    QLabel* TextLabel5;
    QLabel* TextLabel6;
    QLineEdit* password;
    QLineEdit* username;
    QLineEdit* host;
    QLabel* TextLabel3;
    QSpinBox* port;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

protected:
    QVBoxLayout* ConnectDialogLayout;
    QSpacerItem* Spacer5;
    QGridLayout* Frame3Layout;
    QHBoxLayout* Layout21;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // CONNECTDIALOG_H
