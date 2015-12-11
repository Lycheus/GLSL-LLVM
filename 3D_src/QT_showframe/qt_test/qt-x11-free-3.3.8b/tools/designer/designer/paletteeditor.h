/****************************************************************************
** Form interface generated from reading ui file 'paletteeditor.ui'
**
** Created: 五  3月 20 16:22:17 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PALETTEEDITORBASE_H
#define PALETTEEDITORBASE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class StyledButton;
class PreviewFrame;
class QGroupBox;
class QLabel;
class QPushButton;
class QComboBox;

class PaletteEditorBase : public QDialog
{
    Q_OBJECT

public:
    PaletteEditorBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~PaletteEditorBase();

    QGroupBox* groupAutoPalette;
    QLabel* labelMainColor;
    StyledButton* buttonMainColor;
    QLabel* labelMainColor2;
    StyledButton* buttonMainColor2;
    QPushButton* btnAdvanced;
    QGroupBox* GroupBox126;
    QLabel* TextLabel1;
    QComboBox* paletteCombo;
    PreviewFrame* previewFrame;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

protected:
    QVBoxLayout* PaletteEditorBaseLayout;
    QHBoxLayout* groupAutoPaletteLayout;
    QSpacerItem* Spacer1;
    QVBoxLayout* GroupBox126Layout;
    QHBoxLayout* Layout2;
    QHBoxLayout* Layout3;
    QSpacerItem* Horizontal_Spacing11;

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();
    virtual void onTune();
    virtual void onChoose2ndMainColor();
    virtual void onChooseMainColor();
    virtual void paletteSelected(int);


private:
    QPixmap image0;
    QPixmap image1;

};

#endif // PALETTEEDITORBASE_H
