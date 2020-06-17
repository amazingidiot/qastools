/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "multi_page_dialog.hpp"
#include "wdg/scroll_area_vertical.hpp"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <iostream>

namespace Views
{

Multi_Page_Dialog::Multi_Page_Dialog ( QWidget * parent_n,
                                       Qt::WindowFlags flags_n )
: ::Views::Basic_Dialog ( parent_n, flags_n )
{
  {
    _pages_model = new QStandardItemModel ( this );
    _page_selection = new QListView;
    _page_selection->setModel ( _pages_model );

    // QueuedConnection to paint update the tree view before
    // painting heavy new widgets
    connect (
        _page_selection->selectionModel (),
        SIGNAL ( currentChanged ( const QModelIndex &, const QModelIndex & ) ),
        this,
        SLOT ( page_changed ( const QModelIndex &, const QModelIndex & ) ),
        Qt::QueuedConnection );
  }

  // Close button layout
  QHBoxLayout * lay_close ( new QHBoxLayout );
  {
    QPushButton * btn_close ( create_close_button () );

    lay_close->setContentsMargins ( 0, 0, 0, 0 );
    lay_close->addStretch ( 1 );
    lay_close->addWidget ( btn_close, 0 );
    lay_close->addStretch ( 1 );
  }

  // Page selection widget
  QWidget * navi_wdg ( new QWidget );
  {
    QVBoxLayout * lay_vbox ( new QVBoxLayout );
    lay_vbox->setContentsMargins ( 0, 0, 0, 0 );
    lay_vbox->addWidget ( _page_selection, 1 );
    lay_vbox->addLayout ( lay_close, 0 );
    navi_wdg->setLayout ( lay_vbox );
  }

  // Page display widget
  QWidget * pages_wdg ( new QWidget );
  {
    QHBoxLayout * lay_pages ( new QHBoxLayout );
    lay_pages->setContentsMargins ( 0, 0, 0, 0 );
    pages_wdg->setLayout ( lay_pages );

    _lay_pages_stack = new QStackedLayout;
    _lay_pages_stack->setContentsMargins ( 0, 0, 0, 0 );

    lay_pages->addSpacing ( fontMetrics ().averageCharWidth () * 3 / 2 );
    lay_pages->addLayout ( _lay_pages_stack );
  }

  // Main splitter and layout
  {
    QSplitter * hsplit ( new QSplitter );
    hsplit->setChildrenCollapsible ( false );
    hsplit->addWidget ( navi_wdg );
    hsplit->addWidget ( pages_wdg );
    hsplit->setStretchFactor ( 0, 2 );
    hsplit->setStretchFactor ( 1, 5 );

    set_central_widget ( hsplit );
  }
}

Multi_Page_Dialog::~Multi_Page_Dialog () {}

void
Multi_Page_Dialog::add_page_vscroll ( const QString & name_n, QWidget * wdg_n )
{
  if ( wdg_n == 0 ) {
    return;
  }

  // Add widget
  {
    ::Wdg::Scroll_Area_Vertical * vscroll ( new ::Wdg::Scroll_Area_Vertical );
    vscroll->setFrameStyle ( QFrame::NoFrame );
    vscroll->set_widget ( wdg_n );

    add_page ( name_n, vscroll );
  }
}

void
Multi_Page_Dialog::add_page ( const QString & name_n, QWidget * wdg_n )
{
  if ( wdg_n == 0 ) {
    return;
  }

  // Add widget
  _lay_pages_stack->addWidget ( wdg_n );

  // Add selection entry
  {
    QStandardItem * sitem ( new QStandardItem );
    sitem->setText ( name_n );
    sitem->setToolTip ( name_n );
    sitem->setSelectable ( true );
    sitem->setEditable ( false );

    _pages_model->appendRow ( sitem );
  }
}

unsigned int
Multi_Page_Dialog::num_pages () const
{
  return _lay_pages_stack->count ();
}

int
Multi_Page_Dialog::current_page_idx () const
{
  return _lay_pages_stack->currentIndex ();
}

void
Multi_Page_Dialog::set_current_page_idx ( int idx_n )
{
  _page_selection->setCurrentIndex (
      _pages_model->index ( idx_n, 0, QModelIndex () ) );
}

void
Multi_Page_Dialog::page_changed ( const QModelIndex & cur_n,
                                  const QModelIndex & )
{
  page_selected ( cur_n );
}

void
Multi_Page_Dialog::page_selected ( const QModelIndex & index_n )
{
  QModelIndex root_idx;
  for ( int ii = 0; ii < _pages_model->rowCount ( root_idx ); ++ii ) {
    if ( _pages_model->index ( ii, 0, root_idx ) == index_n ) {
      _lay_pages_stack->setCurrentIndex ( ii );
      break;
    }
  }
}

} // namespace Views
