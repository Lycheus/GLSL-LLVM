/****************************************************************************
** Form interface generated from reading ui file 'dnd/dndbase.ui'
**
** Created: 五  3月 20 16:22:57 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef DNDDEMOBASE_H
#define DNDDEMOBASE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class StyledButton;
class IconView;
class ListView;
class QTextEdit;

class DnDDemoBase : public QWidget
{
    Q_OBJECT

public:
    DnDDemoBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~DnDDemoBase();

    StyledButton* buttonColor1;
    StyledButton* buttonColor2;
    StyledButton* buttonColor3;
    StyledButton* buttonColor4;
    StyledButton* buttonPixmap1;
    StyledButton* buttonPixmap2;
    StyledButton* buttonPixmap3;
    StyledButton* buttonPixmap4;
    ListView* listView;
    IconView* iconView;
    QTextEdit* multiLine1;
    QTextEdit* multiLine2;

protected:
    QGridLayout* DnDDemoBaseLayout;
    QHBoxLayout* Layout5;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();


private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;
    QPixmap image4;
    QPixmap image5;

};

#endif // DNDDEMOBASE_H
