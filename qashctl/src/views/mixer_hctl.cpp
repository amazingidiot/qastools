/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl.hpp"
#include "mwdg/mixer_device_setup.hpp"
#include "mwdg/mixer_hctl.hpp"
#include "mwdg/mixer_hctl_table_model.hpp"
#include "mwdg/mixer_hctl_tree_model.hpp"
#include "mwdg/mixer_style.hpp"
#include "qsnd/mixer_hctl.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include "qsnd/mixer_hctl_elem_group.hpp"
#include "qsnd/mixer_hctl_info_db.hpp"
#include "views/message_widget.hpp"
#include "views/mixer_hctl_setup.hpp"
#include "wdg/tree_view_kv.hpp"
#include <QCoreApplication>
#include <QHeaderView>
#include <QSplitter>
#include <iostream>

namespace Views
{

Mixer_HCTL::Mixer_HCTL ( QWidget * parent_n )
: ::Views::View_Base ( parent_n )
, _view_setup ( 0 )
{
  // Snd hctl mixer
  _snd_mixer = new ::QSnd::Mixer_HCTL ( this );
  connect ( _snd_mixer,
            SIGNAL ( sig_mixer_reload_request () ),
            this,
            SIGNAL ( sig_mdev_reload_request () ) );

  _default_iface_type_idx =
      _snd_mixer->info_db ()->iface_type_idx ( SND_CTL_ELEM_IFACE_MIXER );

  // Mixer widget
  _mixer_hctl = new ::MWdg::Mixer_HCTL;
  _mixer_hctl->set_ctl_info_db ( _snd_mixer->info_db () );

  // Tree model
  _tree_model = new ::MWdg::Mixer_HCTL_Tree_Model ( this );

  // Tree view
  _tree_view = new ::Wdg::Tree_View_KV;
  _tree_view->setHeaderHidden ( true );
  _tree_view->setTextElideMode ( Qt::ElideMiddle );
  _tree_view->setModel ( _tree_model );

  connect ( _tree_view,
            SIGNAL ( activated ( const QModelIndex & ) ),
            this,
            SLOT ( tree_element_selected ( const QModelIndex & ) ) );

  // Tree view container widget
  {
    QVBoxLayout * lay_wdg_tree ( new QVBoxLayout );
    lay_wdg_tree->setContentsMargins ( 0, 0, 0, 0 );
    lay_wdg_tree->addWidget ( _tree_view );
    _wdg_side.setLayout ( lay_wdg_tree );
  }

  // Table model
  _table_model = new ::MWdg::Mixer_HCTL_Table_Model ( this );

  // Table view
  {
    _table_view = new QTableView;
    _table_view->setModel ( _table_model );
    {
      QHeaderView * vhv ( _table_view->verticalHeader () );
      if ( vhv != 0 ) {
        vhv->hide ();
      }
    }
    _table_view->setSelectionMode ( QAbstractItemView::SingleSelection );
    _table_view->setSelectionBehavior ( QAbstractItemView::SelectRows );

    connect ( _table_view,
              SIGNAL ( activated ( const QModelIndex & ) ),
              this,
              SLOT ( table_element_selected ( const QModelIndex & ) ) );
  }

  // Central widget with stacked layout
  {
    QVBoxLayout * lay_center ( new QVBoxLayout );
    _wdg_center.setLayout ( lay_center );

    _lay_center_stack = new QStackedLayout;
    lay_center->addLayout ( _lay_center_stack );

    _lay_center_stack->setContentsMargins ( 0, 0, 0, 0 );
    _lay_center_stack->addWidget ( _table_view );
    _lay_center_stack->addWidget ( _mixer_hctl );
  }

  // Horizontal splitter
  {
    QSplitter * hsplit ( new QSplitter );
    hsplit->setOrientation ( Qt::Horizontal );
    hsplit->addWidget ( &_wdg_side );
    hsplit->addWidget ( &_wdg_center );

    hsplit->setCollapsible ( 0, false );
    hsplit->setCollapsible ( 1, false );
    hsplit->setStretchFactor ( 1, 8 );

    lay_stack ()->addWidget ( hsplit );
  }

  // Adjust margins
  {
    QLayout * lay;

    lay = _wdg_center.layout ();
    if ( lay != 0 ) {
      QMargins mgs ( lay->contentsMargins () );
      mgs.setRight ( 0 );
      mgs.setTop ( 0 );
      mgs.setBottom ( 0 );
      lay->setContentsMargins ( mgs );
    }

    lay = _mixer_hctl->layout ();
    if ( lay != 0 ) {
      lay->setContentsMargins ( 0, 0, 0, 0 );
    }
  }
}

Mixer_HCTL::~Mixer_HCTL ()
{
  set_mdev_setup ( 0 );
  set_inputs_setup ( 0 );
  set_view_setup ( 0 );
}

void
Mixer_HCTL::set_mdev_setup ( const ::MWdg::Mixer_Device_Setup * setup_n )
{
  if ( mdev_setup () != 0 ) {
    if ( _snd_mixer->is_open () ) {
      // Close mixer
      _mixer_hctl->set_snd_elem_group ( 0 );
      _tree_model->set_snd_mixer ( 0 );
      _table_model->set_snd_mixer ( 0 );
      _snd_mixer->close ();
    }
  }

  ::Views::View_Base::set_mdev_setup ( setup_n );

  if ( mdev_setup () != 0 ) {
    if ( !mdev_setup ()->ctl_addr.isEmpty () ) {
      // Open mixer
      _snd_mixer->open ( mdev_setup ()->ctl_addr );
      if ( _snd_mixer->is_open () ) {
        setup_view ();
        restore_state ();
      }
    }
  }
}

void
Mixer_HCTL::set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n )
{
  if ( inputs_setup () != 0 ) {
    _mixer_hctl->set_inputs_setup ( 0 );
  }

  ::Views::View_Base::set_inputs_setup ( setup_n );

  if ( inputs_setup () != 0 ) {
    _mixer_hctl->set_inputs_setup ( inputs_setup () );
  }
}

