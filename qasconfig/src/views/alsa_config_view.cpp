/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "alsa_config_view.hpp"
#include "qsnd/alsa_config_model.hpp"
#include "wdg/tree_view_kv.hpp"
#include <QAbstractItemModel>
#include <QHBoxLayout>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <iostream>

namespace Views
{

Alsa_Config_View::Alsa_Config_View ( QWidget * parent_n )
: QWidget ( parent_n )
, _alsa_cfg ( 0 )
{
  QLabel * title_label ( new QLabel );
  {
    QString txt ( "<h2>%1</h2>" );
    txt = txt.arg ( tr ( "ALSA configuration" ) );
    title_label->setText ( txt );
  }

  _btn_expand.setText ( tr ( "&Expand" ) );
  _btn_collapse.setText ( tr ( "Co&llapse" ) );
  _btn_sort.setText ( tr ( "&Sort" ) );

  QLabel * expand_depth_label ( new QLabel ( tr ( "Depth:" ) ) );
  _expand_depth.setRange ( 0, 999 );
  _expand_depth.setValue ( 2 );

  connect ( &_btn_collapse,
            SIGNAL ( clicked () ),
            this,
            SLOT ( collapse_to_level () ) );

  connect (
      &_btn_expand, SIGNAL ( clicked () ), this, SLOT ( expand_to_level () ) );

  connect ( &_btn_sort,
            SIGNAL ( toggled ( bool ) ),
            this,
            SLOT ( set_sorting_enabled ( bool ) ) );

  _tree_view = new ::Wdg::Tree_View_KV ( this );
  _sort_model = new QSortFilterProxyModel ( this );

  _tree_view->setSelectionMode ( QAbstractItemView::ExtendedSelection );
  _tree_view->setModel ( _sort_model );

  if ( _tree_view->selectionModel () != 0 ) {
    connect ( _tree_view->selectionModel (),
              SIGNAL ( selectionChanged ( const QItemSelection &,
                                          const QItemSelection & ) ),
              this,
              SLOT ( items_selected ( const QItemSelection &,
                                      const QItemSelection & ) ) );
  }

  QHBoxLayout * lay_btn ( new QHBoxLayout );
  lay_btn->addWidget ( &_btn_collapse );
  lay_btn->addWidget ( &_btn_expand );
  lay_btn->addWidget ( expand_depth_label );
  lay_btn->addWidget ( &_expand_depth );
  lay_btn->addStretch ( 1 );
  lay_btn->addWidget ( &_btn_sort );

  QVBoxLayout * lay_vbox ( new QVBoxLayout );
  lay_vbox->addWidget ( title_label );
  lay_vbox->addWidget ( _tree_view );
  lay_vbox->addLayout ( lay_btn );

  setLayout ( lay_vbox );
}

Alsa_Config_View::~Alsa_Config_View () {}

void
Alsa_Config_View::set_model ( ::QSnd::Alsa_Config_Model * model_n )
{
  _alsa_cfg = model_n;
  _sort_model->setSourceModel ( _alsa_cfg );
}

bool
Alsa_Config_View::sorting_enabled () const
{
  return _sort_model->dynamicSortFilter ();
}

void
Alsa_Config_View::set_sorting_enabled ( bool flag_n )
{
  if ( flag_n != sorting_enabled () ) {
    QStringList lst;
    collect_expanded ( lst );

    _sort_model->setDynamicSortFilter ( flag_n );
    _sort_model->sort ( flag_n ? 0 : -1 );

    set_expanded ( lst );

    _btn_sort.setChecked ( flag_n );
    update_button_state ();
  }
}

void
Alsa_Config_View::reload_config ()
{
  if ( model () != 0 ) {
    if ( _expanded_items.isEmpty () ) {
      collect_expanded ( _expanded_items );
    }

    if ( model ()->reload () >= 0 ) {
      set_expanded ( _expanded_items );
      _expanded_items.clear ();
    }
  }
}

void
Alsa_Config_View::collect_expanded ( QStringList & lst_n,
                                     const QModelIndex & idx_base_n )
{
  if ( model () == 0 ) {
    return;
  }
  const int rows ( _alsa_cfg->rowCount ( idx_base_n ) );
  for ( int row = 0; row < rows; ++row ) {
    const QModelIndex & idx_src ( _alsa_cfg->index ( row, 0, idx_base_n ) );
    const QModelIndex & idx ( _sort_model->mapFromSource ( idx_src ) );
    if ( idx.isValid () ) {
      if ( _tree_view->isExpanded ( idx ) ) {
        lst_n.append ( _alsa_cfg->index_address_str ( idx_src ) );
        collect_expanded ( lst_n, idx_src );
      }
    }
  }
}

void
Alsa_Config_View::set_expanded ( QStringList & lst_n )
{
  if ( model () == 0 ) {
    return;
  }
  for ( int ii = 0; ii < lst_n.size (); ++ii ) {
    const QModelIndex & idx_src (
        _alsa_cfg->index_from_address ( lst_n[ ii ] ) );
    const QModelIndex & idx ( _sort_model->mapFromSource ( idx_src ) );
    if ( idx.isValid () ) {
      _tree_view->setExpanded ( idx, true );
    }
  }
}

void
Alsa_Config_View::expand_to_level ( bool expanded_n )
{
  int depth ( _expand_depth.value () );
  if ( ( _tree_view->selectionModel () != 0 ) && ( depth >= 0 ) ) {
    QModelIndexList indices ( _tree_view->selectionModel ()->selectedRows () );
    for ( int ii = 0; ii < indices.size (); ++ii ) {
      const QModelIndex & idx ( indices[ ii ] );
      if ( idx.isValid () ) {
        _tree_view->set_expanded_recursive ( idx, depth, expanded_n );
      }
    }
  }
}

void
Alsa_Config_View::expand_to_level ()
{
  expand_to_level ( true );
}

void
Alsa_Config_View::collapse_to_level ()
{
  expand_to_level ( false );
}

void
Alsa_Config_View::items_selected ( const QItemSelection &,
                                   const QItemSelection & )
{
  update_button_state ();
}

void
Alsa_Config_View::update_button_state ()
{
  bool is_enabled ( false );
  if ( _tree_view->selectionModel () != 0 ) {
    is_enabled = _tree_view->selectionModel ()->hasSelection ();
  }

  _btn_expand.setEnabled ( is_enabled );
  _btn_collapse.setEnabled ( is_enabled );
  _expand_depth.setEnabled ( is_enabled );
}

} // namespace Views
