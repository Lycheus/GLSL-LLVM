/****************************************************************************
** Form interface generated from reading ui file 'sql/connect.ui'
**
** Created: 五  3月 20 16:22:57 2015
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
class QGroupBox;
class QLineEdit;
class QLabel;
class QComboBox;
class QSpinBox;
class QPushButton;

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConnectDialog();

    QGroupBox* GroupBox3;
    QLineEdit* editDatabase;
    QLabel* TextLabel3;
    QLineEdit* editPassword;
    QLabel* TextLabel4;
    QLabel* TextLabel4_2;
    QComboBox* comboDriver;
    QLineEdit* editUsername;
    QLineEdit* editHostname;
    QLabel* TextLabel5;
    QLabel* TextLabel5_2;
    QSpinBox* portSpinBox;
    QLabel* TextLabel2;
    QPushButton* PushButton1;
    QPushButton* PushButton2;

public slots:
    virtual void init();
    virtual void destroy();

protected:
    QVBoxLayout* ConnectDialogLayout;
    QGridLayout* GroupBox3Layout;
    QHBoxLayout* Layout25;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

};

#endif // CONNECTDIALOG_H