void
Mixer_HCTL::set_view_setup ( ::Views::View_Base_Setup * setup_n )
{
  if ( _view_setup != 0 ) {
    _mixer_hctl->set_image_alloc ( 0 );
    _mixer_hctl->set_wdg_style_db ( 0 );
  }

  _view_setup = dynamic_cast<::Views::Mixer_HCTL_Setup * > ( setup_n );

  if ( _view_setup != 0 ) {
    _mixer_hctl->set_wdg_style_db ( _view_setup->wdg_style_db );
    _mixer_hctl->set_image_alloc ( _view_setup->image_alloc );
    if ( _view_setup->wdg_style_db != 0 ) {
      // Get playback / capture colors
      {
        const unsigned int style_id ( ::MWdg::Mixer_Style::PLAYBACK );
        const QPalette & pal (
            _view_setup->wdg_style_db->palette ( style_id ) );
        _tree_model->set_snd_dir_foreground (
            0, pal.color ( QPalette::WindowText ) );
      }
      {
        const unsigned int style_id ( ::MWdg::Mixer_Style::CAPTURE );
        const QPalette & pal (
            _view_setup->wdg_style_db->palette ( style_id ) );
        _tree_model->set_snd_dir_foreground (
            1, pal.color ( QPalette::WindowText ) );
      }
    }
    setup_view ();
    restore_state ();
  }
}

