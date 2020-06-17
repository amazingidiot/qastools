/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_switches.hpp"
#include "mwdg/event_types.hpp"
#include "mwdg/inputs_setup.hpp"
#include "mwdg/mixer_gui_state.hpp"
#include "mwdg/mixer_simple_setup.hpp"
#include "mwdg/mixer_switches_proxies_group.hpp"
#include "qsnd/mixer_simple.hpp"
#include "wdg/pad_proxies_column.hpp"
#include "wdg/pad_proxies_group.hpp"
#include "wdg/switches_area.hpp"
#include "wdg/switches_pad.hpp"
#include <QContextMenuEvent>
#include <QCoreApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <iostream>

namespace MWdg
{

Mixer_Switches::Mixer_Switches ( QWidget * parent_n )
: QWidget ( parent_n )
, _mixer_setup ( 0 )
, _inputs_setup ( 0 )
, _separation_requested ( false )
, _act_toggle_joined ( this )
{
  _switches_area = new ::Wdg::Switches_Area;
  _switches_area->setFrameStyle ( QFrame::NoFrame );

  _switches_pad = new ::Wdg::Switches_Pad;
  _switches_pad->hide ();
  _switches_pad->installEventFilter ( this );

  connect ( _switches_pad,
            SIGNAL ( sig_focus_changed () ),
            this,
            SLOT ( update_focus_proxies () ) );

  // Actions
  _act_toggle_joined.setIcon ( QIcon::fromTheme ( "object-flip-horizontal" ) );

  connect ( &_act_toggle_joined,
            SIGNAL ( triggered ( bool ) ),
            this,
            SLOT ( action_toggle_joined () ) );

  // Context menu
  _cmenu.addAction ( &_act_toggle_joined );

  // Center widget layout
  {
    QVBoxLayout * lay_vbox = new QVBoxLayout ();
    lay_vbox->setContentsMargins ( 0, 0, 0, 0 );
    lay_vbox->addWidget ( _switches_area );
    setLayout ( lay_vbox );
  }
}

Mixer_Switches::~Mixer_Switches ()
{
  set_mixer_setup ( 0 );
  delete _switches_pad;
  delete _switches_area;
}

void
Mixer_Switches::set_mixer_setup ( const Mixer_Simple_Setup * setup_n )
{
  _cmenu.close ();

  if ( mixer_setup () != 0 ) {
    if ( mixer_setup ()->mixer_simple != 0 ) {
      disconnect ( mixer_setup ()->mixer_simple, 0, this, 0 );

      show_visible_proxies_sets ( false );
      clear_proxies_groups ();
    }
  }

  _mixer_setup = setup_n;

  if ( mixer_setup () != 0 ) {
    if ( mixer_setup ()->mixer_simple != 0 ) {
      create_proxies_groups ();
      rebuild_visible_proxies_list ();
      show_visible_proxies_sets ( true );
    }
  }
}

void
Mixer_Switches::set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n )
{
  _inputs_setup = setup_n;

  if ( inputs_setup () != 0 ) {
    _act_toggle_joined.setShortcut ( inputs_setup ()->ks_toggle_joined );
  }
}

void
Mixer_Switches::show_visible_proxies_sets ( bool flag_n )
{
  if ( flag_n ) {
    if ( _proxies_groups_vis.size () > 0 ) {
      _switches_pad->set_proxies_groups ( _proxies_groups_pass );
      _switches_area->set_widget ( _switches_pad );

      _switches_pad->setAutoFillBackground ( false );
      _switches_pad->show ();
    }
    updateGeometry ();
  } else {
    if ( _proxies_groups_vis.size () > 0 ) {
      _switches_pad->hide ();
      _switches_area->take_widget ();
      _switches_pad->clear_proxies_groups ();
    }
  }
}

void
Mixer_Switches::clear_proxies_groups ()
{
  for ( int ii = 0; ii < _proxies_groups.size (); ++ii ) {
    delete _proxies_groups[ ii ];
  }
  _proxies_groups.clear ();
  _proxies_groups_vis.clear ();
  _proxies_groups_pass.clear ();
}

