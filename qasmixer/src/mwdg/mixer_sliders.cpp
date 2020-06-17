/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_sliders.hpp"
#include "mwdg/event_types.hpp"
#include "mwdg/inputs_setup.hpp"
#include "mwdg/mixer_gui_state.hpp"
#include "mwdg/mixer_simple_setup.hpp"
#include "mwdg/mixer_sliders_proxies_group.hpp"
#include "mwdg/mixer_style.hpp"
#include "qsnd/mixer_simple.hpp"
#include "wdg/pad_proxy_style.hpp"
#include "wdg/pass_events.hpp"
#include "wdg/scroll_area_horizontal.hpp"
#include "wdg/sliders_pad.hpp"
#include <QCoreApplication>
#include <QFocusEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <iostream>

namespace MWdg
{

Mixer_Sliders::Mixer_Sliders ( QWidget * parent_n )
: QWidget ( parent_n )
, _mixer_setup ( 0 )
, _separation_requested ( false )
, _act_proxies_column ( 0 )
, _cmenu ( this )
, _act_toggle_joined ( this )
, _act_level_channels ( this )
, _act_separator_channels ( this )
, _act_toggle_mute ( this )
{
  // Sliders pad
  _sliders_pad = new ::Wdg::Sliders_Pad;

  connect ( _sliders_pad,
            SIGNAL ( sig_focus_changed () ),
            this,
            SLOT ( update_focus_proxies () ) );

  connect (
      _sliders_pad,
      SIGNAL ( sig_footer_label_selected ( unsigned int, unsigned int ) ),
      this,
      SIGNAL ( sig_footer_label_selected ( unsigned int, unsigned int ) ) );

  _sliders_pad->installEventFilter ( this );

  // Sliders area
  _sliders_area = new ::Wdg::Scroll_Area_Horizontal;
  _sliders_area->setFrameStyle ( QFrame::NoFrame );
  _sliders_area->set_widget ( _sliders_pad );

  // Actions
  _act_toggle_joined.setIcon ( QIcon::fromTheme ( "object-flip-horizontal" ) );

  _act_level_channels.setIcon ( QIcon::fromTheme ( "object-flip-vertical" ) );

  _act_separator_channels.setSeparator ( true );

  _act_str_mute[ 0 ] = tr ( "&Mute" );
  _act_str_mute[ 1 ] = tr ( "&Mute all" );
  _act_str_unmute[ 0 ] = tr ( "Un&mute" );
  _act_str_unmute[ 1 ] = tr ( "Un&mute all" );
  _act_str_toggle_mute = tr ( "Toggle &mutes" );

  _icon_vol_high = QIcon::fromTheme ( "audio-volume-high" );
  _icon_vol_med = QIcon::fromTheme ( "audio-volume-medium" );
  _icon_muted = QIcon::fromTheme ( "audio-volume-muted" );

  connect ( &_act_toggle_joined,
            SIGNAL ( triggered ( bool ) ),
            this,
            SLOT ( action_toggle_joined () ) );

  connect ( &_act_level_channels,
            SIGNAL ( triggered ( bool ) ),
            this,
            SLOT ( action_level_volumes () ) );

  connect ( &_act_toggle_mute,
            SIGNAL ( triggered ( bool ) ),
            this,
            SLOT ( action_toggle_mute () ) );

  // Context menu
  _cmenu.addAction ( &_act_toggle_joined );
  _cmenu.addAction ( &_act_level_channels );
  _cmenu.addAction ( &_act_separator_channels );
  _cmenu.addAction ( &_act_toggle_mute );

  connect ( &_cmenu,
            SIGNAL ( aboutToHide () ),
            this,
            SLOT ( context_menu_cleanup_behind () ) );

  // Proxies vars
  _ttip_slider[ 0 ] = tr ( "Playback slider" );
  _ttip_slider[ 1 ] = tr ( "Capture slider" );
  _ttip_switch[ 0 ] = tr ( "Playback switch" );
  _ttip_switch[ 1 ] = tr ( "Capture switch" );

  // Layout
  QVBoxLayout * lay_vbox ( new QVBoxLayout () );
  lay_vbox->addWidget ( _sliders_area, 1 );
  setLayout ( lay_vbox );

  // Adjust margins and spacings
  {
    int space ( qMax ( fontMetrics ().height (), lay_vbox->spacing () ) );
    lay_vbox->setSpacing ( space );
  }
}

Mixer_Sliders::~Mixer_Sliders ()
{
  set_mixer_setup ( 0 );
  delete _sliders_pad;
}

void
Mixer_Sliders::set_mixer_setup ( const ::MWdg::Mixer_Simple_Setup * setup_n )
{
  _cmenu.close ();

  if ( mixer_setup () != 0 ) {
    if ( mixer_setup ()->mixer_simple != 0 ) {
      disconnect ( mixer_setup ()->mixer_simple, 0, this, 0 );
      show_visible_proxies_sets ( false );
      clear_proxies_groups ();

      _sliders_pad->set_wdg_style_db ( 0 );
      _sliders_pad->set_image_alloc ( 0 );
      _sliders_pad->set_footer_visible ( 0 );
    }
  }

  _mixer_setup = setup_n;

  if ( mixer_setup () != 0 ) {
    if ( mixer_setup ()->mixer_simple != 0 ) {

      _sliders_pad->set_wdg_style_db ( mixer_setup ()->wdg_style_db );
      _sliders_pad->set_image_alloc ( mixer_setup ()->image_alloc );
      _sliders_pad->set_footer_visible (
          mixer_setup ()->show_slider_value_labels );

      create_proxies_groups ();
      rebuild_visible_proxies_list ();
      show_visible_proxies_sets ( true );
    }
  }
}

void
Mixer_Sliders::set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n )
{
  _inputs_setup = setup_n;

  if ( inputs_setup () != 0 ) {

    _act_toggle_joined.setShortcut ( inputs_setup ()->ks_toggle_joined );
    _act_level_channels.setShortcut ( inputs_setup ()->ks_level_channels );
    _act_toggle_mute.setShortcut ( inputs_setup ()->ks_mute_volumes );

    _act_level_channels.setText ( inputs_setup ()->ts_level_channels );

    _sliders_pad->set_wheel_degrees ( inputs_setup ()->wheel_degrees );
  }
}

