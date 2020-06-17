/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "tree_view_kv.hpp"
#include <QHeaderView>
#include <QResizeEvent>
#include <QScrollBar>
#include <iostream>

namespace Wdg
{

Tree_View_KV::Tree_View_KV ( QWidget * parent )
: QTreeView ( parent )
, _activate_current ( true )
{
  header ()->setSectionResizeMode ( QHeaderView::Interactive );
}

void
Tree_View_KV::setModel ( QAbstractItemModel * model_n )
{
  QTreeView::setModel ( model_n );

  QFontMetrics fmet ( fontMetrics () );
  const int mss ( 3 * indentation () + fmet.averageCharWidth () * 8 );
  header ()->setMinimumSectionSize ( mss );
  header ()->resizeSection ( 0, mss );
}

void
Tree_View_KV::set_activate_current ( bool flag_n )
{
  _activate_current = flag_n;
}

void
Tree_View_KV::set_expanded_recursive ( const QModelIndex & index_n,
                                       int depth_n,
                                       bool expanded_n )
{
  setExpanded ( index_n, expanded_n );

  if ( depth_n > 0 ) {
    const int rows ( model ()->rowCount ( index_n ) );
    for ( int ii = 0; ii < rows; ++ii ) {
      const QModelIndex mi ( model ()->index ( ii, 0, index_n ) );
      set_expanded_recursive ( mi, depth_n - 1, expanded_n );
    }
  } else {
    collapse ( index_n );
  }
}

void
Tree_View_KV::expand_recursive ( const QModelIndex & index_n, int depth_n )
{
  set_expanded_recursive ( index_n, depth_n, true );
}

void
Tree_View_KV::collapse_recursive ( const QModelIndex & index_n, int depth_n )
{
  set_expanded_recursive ( index_n, depth_n, false );
}

void
Tree_View_KV::adjust_first_column_width ()
{
  int hspace_free ( contentsRect ().width () );
  if ( verticalScrollBar () != 0 ) {
    if ( verticalScrollBar ()->isVisible () ) {
      hspace_free -= verticalScrollBar ()->width ();
    }
  }
  QFontMetrics fm ( fontMetrics () );

  int ww = sizeHintForColumn ( 0 ) + fm.averageCharWidth ();
  if ( ww < hspace_free ) {
    header ()->resizeSection ( 0, ww );
  }
}

void
Tree_View_KV::currentChanged ( const QModelIndex & current,
                               const QModelIndex & previous )
{
  QTreeView::currentChanged ( current, previous );
  if ( _activate_current ) {
    emit activated ( current );
  }
}

} // namespace Wdg
