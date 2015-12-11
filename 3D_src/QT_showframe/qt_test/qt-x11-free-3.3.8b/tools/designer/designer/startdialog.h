/****************************************************************************
** Form interface generated from reading ui file 'startdialog.ui'
**
** Created: 五  3月 20 16:22:13 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef STARTDIALOGBASE_H
#define STARTDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QIconView;
class QIconViewItem;
class QLabel;
class QCheckBox;
class QPushButton;

class StartDialogBase : public QDialog
{
    Q_OBJECT

public:
    StartDialogBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~StartDialogBase();

    QTabWidget* tabWidget;
    QWidget* Widget8;
    QIconView* templateView;
    QWidget* tab;
    QWidget* Widget9;
    QIconView* recentView;
    QLabel* fileInfoLabel;
    QCheckBox* checkShowInFuture;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public slots:
    virtual void recentItemChanged( QIconViewItem * );
    virtual void clearFileInfo();
    virtual void accept();
    virtual void reject();

protected:
    QVBoxLayout* StartDialogBaseLayout;
    QGridLayout* Widget8Layout;
    QHBoxLayout* tabLayout;
    QGridLayout* Widget9Layout;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // STARTDIALOGBASE_H
