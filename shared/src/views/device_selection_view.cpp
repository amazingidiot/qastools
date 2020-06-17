/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "device_selection_view.hpp"
#include "mwdg/controls_view.hpp"
#include "mwdg/ctl_arg_view.hpp"
#include "mwdg/ctl_arg_view_card.hpp"
#include "mwdg/ctl_arg_view_integer.hpp"
#include "mwdg/ctl_arg_view_string.hpp"
#include "qsnd/controls_model.hpp"
#include "qsnd/ctl_address.hpp"
#include "qsnd/ctl_format.hpp"
#include "views/device_selection_view_setup.hpp"
#include <QContextMenuEvent>
#include <QLabel>
#include <QMenu>
#include <iostream>

namespace Views
{

Device_Selection_View::Device_Selection_View ( QWidget * parent_n )
: QWidget ( parent_n )
, _view_setup ( 0 )
, _lay_arg_views ( 0 )
, _selected_ctl_checks_good ( false )
, _silent_ctl_change ( false )
, _silent_arg_change ( false )
{
  // Strings
  _str_type_card = "card";
  _str_type_string = "string";
  _str_type_integer = "integer";

  // View close action
  _act_close = new QAction ( this );
  _act_close->setText ( tr ( "&Close device selection" ) );
  if ( QIcon::hasThemeIcon ( "window-close" ) ) {
    _act_close->setIcon ( QIcon::fromTheme ( "window-close" ) );
  }
  connect ( _act_close,
            SIGNAL ( triggered ( bool ) ),
            this,
            SIGNAL ( sig_close () ) );

  // Controls database
  connect ( &_controls_db,
            &::QSnd::Controls_Database::sig_reload_required,
            this,
            &Device_Selection_View::reload_database );

  // Control selection model and view
  {
    _controls_model = new ::QSnd::Controls_Model ( this );
    _controls_model->set_controls_db ( &_controls_db );

    _controls_view = new ::MWdg::Controls_View ( this );
    _controls_view->setModel ( _controls_model );

    connect (
        _controls_view->selectionModel (),
        SIGNAL ( currentChanged ( const QModelIndex &, const QModelIndex & ) ),
        this,
        SLOT ( control_changed ( const QModelIndex &, const QModelIndex & ) ) );
  }

  {
    QSizePolicy policy ( sizePolicy () );
    policy.setHorizontalPolicy ( QSizePolicy::Preferred );
    policy.setVerticalPolicy ( QSizePolicy::Expanding );
    setSizePolicy ( policy );
  }

  QLabel * controls_label = new QLabel;
  controls_label->setText ( tr ( "Mixer device" ) );
  {
    QFont fnt ( controls_label->font () );
    fnt.setBold ( true );
    controls_label->setFont ( fnt );
  }

  _lay_arg_views = new QVBoxLayout;
  {
    QVBoxLayout * lay_args ( new QVBoxLayout );
    int vspace ( fontMetrics ().height () * 2 / 3 );
    lay_args->setContentsMargins ( 0, vspace, 0, 0 );
    lay_args->addLayout ( _lay_arg_views, 0 );
    lay_args->addStretch ( 1 );

    QVBoxLayout * lay_vbox ( new QVBoxLayout );
    lay_vbox->addWidget ( controls_label, 0 );
    lay_vbox->addWidget ( _controls_view, 1 );
    lay_vbox->addLayout ( lay_args, 0 );
    setLayout ( lay_vbox );
  }
}

Device_Selection_View::~Device_Selection_View ()
{
  clear_arg_views ();
}

void
Device_Selection_View::set_view_setup (
    ::Views::Device_Selection_View_Setup * setup_n )
{
  _view_setup = setup_n;
  if ( _view_setup != 0 ) {
    selection_db_sanitize ();
  }
}

void
Device_Selection_View::compile_ctl_address ( ::QSnd::CTL_Address & ctl_addr_n )
{
  //::std::cout << "Device_Selection_View::compile_ctl_address: " <<
  // ctl_addr_n.addr_str().toLocal8Bit().data() << "\n";

  const QModelIndex & cidx ( _controls_view->currentIndex () );
  auto * ctl_format = _controls_model->control_by_index ( cidx );
  if ( ctl_format != nullptr ) {
    // Control name
    ctl_addr_n.set_ctl_name ( ctl_format->ctl_name () );
    // Control arguments
    for ( auto & view : _arg_views ) {
      const QString & arg_name = view->ctl_arg ().arg_name;
      const QString & arg_str = view->arg_string ();
      if ( !arg_str.isEmpty () ) {
        ctl_addr_n.append_arg (
            ::QSnd::CTL_Address_Argument ( arg_name, arg_str ) );
      }
    }
  } else {
    ctl_addr_n.clear ();
  }
}

void
Device_Selection_View::silent_select_ctl (
    const ::QSnd::CTL_Address & ctl_addr_n )
{
  if ( !_selected_ctl.match ( ctl_addr_n ) ) {
    _silent_ctl_change = true;

    // Clear previous selection
    clear_arg_views ();
    _selected_ctl_format.clear ();
    _selected_ctl.clear ();
    _controls_view->clearSelection ();

    // Find control format model index from name
    const QModelIndex midx =
        _controls_model->control_index ( ctl_addr_n.ctl_name () );

    // It's safer to set a current index even if it is invalid.
    // Otherwise Qt may pick the first index just to have one selected.
    _controls_view->setCurrentIndex ( midx );
    if ( midx.isValid () ) {
      // The address in the database will be used by the arg view restore
      selection_db_commit ( ctl_addr_n );
      {
        auto * format = _controls_model->control_by_index ( midx );
        if ( format != nullptr ) {
          _selected_ctl_format = *format;
        }
      }

      // Create and update argument inputs
      _silent_arg_change = true;
      create_arg_views ();
      restore_arg_views ();
      _silent_arg_change = false;

      compile_ctl_address ( _selected_ctl );
      update_selected_ctl_checks ();
    }

    _silent_ctl_change = false;
  }
}

void
Device_Selection_View::reload_database ()
{
  //::std::cout << "Device_Selection_View::reload_database" << "\n";
  if ( _view_setup != 0 ) {
    // Remember selection
    const ::QSnd::CTL_Address ctl_addr ( _selected_ctl );
    const bool ctl_check_good ( _selected_ctl_checks_good );

    // Clear selection
    _selected_ctl.clear ();
    _selected_ctl_format.clear ();

    // Silent reload
    _silent_ctl_change = true;
    _controls_db.reload ();
    selection_db_sanitize ();
    _silent_ctl_change = false;

    // Non silent selection restore
    silent_select_ctl ( ctl_addr );

    // Look for changes
    if ( !_selected_ctl.match ( ctl_addr ) ) {
      emit sig_control_selected ();
    } else if ( _selected_ctl_checks_good != ctl_check_good ) {
      emit sig_control_reload ();
    }
  }
}

void
Device_Selection_View::control_changed ( const QModelIndex & cur_idx_n,
                                         const QModelIndex & )
{
  control_changed ( cur_idx_n );
}

void
Device_Selection_View::control_changed ( const QModelIndex & idx_n )
{
  //::std::cout << "Device_Selection_View::control_changed " << "\n";
  if ( _silent_ctl_change ) {
    return;
  }

  {
    auto * ctl_format = _controls_model->control_by_index ( idx_n );
    if ( ( ctl_format != nullptr ) &&
         ( _selected_ctl_format != *ctl_format ) ) {
      // Clear argument inputs
      clear_arg_views ();
      // Update format
      _selected_ctl_format = *ctl_format;
      // Create new argument inputs on demand
      if ( _selected_ctl_format.is_valid () ) {
        _silent_arg_change = true;
        create_arg_views ();
        restore_arg_views ();
        _silent_arg_change = false;
      }
    }
  }
  update_selected_ctl ();
}

void
Device_Selection_View::control_arg_changed ()
{
  //::std::cout << "Device_Selection_View::control_arg_changed " << "\n";

  if ( !( _silent_ctl_change || _silent_arg_change ) ) {
    if ( update_selected_ctl () ) {
      // Remember _selected_ctl for a later visit
      selection_db_commit ( _selected_ctl );
    }
  }
}

void
Device_Selection_View::update_selected_ctl_checks ()
{
  _selected_ctl_checks_good = false;
  if ( _selected_ctl.is_valid () ) {
    _selected_ctl_checks_good = true;

    const unsigned int num_args ( _selected_ctl.num_args () );
    if ( num_args <= _selected_ctl_format.num_args () ) {
      for ( unsigned int ii = 0; ii != num_args; ++ii ) {
        const ::QSnd::CTL_Address_Argument & arg ( _selected_ctl.arg ( ii ) );
        if ( arg.arg_value.isEmpty () ) {
          // Empty arguments are bad
          _selected_ctl_checks_good = false;
        } else {
          const ::QSnd::CTL_Format_Argument & arg_format (
              _selected_ctl_format.arg ( ii ) );
          if ( arg_format.arg_name.compare ( _str_type_card,
                                             Qt::CaseInsensitive ) == 0 ) {
            unsigned int card_id ( arg.arg_value.toUInt () );
            if ( !_cards_model.card_info_by_card_index ( card_id ) ) {
              _selected_ctl_checks_good = false;
            }
          }
        }
      }
    } else {
      _selected_ctl_checks_good = false;
    }
  }
}

bool
Device_Selection_View::update_selected_ctl ()
{
  //::std::cout << "Device_Selection_View::update_selected_ctl " << "\n";
  bool res ( false );
  {
    ::QSnd::CTL_Address ctl_addr;
    compile_ctl_address ( ctl_addr );
    if ( !_selected_ctl.match ( ctl_addr ) ) {
      _selected_ctl = ctl_addr;
      emit sig_control_selected ();
      res = true;
    }
  }
  update_selected_ctl_checks ();
  return res;
}

const ::QSnd::CTL_Address *
Device_Selection_View::selection_db_find ( const QString & ctl_name_n ) const
{
  const ::QSnd::CTL_Address * res ( 0 );
  if ( _view_setup != 0 ) {
    for ( int ii = 0; ii < _view_setup->selection_db.size (); ++ii ) {
      const ::QSnd::CTL_Address & addr ( _view_setup->selection_db[ ii ] );
      if ( addr.ctl_name () == ctl_name_n ) {
        res = &addr;
        break;
      }
    }
  }
  return res;
}

void
Device_Selection_View::selection_db_commit (
    const ::QSnd::CTL_Address & ctl_addr_n )
{
  if ( ( _view_setup != 0 ) && ctl_addr_n.is_valid () ) {
    bool found ( false );
    for ( int ii = 0; ii < _view_setup->selection_db.size (); ++ii ) {
      ::QSnd::CTL_Address & addr ( _view_setup->selection_db[ ii ] );
      if ( addr.ctl_name () == ctl_addr_n.ctl_name () ) {
        addr = ctl_addr_n;
        found = true;
        break;
      }
    }
    if ( !found ) {
      _view_setup->selection_db.append ( ctl_addr_n );
    }
  }
}

void
Device_Selection_View::selection_db_sanitize ()
{
  if ( _view_setup != 0 ) {
    // Remove entries from selection db that don't exist in the
    // available controls database
    typedef Device_Selection_View_Setup::Selection_DB::iterator Iterator;
    Iterator itc ( _view_setup->selection_db.begin () );
    while ( itc != _view_setup->selection_db.end () ) {
      if ( _controls_db.find_control_format ( itc->ctl_name () ) == 0 ) {
        itc = _view_setup->selection_db.erase ( itc );
      } else {
        ++itc;
      }
    }
  }
}

void
Device_Selection_View::clear_arg_views ()
{
  for ( int ii = 0; ii < _arg_views.size (); ++ii ) {
    delete _arg_views[ ii ];
  }
  _arg_views.clear ();
}

void
Device_Selection_View::create_arg_views ()
{
  //::std::cout << "Device_Selection_View::create_arg_views"\n";
  if ( _selected_ctl_format.is_valid () ) {
    const unsigned int num_args ( _selected_ctl_format.num_args () );
    for ( unsigned int ii = 0; ii != num_args; ++ii ) {
      const ::QSnd::CTL_Format_Argument & ctl_arg (
          _selected_ctl_format.arg ( ii ) );

      ::MWdg::CTL_Arg_View * arg_view ( 0 );
      if ( ctl_arg.arg_name.compare ( _str_type_card, Qt::CaseInsensitive ) ==
           0 ) {
        arg_view = new ::MWdg::CTL_Arg_View_Card;
      } else if ( ctl_arg.arg_type.compare ( _str_type_string,
                                             Qt::CaseInsensitive ) == 0 ) {
        arg_view = new ::MWdg::CTL_Arg_View_String;
      } else if ( ctl_arg.arg_type.compare ( _str_type_integer,
                                             Qt::CaseInsensitive ) == 0 ) {
        arg_view = new ::MWdg::CTL_Arg_View_Integer;
      }

      if ( arg_view != 0 ) {
        arg_view->set_ctl_db ( &_controls_db );
        arg_view->set_cards_model ( &_cards_model );
        arg_view->set_ctl_arg ( ctl_arg );
        connect ( arg_view,
                  SIGNAL ( sig_arg_changed () ),
                  this,
                  SLOT ( control_arg_changed () ) );
        _arg_views.append ( arg_view );
        _lay_arg_views->addWidget ( arg_view );
      }
    }
  }
}

void
Device_Selection_View::restore_arg_views ()
{
  if ( _selected_ctl_format.is_valid () &&
       ( _selected_ctl_format.num_args () != 0 ) ) {
    const ::QSnd::CTL_Address * ctl_addr (
        selection_db_find ( _selected_ctl_format.ctl_name () ) );
    if ( ctl_addr != 0 ) {
      const unsigned int num_args ( ::std::min (
          ctl_addr->num_args (), _selected_ctl_format.num_args () ) );
      for ( unsigned int ii = 0; ii != num_args; ++ii ) {
        const ::QSnd::CTL_Address_Argument & ctl_arg ( ctl_addr->arg ( ii ) );
        ::MWdg::CTL_Arg_View * arg_view ( _arg_views[ ii ] );
        bool name_match ( true );
        if ( !ctl_arg.arg_name.isEmpty () ) {
          name_match = ( arg_view->ctl_arg ().arg_name == ctl_arg.arg_name );
        }
        if ( name_match ) {
          arg_view->set_arg_string ( ctl_arg.arg_value );
        }
      }
    }
  }
}

void
Device_Selection_View::contextMenuEvent ( QContextMenuEvent * event_n )
{
  QMenu cmenu ( this );
  cmenu.addAction ( _act_close );
  cmenu.exec ( event_n->globalPos () );
}

} // namespace Views
