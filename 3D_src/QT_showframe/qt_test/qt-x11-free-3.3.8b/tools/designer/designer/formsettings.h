/****************************************************************************
** Form interface generated from reading ui file 'formsettings.ui'
**
** Created: 五  3月 20 16:22:13 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FORMSETTINGSBASE_H
#define FORMSETTINGSBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QButtonGroup;
class QRadioButton;
class QLineEdit;
class QGroupBox;
class QLabel;
class QMultiLineEdit;
class QSpinBox;
class QCheckBox;
class QPushButton;

class FormSettingsBase : public QDialog
{
    Q_OBJECT

public:
    FormSettingsBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FormSettingsBase();

    QButtonGroup* ButtonGroup1;
    QRadioButton* radioPixmapInline;
    QRadioButton* radioProjectImageFile;
    QLineEdit* editPixmapFunction;
    QRadioButton* radioPixmapFunction;
    QGroupBox* GroupBox1;
    QLineEdit* editClassName;
    QLabel* TextLabel1_2;
    QLineEdit* editAuthor;
    QLabel* TextLabel1;
    QLabel* TextLabel2;
    QMultiLineEdit* editComment;
    QGroupBox* GroupBox2;
    QSpinBox* spinMargin;
    QLabel* TextLabel2_2;
    QCheckBox* checkLayoutFunctions;
    QSpinBox* spinSpacing;
    QLabel* TextLabel1_3;
    QLabel* spaFuLabel;
    QLabel* marFuLabel;
    QLineEdit* editSpacingFunction;
    QLineEdit* editMarginFunction;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public slots:
    virtual void okClicked();

protected:
    QGridLayout* FormSettingsBaseLayout;
    QGridLayout* ButtonGroup1Layout;
    QGridLayout* GroupBox1Layout;
    QSpacerItem* Spacer1;
    QGridLayout* GroupBox2Layout;
    QSpacerItem* Spacer3;
    QSpacerItem* Spacer4;
    QSpacerItem* Spacer5;
    QHBoxLayout* Layout4;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // FORMSETTINGSBASE_H