void
Mixer_HCTL::setup_view ()
{
  if ( ( mdev_setup () == 0 ) || ( _view_setup == 0 ) ) {
    return;
  }

  int lay_stack_idx ( 0 );

  // Open mixer
  if ( !mdev_setup ()->ctl_addr.isEmpty () ) {
    if ( _snd_mixer->is_open () ) {
      _tree_model->set_snd_mixer ( _snd_mixer );
      _table_model->set_snd_mixer ( _snd_mixer );
      lay_stack_idx = 1;
    } else {
      message_wdg ()->set_mixer_open_fail ( mdev_setup ()->ctl_addr,
                                            _snd_mixer->err_message (),
                                            _snd_mixer->err_func () );
    }
  } else {
    message_wdg ()->set_no_device ();
  }

  expand_tree_items ();
  adjust_table_columns ();

  lay_stack ()->setCurrentIndex ( lay_stack_idx );
}

void
Mixer_HCTL::restore_state ()
{
  if ( ( mdev_setup () == 0 ) || ( _view_setup == 0 ) ) {
    return;
  }

  QModelIndex idx (
      _tree_model->elem_desc_index ( _view_setup->iface_name,
                                     _view_setup->elem_grp_name,
                                     _view_setup->elem_grp_index ) );

  if ( !idx.isValid () ) {
    idx = _tree_model->index ( 0, 0 );
  }
  if ( idx.isValid () ) {
    _tree_view->setCurrentIndex ( idx );
  }
}

void
Mixer_HCTL::expand_tree_items ()
{
  const QModelIndex base_idx ( _tree_model->invisibleRootItem ()->index () );

  _tree_view->expand ( base_idx );

  const unsigned int num_rows ( _tree_model->rowCount ( base_idx ) );
  if ( num_rows == 1 ) {
    for ( unsigned int ii = 0; ii < num_rows; ++ii ) {
      QModelIndex mod_idx ( _tree_model->index ( ii, 0, base_idx ) );
      _tree_view->expand ( mod_idx );
    }
  }
}

void
Mixer_HCTL::adjust_table_columns ()
{
  _table_view->resizeColumnsToContents ();
  _table_view->resizeRowsToContents ();
}

void
Mixer_HCTL::tree_element_selected ( const QModelIndex & idx_n )
{
  if ( !idx_n.isValid () ) {
    return;
  }

  ::QSnd::Mixer_HCTL_Elem_Group * elem_grp ( 0 );
  unsigned int elem_grp_index ( 0 );
  unsigned int iface_type_index ( 0 );

  unsigned int found_type = _tree_model->index_data (
      idx_n, &elem_grp, &elem_grp_index, &iface_type_index );

  if ( found_type == 1 ) {
    //::std::cout << "Element type selected\n";
    _table_model->set_iface_type_idx ( iface_type_index );
    _lay_center_stack->setCurrentIndex ( 0 );
    adjust_table_columns ();
  } else if ( found_type == 2 ) {
    //::std::cout << "Element group selected\n";
    _mixer_hctl->set_snd_elem_group ( elem_grp, elem_grp_index );
    _lay_center_stack->setCurrentIndex ( 1 );
  } else if ( found_type == 3 ) {
    //::std::cout << "Element selected\n";
    _mixer_hctl->set_snd_elem_group ( elem_grp, elem_grp_index );
    _lay_center_stack->setCurrentIndex ( 1 );
  }

  // Save selected state in the setup class
  if ( _view_setup != 0 ) {
    if ( found_type > 0 ) {
      _view_setup->iface_name =
          _snd_mixer->info_db ()->iface_name ( iface_type_index );
      if ( found_type == 1 ) {
        _view_setup->elem_grp_name.clear ();
      } else {
        _view_setup->elem_grp_name = elem_grp->elem ( 0 )->elem_name ();
        _view_setup->elem_grp_index = elem_grp_index;
      }
    }
  }
}

void
Mixer_HCTL::table_element_selected ( const QModelIndex & idx_n )
{
  ::QSnd::Mixer_HCTL_Elem * elem ( _table_model->elem ( idx_n ) );
  if ( elem != 0 ) {
    QModelIndex tree_idx ( _tree_model->elem_index ( elem ) );
    if ( tree_idx.isValid () ) {
      _tree_view->setCurrentIndex ( tree_idx );
    }
  }
}

} // namespace Views