void
Mixer_Switches::create_proxies_groups ()
{
  ::QSnd::Mixer_Simple * snd_mixer ( 0 );
  if ( mixer_setup () != 0 ) {
    snd_mixer = mixer_setup ()->mixer_simple;
  }
  if ( snd_mixer == 0 ) {
    return;
  }

  //::std::cout << "Mixer_Switches::init_widgets\n";

  QString ttip;

  for ( unsigned int ii = 0; ii < snd_mixer->num_elems (); ++ii ) {
    ::QSnd::Mixer_Simple_Elem * qsme ( snd_mixer->elem ( ii ) );

    // Types means either enum or switch
    for ( unsigned int itype = 0; itype < 2; ++itype ) {
      for ( unsigned int snd_dir = 0; snd_dir < 2; ++snd_dir ) {
        if ( qsme->has_volume ( snd_dir ) ) {
          continue;
        }

        bool cr_enum ( itype == 0 );
        bool cr_switch ( itype == 1 );

        if ( ( cr_enum && qsme->has_enum ( snd_dir ) ) ||
             ( cr_switch && qsme->has_switch ( snd_dir ) ) ) {
          ::MWdg::Mixer_Switches_Proxies_Group * mspg (
              new ::MWdg::Mixer_Switches_Proxies_Group ( this ) );
          mspg->set_mixer_simple_elem ( qsme );
          mspg->set_snd_dir ( snd_dir );
          mspg->set_group_name ( qsme->display_name () );

          {
            ttip = "<div><b>";
            ttip += mspg->group_name ();
            ttip += "</b></div>\n";
            mspg->set_tool_tip ( ttip );
          }

          if ( mspg->should_be_separated () ) {
            setup_proxies_group_separate ( mspg );
          } else {
            setup_proxies_group_joined ( mspg );
          }

          if ( mspg->num_columns () > 0 ) {
            _proxies_groups.append ( mspg );
          } else {
            delete mspg;
          }
        }
      }
    }
  }
}

void
Mixer_Switches::setup_proxies_group_joined (
    ::MWdg::Mixer_Switches_Proxies_Group * mspg_n )
{
  const ::QSnd::Mixer_Simple_Elem * qsme ( mspg_n->mixer_simple_elem () );
  const unsigned int snd_dir ( mspg_n->snd_dir () );

  mspg_n->clear_columns ();
  mspg_n->set_is_joined ( true );

  unsigned int num_channels ( 0 );
  if ( qsme->has_switch ( snd_dir ) ) {
    num_channels = qsme->num_switch_channels ( snd_dir );
  } else if ( qsme->has_enum ( snd_dir ) ) {
    num_channels = qsme->num_enum_channels ( snd_dir );
  }

  if ( num_channels > 0 ) {
    create_proxy ( mspg_n, 0 );
  }

  mspg_n->update_mixer_values ();
}

void
Mixer_Switches::setup_proxies_group_separate (
    ::MWdg::Mixer_Switches_Proxies_Group * mspg_n )
{
  const ::QSnd::Mixer_Simple_Elem * qsme ( mspg_n->mixer_simple_elem () );
  const unsigned int snd_dir ( mspg_n->snd_dir () );

  mspg_n->clear_columns ();
  mspg_n->set_is_joined ( false );

  unsigned int num_channels ( 0 );
  if ( qsme->has_switch ( snd_dir ) ) {
    num_channels = qsme->num_switch_channels ( snd_dir );
  } else if ( qsme->has_enum ( snd_dir ) ) {
    num_channels = qsme->num_enum_channels ( snd_dir );
  }

  for ( unsigned int ii = 0; ii < num_channels; ++ii ) {
    create_proxy ( mspg_n, ii );
  }

  mspg_n->update_mixer_values ();
}