void
Mixer_Sliders::clear_proxies_groups ()
{
  for ( int ii = 0; ii < _proxies_groups.size (); ++ii ) {
    delete _proxies_groups[ ii ];
  }
  _proxies_groups.clear ();
  _proxies_groups_vis.clear ();
  _proxies_groups_pass.clear ();
}

void
Mixer_Sliders::create_proxies_groups ()
{
  ::QSnd::Mixer_Simple * snd_mixer ( 0 );
  if ( mixer_setup () != 0 ) {
    snd_mixer = mixer_setup ()->mixer_simple;
  }
  if ( snd_mixer == 0 ) {
    return;
  }

  for ( unsigned int ii = 0; ii < snd_mixer->num_elems (); ++ii ) {
    ::QSnd::Mixer_Simple_Elem * qsme ( snd_mixer->elem ( ii ) );

    for ( unsigned int snd_dir = 0; snd_dir < 2; ++snd_dir ) {
      if ( qsme->has_volume ( snd_dir ) ) {

        Mixer_Sliders_Proxies_Group * mspg (
            new Mixer_Sliders_Proxies_Group ( this ) );

        mspg->set_snd_dir ( snd_dir );
        mspg->set_mixer_simple_elem ( qsme );
        mspg->set_group_name ( qsme->display_name () );
        mspg->set_style_id ( ::MWdg::Mixer_Style::PLAYBACK + snd_dir );

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

void
Mixer_Sliders::setup_proxies_group_joined (
    Mixer_Sliders_Proxies_Group * mspg_n )
{
  mspg_n->clear_columns ();
  mspg_n->set_is_joined ( true );

  {
    QString ttip;
    ttip = mspg_n->group_name ();
    mspg_n->set_tool_tip ( ttip );
  }

  create_proxies_group ( mspg_n, 0 );
  mspg_n->update_mixer_values ();
}

void
Mixer_Sliders::setup_proxies_group_separate (
    Mixer_Sliders_Proxies_Group * mspg_n )
{
  const unsigned int num_channels (
      mspg_n->mixer_simple_elem ()->num_channels ( mspg_n->snd_dir () ) );

  mspg_n->clear_columns ();
  mspg_n->set_is_joined ( false );

  {
    QString ttip;
    ttip = mspg_n->group_name ();
    mspg_n->set_tool_tip ( ttip );
  }

  for ( unsigned int ii = 0; ii < num_channels; ++ii ) {
    create_proxies_group ( mspg_n, ii );
  }
  mspg_n->update_mixer_values ();
}

bool
Mixer_Sliders::create_proxies_group ( Mixer_Sliders_Proxies_Group * mspg_n,
                                      unsigned int channel_idx_n )
{
  bool res ( false );

  ::QSnd::Mixer_Simple_Elem * qsme ( mspg_n->mixer_simple_elem () );
  const unsigned int snd_dir ( mspg_n->snd_dir () );

  Mixer_Sliders_Proxies_Column * mspc ( new Mixer_Sliders_Proxies_Column );

  // Item name
  QString iname;
  if ( mspg_n->is_joined () ) {
    iname = mspg_n->group_name ();
  } else {
    iname = tr ( "%1 (%2)" );
    iname = iname.arg ( QCoreApplication::translate (
        "ALSA::Channel_Name", qsme->channel_name ( snd_dir, channel_idx_n ) ) );
    iname = iname.arg ( qsme->channel ( snd_dir, channel_idx_n ) );
  }

  // Tool tip
  QString ttip_name;
  QString ttip_channel;
  ttip_name += "<div><b>";
  ttip_name += mspg_n->group_name ();
  ttip_name += "</b></div>\n";

  if ( !mspg_n->is_joined () ) {
    ttip_channel += "<div>";
    ttip_channel += iname;
    ttip_channel += "</div>\n";
  }

  // Create volume proxy
  if ( qsme->has_volume ( snd_dir ) &&
       ( channel_idx_n < qsme->num_volume_channels ( snd_dir ) ) ) {
    Mixer_Sliders_Proxy_Slider * msps ( new Mixer_Sliders_Proxy_Slider );

    msps->set_snd_dir ( snd_dir );
    msps->set_channel_idx ( channel_idx_n );
    msps->set_is_joined ( mspg_n->is_joined () );
    msps->set_mixer_simple_elem ( qsme );

    msps->set_item_name ( iname );
    msps->set_group_name ( mspg_n->group_name () );
    msps->set_style_id ( ::MWdg::Mixer_Style::PLAYBACK + snd_dir );

    {
      QString ttip ( ttip_name );
      {
        QString ttip_type;
        ttip_type += "<div>";
        ttip_type += _ttip_slider[ snd_dir ];
        ttip_type += "</div>";
        ttip += ttip_type;
      }
      ttip += ttip_channel;
      msps->set_tool_tip ( ttip );
    }

    // Proxy Style
    if ( msps->has_dB () ) {

      ::Wdg::Pad_Proxy_Style * pstyle ( new ::Wdg::Pad_Proxy_Style );

      if ( ( msps->dB_max () > 0 ) && ( msps->dB_min () < 0 ) ) {
        pstyle->slider_has_minimum = true;
        pstyle->slider_minimum_idx = msps->ask_dB_vol_nearest ( 0 );
      } else {
        if ( msps->dB_max () <= 0 ) {
          pstyle->slider_has_minimum = true;
          pstyle->slider_minimum_idx = msps->slider_index_max ();
        } else if ( msps->dB_min () >= 0 ) {
          pstyle->slider_has_minimum = true;
          pstyle->slider_minimum_idx = 0;
        }
      }

      msps->set_style ( pstyle );
    }

    mspc->set_slider_proxy ( msps );
  }

  // Create switch proxy
  if ( qsme->has_switch ( snd_dir ) &&
       ( channel_idx_n < qsme->num_switch_channels ( snd_dir ) ) ) {
    Mixer_Sliders_Proxy_Switch * msps ( new Mixer_Sliders_Proxy_Switch );

    msps->set_snd_dir ( snd_dir );
    msps->set_channel_idx ( channel_idx_n );
    msps->set_is_joined ( mspg_n->is_joined () );
    msps->set_mixer_simple_elem ( qsme );

    msps->set_item_name ( iname );
    msps->set_group_name ( mspg_n->group_name () );
    msps->set_style_id ( ::MWdg::Mixer_Style::PLAYBACK + snd_dir );

    {
      QString ttip ( ttip_name );
      {
        QString ttip_type;
        ttip_type += "<div>";
        ttip_type += _ttip_switch[ snd_dir ];
        ttip_type += "</div>";
        ttip += ttip_type;
      }

      ttip += ttip_channel;
      msps->set_tool_tip ( ttip );
    }

    mspc->set_switch_proxy ( msps );
  }

  if ( mspc->has_slider () || mspc->has_switch () ) {
    mspg_n->append_column ( mspc );
  } else {
    delete mspc;
    res = true;
  }

  return res;
}

bool
Mixer_Sliders::should_be_visible (
    const Mixer_Sliders_Proxies_Group * mspg_n ) const
{
  const ::QSnd::Mixer_Simple_Elem * qsme ( mspg_n->mixer_simple_elem () );
  unsigned int snd_dir ( mspg_n->snd_dir () % 2 );
  return ( qsme->is_active () && mixer_setup ()->show_stream[ snd_dir ] );
}

void
Mixer_Sliders::rebuild_visible_proxies_list ()
{
  _proxies_groups_vis.clear ();
  _proxies_groups_pass.clear ();

  for ( int ii = 0; ii < _proxies_groups.size (); ++ii ) {
    Mixer_Sliders_Proxies_Group * mspg ( _proxies_groups[ ii ] );
    mspg->set_is_visible ( should_be_visible ( mspg ) );
    if ( mspg->is_visible () ) {
      if ( mspg->needs_separation () ) {
        separate_proxies_group ( mspg );
      }
      _proxies_groups_vis.append ( mspg );
      _proxies_groups_pass.append ( mspg );
    }
  }
}

void
Mixer_Sliders::action_level_volumes ()
{
  //::std::cout << "Mixer_Sliders::action_level_volumes" << "\n";

  ::MWdg::Mixer_Sliders_Proxies_Group * mspg ( _act_proxies_group );

  if ( mspg != 0 ) {

    ::MWdg::Mixer_Sliders_Proxies_Column * mspc ( 0 );

    {
      unsigned int col_idx ( 0 );
      if ( _act_proxies_column < mspg->num_columns () ) {
        col_idx = _act_proxies_column;
      }

      mspc = static_cast<::MWdg::Mixer_Sliders_Proxies_Column * > (
          mspg->column ( col_idx ) );
    }

    ::MWdg::Mixer_Sliders_Proxy_Slider * msps ( 0 );
    if ( mspc != 0 ) {
      msps = static_cast<::MWdg::Mixer_Sliders_Proxy_Slider * > (
          mspc->slider_proxy () );
    }

    if ( msps != 0 ) {
      mspg->mixer_simple_elem ()->set_volume_all ( mspg->snd_dir (),
                                                   msps->volume_value () );
    } else {
      mspg->mixer_simple_elem ()->level_volumes ( mspg->snd_dir () );
    }
  }
}

void
Mixer_Sliders::action_toggle_joined ()
{
  //::std::cout << "Mixer_Sliders::action_toggle_joined" << "\n";

  Mixer_Sliders_Proxies_Group * mspg ( _act_proxies_group );
  if ( mspg != 0 ) {
    toggle_joined_separated ( mspg );
  }
}

void
Mixer_Sliders::action_toggle_mute ()
{
  //::std::cout << "Mixer_Sliders::action_toggle_mute" << "\n";

  Mixer_Sliders_Proxies_Group * mspg ( _act_proxies_group );
  if ( mspg != 0 ) {
    mspg->mixer_simple_elem ()->invert_switches ( mspg->snd_dir () );
  }
}

void
Mixer_Sliders::update_focus_proxies ()
{
  if ( _sliders_pad->focus_info ().has_focus ) {
    // Find focus proxies_group
    const int idx ( _sliders_pad->focus_info ().group_idx );
    if ( idx < _proxies_groups_vis.size () ) {
      _act_proxies_group = _proxies_groups_vis[ idx ];
      _act_proxies_column = _sliders_pad->focus_info ().column_idx;
    }
  }
}

void
Mixer_Sliders::acquire_gui_state ( ::MWdg::Mixer_GUI_State_Proxy & state_n )
{
  if ( _act_proxies_group != 0 ) {
    ::MWdg::Mixer_Sliders_Proxies_Group * mspg ( _act_proxies_group );
    state_n.group_name = mspg->group_name ();
    state_n.snd_dir = mspg->snd_dir ();
    state_n.column_idx = mspg->focus_column ();
    state_n.row_idx = mspg->focus_row ();
    state_n.has_focus = _sliders_pad->focus_info ().has_focus;
  }
}

void
Mixer_Sliders::restore_gui_state (
    const ::MWdg::Mixer_GUI_State_Proxy & state_n )
{
  {
    _act_proxies_group = find_visible_proxy ( state_n );
    _act_proxies_column = state_n.column_idx;
  }

  // Restore focus
  if ( state_n.has_focus && ( _act_proxies_group != 0 ) ) {
    _sliders_pad->set_focus_proxy ( _act_proxies_group->group_index (),
                                    state_n.column_idx,
                                    state_n.row_idx );
  }

  if ( _cmenu.isVisible () ) {
    context_menu_update ();
  }
}

::MWdg::Mixer_Sliders_Proxies_Group *
Mixer_Sliders::find_visible_proxy (
    const ::MWdg::Mixer_GUI_State_Proxy & prox_id_n )
{
  ::MWdg::Mixer_Sliders_Proxies_Group * mspg_res ( 0 );

  if ( !prox_id_n.is_clear () ) {
    for ( int ii = 0; ii < _proxies_groups_vis.size (); ++ii ) {
      ::MWdg::Mixer_Sliders_Proxies_Group * mspg ( _proxies_groups_vis[ ii ] );
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
Mixer_Sliders::toggle_joined_separated (
    ::MWdg::Mixer_Sliders_Proxies_Group * mspg_n )
{
  if ( mspg_n != 0 ) {

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
}

void
Mixer_Sliders::join_proxies_group (
    ::MWdg::Mixer_Sliders_Proxies_Group * mspg_n )
{
  if ( !mspg_n->is_joined () ) {
    ::QSnd::Mixer_Simple_Elem * qsme ( mspg_n->mixer_simple_elem () );
    qsme->level_volumes ( mspg_n->snd_dir () );
    qsme->level_switches ( mspg_n->snd_dir () );

    setup_proxies_group_joined ( mspg_n );
    if ( _cmenu.isVisible () ) {
      context_menu_update ();
    }
  }
}

void
Mixer_Sliders::separate_proxies_group ( Mixer_Sliders_Proxies_Group * mspg_n )
{
  if ( mspg_n->can_be_separated () ) {
    setup_proxies_group_separate ( mspg_n );
    if ( _cmenu.isVisible () ) {
      context_menu_update ();
    }
  }
}

void
Mixer_Sliders::show_visible_proxies_sets ( bool flag_n )
{
  if ( flag_n ) {
    if ( _proxies_groups_vis.size () > 0 ) {
      _sliders_pad->set_proxies_groups ( _proxies_groups_pass );
      _sliders_area->set_widget ( _sliders_pad );

      _sliders_pad->setAutoFillBackground ( false );
      _sliders_pad->show ();
    }
    updateGeometry ();
  } else {
    _sliders_pad->hide ();
    _sliders_area->take_widget ();
    _sliders_pad->clear_proxies_groups ();
  }
}

void
Mixer_Sliders::separate_where_requested ()
{
  //::std::cout << "Mixer_Sliders::separate_where_requested" << "\n";

  bool visible_request ( false );
  for ( int ii = 0; ii < _proxies_groups_vis.size (); ++ii ) {
    ::MWdg::Mixer_Sliders_Proxies_Group * mspg ( _proxies_groups_vis[ ii ] );
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
      Mixer_Sliders_Proxies_Group * mspg ( _proxies_groups_vis[ ii ] );
      if ( mspg->separation_request () ) {
        mspg->set_separation_request ( false );
        separate_proxies_group ( mspg );
      }
    }

    show_visible_proxies_sets ( true );
    restore_gui_state ( proxy_state );
  }

  //::std::cout << "Mixer_Sliders::separate_where_requested done" << "\n";
}

bool
Mixer_Sliders::context_menu_start ( const QPoint & pos_n )
{
  bool res ( false );

  if ( !_cmenu.isVisible () && ( _sliders_pad->focus_info ().has_focus ) &&
       ( _act_proxies_group != 0 ) ) {
    if ( context_menu_update () > 0 ) {
      // Tells the proxy that any changes should be notified
      _act_proxies_group->set_notify_value_change ( true );

      _cmenu.setTitle ( _act_proxies_group->group_name () );
      _cmenu.popup ( pos_n );
      res = true;
    }
  }

  return res;
}

void
Mixer_Sliders::context_menu_cleanup_behind ()
{
  //::std::cout << "Mixer_Sliders::context_cleanup_behind\n";
  if ( _act_proxies_group != 0 ) {
    // Tells the proxy that changes must not be notified
    _act_proxies_group->set_notify_value_change ( false );
  }
}

unsigned int
Mixer_Sliders::context_menu_update ()
{
  //::std::cout << "Mixer_Sliders::context_menu_update\n";

  unsigned int act_vis ( 0 );

  Mixer_Sliders_Proxies_Group * mspg ( _act_proxies_group );

  if ( mspg == 0 ) {
    _cmenu.close ();
    return act_vis;
  }

  const ::QSnd::Mixer_Simple_Elem * qsme ( mspg->mixer_simple_elem () );
  const unsigned int snd_dir ( mspg->snd_dir () );

  // Update split/join and level channels actions
  {
    const bool vis_joined ( mspg->can_be_separated () );

    _act_toggle_joined.setVisible ( vis_joined );
    if ( vis_joined ) {
      ++act_vis;

      if ( inputs_setup () != 0 ) {
        const QString * str;
        if ( mspg->is_joined () ) {
          str = &inputs_setup ()->ts_split_channels;
        } else {
          str = &inputs_setup ()->ts_join_channels;
        }
        _act_toggle_joined.setText ( *str );
      }
    }

    const bool vis_level ( ( mspg->num_sliders () > 1 ) &&
                           !qsme->volumes_equal ( snd_dir ) );

    _act_level_channels.setVisible ( vis_level );
    if ( vis_level ) {
      ++act_vis;
    }
  }

  // Update mute / unmute channels actions
  {
    unsigned int num_sw ( mspg->num_switches () );

    if ( num_sw > 0 ) {
      ++act_vis;

      QString * act_txt = &_act_str_toggle_mute;
      QIcon * act_icon = &_icon_vol_med;
      const bool is_on ( qsme->switch_state ( snd_dir, 0 ) );

      if ( is_on ) {
        act_icon = &_icon_muted;
      } else {
        act_icon = &_icon_vol_high;
      }

      if ( num_sw == 1 ) {
        if ( is_on ) {
          act_txt = &_act_str_mute[ 0 ];
        } else {
          act_txt = &_act_str_unmute[ 0 ];
        }
      } else {
        if ( qsme->switches_equal ( snd_dir ) ) {
          if ( is_on ) {
            act_txt = &_act_str_mute[ 1 ];
          } else {
            act_txt = &_act_str_unmute[ 1 ];
          }
        }
      }

      _act_toggle_mute.setText ( *act_txt );
      _act_toggle_mute.setIcon ( *act_icon );
    }

    _act_toggle_mute.setVisible ( num_sw > 0 );
  }

  if ( act_vis == 0 ) {
    _cmenu.close ();
  }

  return act_vis;
}

bool
Mixer_Sliders::eventFilter ( QObject * watched_n, QEvent * event_n )
{
  bool filtered ( false );

  if ( watched_n == _sliders_pad ) {

    if ( event_n->type () == QEvent::KeyPress ) {

      if ( inputs_setup () != 0 ) {
        filtered = true;
        const QKeySequence kseq (
            static_cast< QKeyEvent * > ( event_n )->key () );

        // Trigger actions on a key press
        if ( kseq == inputs_setup ()->ks_toggle_joined ) {
          _act_toggle_joined.trigger ();
        } else if ( kseq == inputs_setup ()->ks_level_channels ) {
          _act_level_channels.trigger ();
        } else if ( kseq == inputs_setup ()->ks_mute_volumes ) {
          _act_toggle_mute.trigger ();
        } else {
          filtered = false;
        }
      }

    } else if ( event_n->type () == QEvent::ContextMenu ) {
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
Mixer_Sliders::event ( QEvent * event_n )
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
