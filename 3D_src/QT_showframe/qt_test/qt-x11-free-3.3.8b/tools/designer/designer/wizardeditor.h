/****************************************************************************
** Form interface generated from reading ui file 'wizardeditor.ui'
**
** Created: 五  3月 20 16:22:13 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WIZARDEDITORBASE_H
#define WIZARDEDITORBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QListBox;
class QListBoxItem;
class QPushButton;

class WizardEditorBase : public QDialog
{
    Q_OBJECT

public:
    WizardEditorBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~WizardEditorBase();

    QLabel* pagesLabel;
    QListBox* listBox;
    QPushButton* buttonAdd;
    QPushButton* buttonRemove;
    QPushButton* buttonUp;
    QPushButton* buttonDown;
    QPushButton* buttonHelp;
    QPushButton* buttonApply;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

protected:
    QVBoxLayout* WizardEditorBaseLayout;
    QHBoxLayout* Layout19;
    QVBoxLayout* Layout14;
    QVBoxLayout* Layout18;
    QSpacerItem* Spacer11;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();
    virtual void addClicked();
    virtual void applyClicked();
    virtual void cancelClicked();
    virtual void downClicked();
    virtual void helpClicked();
    virtual void itemHighlighted(int);
    virtual void itemSelected(int);
    virtual void okClicked();
    virtual void removeClicked();
    virtual void upClicked();


};

#endif // WIZARDEDITORBASE_H