void
Mixer_Switches::create_proxy ( ::MWdg::Mixer_Switches_Proxies_Group * mspg_n,
                               int channel_idx_n )
{
  ::QSnd::Mixer_Simple_Elem * qsme ( mspg_n->mixer_simple_elem () );
  const unsigned int snd_dir ( mspg_n->snd_dir () );

  QString iname;
  if ( mspg_n->is_joined () ) {
    iname = mspg_n->group_name ();
  } else {
    iname = tr ( "%1 (%2)" );
    iname = iname.arg ( QCoreApplication::translate (
        "ALSA::Channel_Name", qsme->channel_name ( snd_dir, channel_idx_n ) ) );
    iname = iname.arg ( qsme->channel ( snd_dir, channel_idx_n ) );
  }

  QString ttip;
  ttip += "<div><b>";
  ttip += mspg_n->group_name ();
  ttip += "</b></div>\n";

  ttip += "<div>";
  if ( snd_dir == 0 ) {
    if ( qsme->has_enum ( snd_dir ) ) {
      ttip += tr ( "Playback selection" );
    } else {
      ttip += tr ( "Playback switch" );
    }
  } else {
    if ( qsme->has_enum ( snd_dir ) ) {
      ttip += tr ( "Capture selection" );
    } else {
      ttip += tr ( "Capture switch" );
    }
  }
  ttip += "</div>\n";

  if ( !mspg_n->is_joined () ) {
    ttip += "<div>";
    ttip += iname;
    ttip += "</div>";
  }

  ::Wdg::Pad_Proxies_Column * sppc ( new ::Wdg::Pad_Proxies_Column );

  if ( qsme->has_enum ( snd_dir ) ) {
    ::MWdg::Mixer_Switches_Proxy_Enum * msp (
        new ::MWdg::Mixer_Switches_Proxy_Enum );

    msp->set_mixer_simple_elem ( qsme );
    msp->set_snd_dir ( snd_dir );
    msp->set_is_joined ( mspg_n->is_joined () );
    msp->set_channel_idx ( channel_idx_n );
    msp->set_enum_num_items ( qsme->enum_item_names ().size () );

    msp->set_item_name ( iname );
    msp->set_tool_tip ( ttip );
    sppc->set_enum_proxy ( msp );
  } else if ( qsme->has_switch ( snd_dir ) ) {
    ::MWdg::Mixer_Switches_Proxy_Switch * msp (
        new ::MWdg::Mixer_Switches_Proxy_Switch );

    msp->set_mixer_simple_elem ( qsme );
    msp->set_snd_dir ( snd_dir );
    msp->set_is_joined ( mspg_n->is_joined () );
    msp->set_channel_idx ( channel_idx_n );

    msp->set_item_name ( iname );
    msp->set_tool_tip ( ttip );
    sppc->set_switch_proxy ( msp );
  } else {
    delete sppc;
    sppc = 0;
  }

  if ( sppc != 0 ) {
    mspg_n->append_column ( sppc );
  }
}

bool
Mixer_Switches::should_be_visible (
    const ::MWdg::Mixer_Switches_Proxies_Group * mspg_n ) const
{
  const ::QSnd::Mixer_Simple_Elem * qsme ( mspg_n->mixer_simple_elem () );
  const unsigned int snd_dir ( mspg_n->snd_dir () % 2 );
  return ( qsme->is_active () && mixer_setup ()->show_stream[ snd_dir ] );
}

void
Mixer_Switches::rebuild_visible_proxies_list ()
{
  _proxies_groups_vis.clear ();
  _proxies_groups_pass.clear ();

  for ( int ii = 0; ii < _proxies_groups.size (); ++ii ) {
    ::MWdg::Mixer_Switches_Proxies_Group * mspg ( _proxies_groups[ ii ] );
    mspg->set_is_visible ( should_be_visible ( mspg ) );
    if ( mspg->is_visible () ) {
      // Separate on demand
      if ( mspg->needs_separation () ) {
        separate_proxies_group ( mspg );
      }
      _proxies_groups_vis.append ( mspg );
      _proxies_groups_pass.append ( mspg );
    }
  }
}

void
Mixer_Switches::update_focus_proxies ()
{
  if ( _switches_pad->focus_info ().has_focus ) {
    // Find focus proxies_group
    const int idx ( _switches_pad->focus_info ().group_idx );
    if ( idx < _proxies_groups_vis.size () ) {
      _act_proxies_group = _proxies_groups_vis[ idx ];
      _act_proxies_column = _switches_pad->focus_info ().column_idx;
    }
  }
}

