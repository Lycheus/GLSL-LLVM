/****************************************************************************
** Form interface generated from reading ui file 'listeditor.ui'
**
** Created: 五  3月 20 16:22:13 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef LISTEDITOR_H
#define LISTEDITOR_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListView;
class QListViewItem;
class QPushButton;
class QStringList;

class ListEditor : public QDialog
{
    Q_OBJECT

public:
    ListEditor( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ListEditor();

    QListView* listview;
    QPushButton* PushButton1;
    QPushButton* PushButton2;
    QPushButton* PushButton3;
    QPushButton* PushButton4;

public slots:
    virtual void init();
    virtual void setList( const QStringList & l );
    virtual void addItem();
    virtual void renamed( QListViewItem * i );
    virtual void removeItems();
    virtual QStringList items();
    virtual void renameItem();

protected:
    QGridLayout* ListEditorLayout;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

};

#endif // LISTEDITOR_H
