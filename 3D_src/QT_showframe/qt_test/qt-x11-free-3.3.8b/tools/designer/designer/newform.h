/****************************************************************************
** Form interface generated from reading ui file 'newform.ui'
**
** Created: 五  3月 20 16:22:13 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef NEWFORMBASE_H
#define NEWFORMBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QIconView;
class QIconViewItem;
class QLabel;
class QComboBox;

class NewFormBase : public QDialog
{
    Q_OBJECT

public:
    NewFormBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~NewFormBase();

    QPushButton* helpButton;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QIconView* templateView;
    QLabel* labelProject;
    QComboBox* projectCombo;

protected:
    QGridLayout* NewFormBaseLayout;
    QSpacerItem* Spacer2;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

    virtual void itemChanged( QIconViewItem * );
    virtual void projectChanged( const QString & );


};

#endif // NEWFORMBASE_H
