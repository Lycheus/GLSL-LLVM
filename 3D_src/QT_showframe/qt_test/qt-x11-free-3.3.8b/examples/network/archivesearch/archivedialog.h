/****************************************************************************
** Form interface generated from reading ui file 'archivedialog.ui'
**
** Created: 五  3月 20 16:23:03 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ARCHIVEDIALOG_H
#define ARCHIVEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include <qhttp.h>
#include <qurl.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qcursor.h>
#include <qregexp.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QSplitter;
class QListView;
class QListViewItem;
class QTextBrowser;
class QPushButton;

class ArchiveDialog : public QDialog
{
    Q_OBJECT

public:
    ArchiveDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ArchiveDialog();

    QLabel* myTextLabel;
    QLineEdit* myLineEdit;
    QSplitter* splitter1;
    QListView* myListView;
    QTextBrowser* myTextBrowser;
    QPushButton* myPushButton;

public slots:
    void fetch( QListViewItem * it );
    void fetchDone( bool error );
    void search();
    void searchDone( bool error );

protected:
    QVBoxLayout* ArchiveDialogLayout;
    QHBoxLayout* layout1;
    QHBoxLayout* layout3;
    QSpacerItem* mySpacer;

protected slots:
    virtual void languageChange();

private:
    QHttp articleSearcher;
    QHttp articleFetcher;

    void init();

};

#endif // ARCHIVEDIALOG_H