void
Mixer_Switches::acquire_gui_state ( ::MWdg::Mixer_GUI_State_Proxy & state_n )
{
  if ( _act_proxies_group != 0 ) {
    ::MWdg::Mixer_Switches_Proxies_Group * mspg ( _act_proxies_group );
    state_n.group_name = mspg->group_name ();
    state_n.snd_dir = mspg->snd_dir ();
    state_n.column_idx = mspg->focus_column ();
    state_n.has_focus = _switches_pad->focus_info ().has_focus;
  }
}

void
Mixer_Switches::restore_gui_state (
    const ::MWdg::Mixer_GUI_State_Proxy & state_n )
{
  {
    _act_proxies_group = find_visible_proxy ( state_n );
    _act_proxies_column = state_n.column_idx;
  }

  // Restore focus
  if ( state_n.has_focus ) {
    if ( _act_proxies_group != 0 ) {
      _switches_pad->set_focus_proxy ( _act_proxies_group->group_index (),
                                       state_n.column_idx );
    }
  }

  if ( _cmenu.isVisible () ) {
    context_menu_update ();
  }
}

Mixer_Switches_Proxies_Group *
Mixer_Switches::find_visible_proxy ( const Mixer_GUI_State_Proxy & prox_id_n )
{
  ::MWdg::Mixer_Switches_Proxies_Group * mspg_res ( 0 );

  if ( !prox_id_n.is_clear () ) {
    for ( int ii = 0; ii < _proxies_groups_vis.size (); ++ii ) {
      ::MWdg::Mixer_Switches_Proxies_Group * mspg ( _proxies_groups_vis[ ii ] );
      if ( ( mspg->group_name () == prox_id_n.group_name ) &&
           ( mspg->snd_dir () == prox_id_n.snd_dir ) ) {
        mspg_res = mspg;
        break;
      }
    }
  }

  return mspg_res;
}

void
Mixer_Switches::action_toggle_joined ()
{
  //::std::cout << "Mixer_Switches::action_toggle_joined" << "\n";

  if ( _act_proxies_group != 0 ) {
    toggle_joined_separated ( _act_proxies_group );
  }
}

void
Mixer_Switches::toggle_joined_separated (
    ::MWdg::Mixer_Switches_Proxies_Group * mspg_n )
{
  if ( mspg_n == 0 ) {
    return;
  }

  if ( !mspg_n->can_be_separated () ) {
    return;
  }

  ::MWdg::Mixer_GUI_State_Proxy proxy_state;
  acquire_gui_state ( proxy_state );
  show_visible_proxies_sets ( false );

  if ( mspg_n->is_joined () ) {
    separate_proxies_group ( mspg_n );
  } else {
    join_proxies_group ( mspg_n );
  }

  show_visible_proxies_sets ( true );
  restore_gui_state ( proxy_state );
}

void
Mixer_Switches::join_proxies_group (
    ::MWdg::Mixer_Switches_Proxies_Group * mspg_n )
{
  if ( !mspg_n->is_joined () ) {
    ::QSnd::Mixer_Simple_Elem * qsme ( mspg_n->mixer_simple_elem () );
    qsme->level_switches ( mspg_n->snd_dir () );

    setup_proxies_group_joined ( mspg_n );
    if ( _cmenu.isVisible () ) {
      context_menu_update ();
    }
  }
}

void
Mixer_Switches::separate_proxies_group (
    ::MWdg::Mixer_Switches_Proxies_Group * mspg_n )
{
  if ( mspg_n->is_joined () && mspg_n->can_be_separated () ) {
    setup_proxies_group_separate ( mspg_n );
    if ( _cmenu.isVisible () ) {
      context_menu_update ();
    }
  }
}

