/****************************************************************************
** Form implementation generated from reading ui file 'tabbedbrowser.ui'
**
** Created: 五  3月 20 16:22:45 2015
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "tabbedbrowser.h"

#include <qvariant.h>
#include <qstylesheet.h>
#include <qmime.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "helpwindow.h"
#include "mainwindow.h"
#include "tabbedbrowser.ui.h"
/*
 *  Constructs a TabbedBrowser as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
TabbedBrowser::TabbedBrowser( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "TabbedBrowser" );
    TabbedBrowserLayout = new QGridLayout( this, 1, 1, 0, 0, "TabbedBrowserLayout"); 

    tab = new QTabWidget( this, "tab" );

    frontpage = new QWidget( tab, "frontpage" );
    frontpageLayout = new QGridLayout( frontpage, 1, 1, 11, 6, "frontpageLayout"); 
    tab->insertTab( frontpage, QString::fromLatin1("") );

    TabbedBrowserLayout->addWidget( tab, 0, 0 );
    languageChange();
    resize( QSize(710, 681).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
TabbedBrowser::~TabbedBrowser()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void TabbedBrowser::languageChange()
{
    setCaption( tr( "TabbedBrowser" ) );
    tab->changeTab( frontpage, tr( "Untitled" ) );
}