void
Mixer_Switches::separate_where_requested ()
{
  bool visible_request ( false );
  for ( int ii = 0; ii < _proxies_groups_vis.size (); ++ii ) {
    ::MWdg::Mixer_Switches_Proxies_Group * mspg ( _proxies_groups_vis[ ii ] );
    if ( mspg->separation_request () ) {
      visible_request = true;
      break;
    }
  }

  if ( visible_request ) {
    ::MWdg::Mixer_GUI_State_Proxy proxy_state;
    acquire_gui_state ( proxy_state );
    show_visible_proxies_sets ( false );

    for ( int ii = 0; ii < _proxies_groups_vis.size (); ++ii ) {
      ::MWdg::Mixer_Switches_Proxies_Group * mspg ( _proxies_groups_vis[ ii ] );
      if ( mspg->separation_request () ) {
        mspg->set_separation_request ( false );
        separate_proxies_group ( mspg );
      }
    }

    show_visible_proxies_sets ( true );
    restore_gui_state ( proxy_state );
  }
}

bool
Mixer_Switches::context_menu_start ( const QPoint & pos_n )
{
  bool res ( false );

  if ( !_cmenu.isVisible () && ( _switches_pad->focus_info ().has_focus ) &&
       ( _act_proxies_group != 0 ) ) {
    if ( context_menu_update () > 0 ) {
      _act_proxies_group->set_notify_value_change ( true );

      _cmenu.setTitle ( _act_proxies_group->group_name () );
      _cmenu.popup ( pos_n );
      res = true;
    }
  }

  return res;
}

void
Mixer_Switches::context_menu_cleanup_behind ()
{
  if ( _act_proxies_group != 0 ) {
    _act_proxies_group->set_notify_value_change ( false );
  }
}

unsigned int
Mixer_Switches::context_menu_update ()
{
  unsigned int act_vis ( 0 );

  ::MWdg::Mixer_Switches_Proxies_Group * mspg ( _act_proxies_group );

  if ( mspg == 0 ) {
    _cmenu.close ();
    return act_vis;
  }

  const ::QSnd::Mixer_Simple_Elem * qsme ( mspg->mixer_simple_elem () );

  // Split/Join and level channels
  {
    const bool vis_separate ( qsme->num_channels ( mspg->snd_dir () ) > 1 );

    _act_toggle_joined.setVisible ( vis_separate );
    if ( vis_separate ) {
      if ( inputs_setup () != 0 ) {
        const QString * str;
        if ( mspg->num_columns () <= 1 ) {
          str = &inputs_setup ()->ts_split_channels;
        } else {
          str = &inputs_setup ()->ts_join_channels;
        }
        _act_toggle_joined.setText ( *str );
      }
      ++act_vis;
    }
  }

  if ( act_vis == 0 ) {
    _cmenu.close ();
  }

  return act_vis;
}

bool
Mixer_Switches::eventFilter ( QObject * watched_n, QEvent * event_n )
{
  bool filtered ( false );

  if ( watched_n == _switches_pad ) {
    if ( event_n->type () == QEvent::KeyPress ) {
      if ( inputs_setup () != 0 ) {
        filtered = true;
        const QKeySequence kseq (
            static_cast< QKeyEvent * > ( event_n )->key () );
        if ( kseq == inputs_setup ()->ks_toggle_joined ) {
          _act_toggle_joined.trigger ();
        } else {
          filtered = false;
        }
      }
    } else if ( event_n->type () == QEvent::ContextMenu ) {
      //::std::cout << "Mixer_Switches::eventFilter: QContextMenuEvent\n";
      QContextMenuEvent * ev_cmenu (
          static_cast< QContextMenuEvent * > ( event_n ) );

      if ( context_menu_start ( ev_cmenu->globalPos () ) ) {
        filtered = true;
      }
    }
  }

  return filtered;
}

bool
Mixer_Switches::event ( QEvent * event_n )
{
  bool res ( true );

  if ( event_n->type () == ::MWdg::evt_separation_request ) {

    if ( !_separation_requested ) {
      _separation_requested = true;
      QCoreApplication::postEvent ( this,
                                    new QEvent ( ::MWdg::evt_separation ) );
    }

  } else if ( event_n->type () == ::MWdg::evt_separation ) {

    _separation_requested = false;
    separate_where_requested ();

  } else if ( event_n->type () == ::MWdg::evt_values_changed ) {

    context_menu_update ();

  } else {

    res = QWidget::event ( event_n );
  }

  return res;
}

} // namespace MWdg
