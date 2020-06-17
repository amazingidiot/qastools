/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_edit_int.hpp"
#include "mwdg/event_types.hpp"
#include "mwdg/inputs_setup.hpp"
#include "mwdg/mixer_hctl_editor_data.hpp"
#include "mwdg/mixer_hctl_int_proxies_group.hpp"
#include "mwdg/mixer_hctl_int_proxy_column.hpp"
#include "mwdg/mixer_hctl_int_proxy_slider.hpp"
#include "mwdg/mixer_hctl_slider_status_widget.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include "qsnd/mixer_hctl_elem_group.hpp"
#include "wdg/ds_widget_types.hpp"
#include "wdg/label_width.hpp"
#include "wdg/pad_proxy_style.hpp"
#include "wdg/pad_proxy_switch.hpp"
#include <QContextMenuEvent>
#include <QEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QVBoxLayout>
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Edit_Int::Mixer_HCTL_Edit_Int (
    ::MWdg::Mixer_HCTL_Editor_Data * data_n, QWidget * parent_n )
: Mixer_HCTL_Editor ( data_n, parent_n )
, _act_toggle_joined ( this )
, _act_level_channels ( this )
{
  _range_mask = "%1 / %2";
  _range_ttip = _range_mask.arg ( tr ( "minimum" ), tr ( "maximum" ) );
  _str_int_range = tr ( "Integer range:" );
  _str_dB_range = tr ( "Decibel range:" );

  // Actions
  _act_toggle_joined.setIcon ( QIcon::fromTheme ( "object-flip-horizontal" ) );

  _act_level_channels.setIcon ( QIcon::fromTheme ( "object-flip-vertical" ) );

  connect ( &_act_toggle_joined,
            SIGNAL ( triggered ( bool ) ),
            this,
            SLOT ( action_toggle_joined () ) );

  connect ( &_act_level_channels,
            SIGNAL ( triggered ( bool ) ),
            this,
            SLOT ( action_level_volumes () ) );

  // Context menu
  _cmenu.addAction ( &_act_toggle_joined );
  _cmenu.addAction ( &_act_level_channels );

  connect ( &_cmenu,
            SIGNAL ( aboutToHide () ),
            this,
            SLOT ( context_menu_cleanup_behind () ) );

  rebuild ();
}

Mixer_HCTL_Edit_Int::~Mixer_HCTL_Edit_Int ()
{
  clear ();
}

void
Mixer_HCTL_Edit_Int::set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n )
{
  ::MWdg::Mixer_HCTL_Editor::set_inputs_setup ( setup_n );

  if ( inputs_setup () != 0 ) {

    _act_toggle_joined.setShortcut ( inputs_setup ()->ks_toggle_joined );
    _act_level_channels.setShortcut ( inputs_setup ()->ks_level_channels );
    _act_level_channels.setText ( inputs_setup ()->ts_level_channels );

    if ( _sliders_pad != 0 ) {
      _sliders_pad->set_wheel_degrees ( inputs_setup ()->wheel_degrees );
    }
  }
}

void
Mixer_HCTL_Edit_Int::clear ()
{
  _sliders_pad.reset ();
  _scroll_area.reset ();

  for ( int pii = 0; pii < _proxies_groups.size (); ++pii ) {
    delete _proxies_groups[ pii ];
  }
  _proxies_groups.clear ();
  _proxies_slider.clear ();
}

void
Mixer_HCTL_Edit_Int::rebuild ()
{
  clear ();

  if ( editor_data ()->snd_elem_group != 0 ) {
    if ( editor_data ()->snd_elem_group->num_elems () > 0 ) {
      if ( editor_data ()->elem_idx <
           editor_data ()->snd_elem_group->num_elems () ) {
        setup_single ();
      } else {
        setup_multi ();
      }
      update_proxies_values ();
    }
  }
}

Wdg::Pad_Proxies_Group *
Mixer_HCTL_Edit_Int::create_proxies_group ( ::QSnd::Mixer_HCTL_Elem * elem_n,
                                            bool multi_n )
{
  Mixer_HCTL_Int_Proxies_Group * sppg (
      new Mixer_HCTL_Int_Proxies_Group ( elem_n, this ) );

  const QString str_ch ( tr ( "Channel %1" ) );
  const QString str_idx ( tr ( "Index %1" ) );
  const unsigned int style_id ( elem_style_id ( elem_n ) );

  sppg->set_group_name ( str_idx.arg ( elem_n->elem_index () ) );
  sppg->set_style_id ( style_id );

  const bool int_equal ( elem_n->integers_equal () );
  const unsigned int num_channels ( elem_n->count () );
  for ( unsigned int ii = 0; ii < num_channels; ++ii ) {
    Mixer_HCTL_Int_Proxy_Column * pcol ( new Mixer_HCTL_Int_Proxy_Column );

    // Slider proxy
    {
      Mixer_HCTL_Int_Proxy_Slider * pslider (
          new Mixer_HCTL_Int_Proxy_Slider ( elem_n, ii ) );

      if ( editor_data ()->wdg_style_db != 0 ) {
        if ( multi_n ) {
          QString gname ( "%1 (%2)" );
          gname = gname.arg ( elem_n->display_name () );
          gname = gname.arg ( elem_n->elem_index () );
          pslider->set_group_name ( gname );
        } else {
          pslider->set_group_name ( elem_n->display_name () );
        }
        pslider->set_item_name ( str_ch.arg ( ii ) );
        pslider->set_style_id ( style_id );

        // Decible minimum
        long db_min;
        long db_max;
        if ( elem_n->dB_range ( &db_min, &db_max ) == 0 ) {

          ::Wdg::Pad_Proxy_Style * pstyle ( new ::Wdg::Pad_Proxy_Style );
          pslider->set_style ( pstyle );

          if ( ( db_min < 0 ) && ( db_max > 0 ) ) {
            pstyle->slider_has_minimum = true;
            pstyle->slider_minimum_idx =
                pslider->integer_to_index ( elem_n->ask_int_from_dB ( 0 ) );
          } else {
            if ( db_max <= 0 ) {
              pstyle->slider_has_minimum = true;
              pstyle->slider_minimum_idx = pslider->slider_index_max ();
            } else if ( db_min >= 0 ) {
              pstyle->slider_has_minimum = true;
              pstyle->slider_minimum_idx = 0;
            }
          }
        }
      }

      pslider->set_tool_tip ( str_ch.arg ( ii ) );
      pcol->set_slider_proxy ( pslider );
      _proxies_slider.append ( pslider );
    }

    // Switch proxy (joined switch)
    if ( ( ii == 0 ) && ( num_channels > 1 ) ) {
      ::Wdg::Pad_Proxy_Switch * pswitch ( new ::Wdg::Pad_Proxy_Switch );

      pswitch->set_tool_tip ( editor_data ()->str_joined );
      pswitch->set_switch_state ( int_equal );
      pswitch->set_variant_id ( ::Wdg::DS_SVG_JOINED );
      pswitch->set_style_id ( elem_style_id ( elem_n ) );
      pcol->set_switch_proxy ( pswitch );

      connect ( pswitch,
                SIGNAL ( sig_switch_state_changed ( bool ) ),
                sppg,
                SLOT ( set_joined ( bool ) ) );
    }

    sppg->append_column ( pcol );
    sppg->set_joined ( int_equal );
  }

  return sppg;
}

void
Mixer_HCTL_Edit_Int::setup_single ()
{
  ::QSnd::Mixer_HCTL_Elem * elem (
      editor_data ()->snd_elem_group->elem ( editor_data ()->elem_idx ) );

  _proxies_groups.append ( create_proxies_group ( elem, false ) );

  setup_widgets ();
}

void
Mixer_HCTL_Edit_Int::setup_multi ()
{
  // Create proxies
  const unsigned int num_elems ( editor_data ()->snd_elem_group->num_elems () );
  for ( unsigned int eii = 0; eii < num_elems; ++eii ) {
    ::QSnd::Mixer_HCTL_Elem * elem (
        editor_data ()->snd_elem_group->elem ( eii ) );
    _proxies_groups.append ( create_proxies_group ( elem, true ) );
  }

  setup_widgets ();
}

void
Mixer_HCTL_Edit_Int::setup_widgets ()
{
  _sliders_pad.reset (
      new ::Wdg::Sliders_Pad ( this, editor_data ()->image_alloc ) );
  _sliders_pad->set_wdg_style_db ( editor_data ()->wdg_style_db );
  if ( inputs_setup () != 0 ) {
    _sliders_pad->set_wheel_degrees ( inputs_setup ()->wheel_degrees );
  }
  _sliders_pad->set_proxies_groups ( _proxies_groups );
  _sliders_pad->installEventFilter ( this );

  connect ( _sliders_pad.data (),
            SIGNAL ( sig_focus_changed () ),
            this,
            SLOT ( update_focus_proxies () ) );

  connect ( _sliders_pad.data (),
            SIGNAL ( sig_footer_label_selected ( unsigned int, unsigned int ) ),
            this,
            SLOT ( footer_label_selected ( unsigned int, unsigned int ) ) );

  // Scroll area
  _scroll_area.reset ( new ::Wdg::Scroll_Area_Horizontal );
  _scroll_area->setFrameStyle ( QFrame::NoFrame );
  _scroll_area->setWidget ( _sliders_pad.data () );

  // Editor pad layout
  {
    QVBoxLayout * lay_pad ( new QVBoxLayout );
    lay_pad->setContentsMargins ( 0, 0, 0, 0 );
    lay_pad->addWidget ( _scroll_area.data (), 1 );
    lay_pad->addLayout ( create_range_label () );
    setLayout ( lay_pad );
  }
}

QString
Mixer_HCTL_Edit_Int::integer_string ( long value_n ) const
{
  return editor_data ()->loc.toString ( (int)value_n );
}

QString
Mixer_HCTL_Edit_Int::dB_string ( double value_n ) const
{
  return editor_data ()->loc.toString ( value_n, 'f', 2 );
}

QLayout *
Mixer_HCTL_Edit_Int::create_range_label ()
{
  ::QSnd::Mixer_HCTL_Elem * elem ( editor_data ()->snd_elem_group->elem ( 0 ) );

  QLabel * lbl_int_range ( new QLabel );
  ::Wdg::Label_Width * lbl_int_rval ( new ::Wdg::Label_Width );

  QLabel * lbl_dB_range ( new QLabel );
  ::Wdg::Label_Width * lbl_dB_rval ( new ::Wdg::Label_Width );

  lbl_int_range->setText ( _str_int_range );
  lbl_dB_range->setText ( _str_dB_range );

  const Qt::Alignment align_cc ( Qt::AlignHCenter | Qt::AlignVCenter );
  // const Qt::Alignment align_lc ( Qt::AlignLeft | Qt::AlignVCenter );

  {
    {
      const QString mstr ( integer_string ( -99999 ) );
      lbl_int_rval->set_min_text ( _range_mask.arg ( mstr, mstr ) );
    }
    lbl_int_rval->setAlignment ( align_cc );
    lbl_int_rval->setToolTip ( _range_ttip );

    {
      const QString mstr ( dB_string ( -99.99 ) );
      lbl_dB_rval->set_min_text ( _range_mask.arg ( mstr, mstr ) );
    }
    lbl_dB_rval->setAlignment ( align_cc );
    lbl_dB_rval->setToolTip ( _range_ttip );

    {
      const QString min_str ( integer_string ( elem->integer_min () ) );
      const QString max_str ( integer_string ( elem->integer_max () ) );
      lbl_int_rval->setText ( _range_mask.arg ( min_str, max_str ) );
    }

    long db_range[ 2 ];
    if ( elem->dB_range ( &db_range[ 0 ], &db_range[ 1 ] ) == 0 ) {
      const QString min_str ( dB_string ( db_range[ 0 ] / 100.0 ) );
      const QString max_str ( dB_string ( db_range[ 1 ] / 100.0 ) );
      lbl_dB_rval->setText ( _range_mask.arg ( min_str, max_str ) );
    } else {
      lbl_dB_range->setEnabled ( false );
      lbl_dB_rval->setEnabled ( false );
    }
  }

  QGridLayout * lay_res ( new QGridLayout );
  lay_res->setContentsMargins ( 0, 0, 0, 0 );
  lay_res->addWidget ( lbl_int_range, 0, 0 );
  lay_res->addWidget ( lbl_int_rval, 0, 1, align_cc );
  lay_res->addWidget ( lbl_dB_range, 1, 0 );
  lay_res->addWidget ( lbl_dB_rval, 1, 1, align_cc );

  QHBoxLayout * lay_hbox ( new QHBoxLayout );
  lay_hbox->setContentsMargins ( 0, 0, 0, 0 );
  lay_hbox->addLayout ( lay_res );
  lay_hbox->addStretch ( 1 );

  return lay_hbox;
}

void
Mixer_HCTL_Edit_Int::update_proxies_values ()
{
  for ( int ii = 0; ii < _proxies_slider.size (); ++ii ) {
    _proxies_slider[ ii ]->update_value_from_source ();
  }
}

void
Mixer_HCTL_Edit_Int::update_focus_proxies ()
{
  _focus_proxies_group = 0;
  _focus_proxy_column = 0;
  if ( _sliders_pad != 0 ) {
    if ( _sliders_pad->focus_info ().has_focus ) {
      // Find focus proxies_group
      const int idx ( _sliders_pad->focus_info ().group_idx );
      if ( idx < _proxies_groups.size () ) {
        _focus_proxies_group = static_cast< Mixer_HCTL_Int_Proxies_Group * > (
            _proxies_groups[ idx ] );
        _focus_proxy_column = _sliders_pad->focus_info ().column_idx;
      }
    }
  }

  if ( _focus_proxies_group != 0 ) {
    _act_proxies_group = _focus_proxies_group;
    _act_proxy_column = _focus_proxy_column;
  }
}

bool
Mixer_HCTL_Edit_Int::context_menu_start ( const QPoint & pos_n )
{
  bool res ( false );

  if ( !_cmenu.isVisible () && ( _focus_proxies_group != 0 ) &&
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

unsigned int
Mixer_HCTL_Edit_Int::context_menu_update ()
{
  //::std::cout << "Mixer_HCTL_Edit_Int::context_menu_update\n";

  unsigned int act_vis ( 0 );

  Mixer_HCTL_Int_Proxies_Group * mspg ( _act_proxies_group );

  if ( mspg == 0 ) {
    _cmenu.close ();
    return act_vis;
  }

  // Update split/join and level channels actions
  {
    const bool vis_joined ( mspg->num_sliders () > 1 );

    _act_toggle_joined.setVisible ( vis_joined );
    _act_level_channels.setVisible ( vis_joined );
    if ( vis_joined ) {
      if ( inputs_setup () != 0 ) {
        const QString * str;
        if ( mspg->is_joined () ) {
          str = &inputs_setup ()->ts_split_channels;
        } else {
          str = &inputs_setup ()->ts_join_channels;
        }
        _act_toggle_joined.setText ( *str );
      }
      _act_level_channels.setEnabled ( !mspg->volumes_equal () );
      act_vis += 2;
    }
  }

  if ( act_vis == 0 ) {
    _cmenu.close ();
  }

  return act_vis;
}

void
Mixer_HCTL_Edit_Int::context_menu_cleanup_behind ()
{
  if ( _act_proxies_group != 0 ) {
    _act_proxies_group->set_notify_value_change ( false );
  }
}

void
Mixer_HCTL_Edit_Int::action_toggle_joined ()
{
  if ( _act_proxies_group != 0 ) {
    _act_proxies_group->set_joined ( !_act_proxies_group->is_joined () );
  }
}

void
Mixer_HCTL_Edit_Int::action_level_volumes ()
{
  if ( _act_proxies_group != 0 ) {
    _act_proxies_group->level_volumes ( _act_proxy_column );
  }
}

void
Mixer_HCTL_Edit_Int::footer_label_selected ( unsigned int group_idx_n,
                                             unsigned int column_idx_n )
{
  //::std::cout << "Footer label selected " << group_idx_n << " " <<
  //column_idx_n << "\n";
  bool keep_indices ( true );
  if ( _status_wdg == 0 ) {
    // Create new status widget
    if ( _sliders_pad != 0 ) {
      ::MWdg::Mixer_HCTL_Slider_Status_Widget * swdg (
          new ::MWdg::Mixer_HCTL_Slider_Status_Widget ( this ) );
      swdg->setAttribute ( Qt::WA_DeleteOnClose );
      swdg->set_sliders_pad ( _sliders_pad.data () );
      swdg->slider_focus_changed ();

      _status_wdg = swdg;
      _status_wdg->show ();
    }
  } else {
    if ( ( group_idx_n == _status_group_idx ) &&
         ( column_idx_n == _status_column_idx ) ) {
      _status_group_idx = ~0;
      _status_column_idx = ~0;
      _status_wdg->close ();
      keep_indices = false;
    }
  }

  if ( keep_indices ) {
    _status_group_idx = group_idx_n;
    _status_column_idx = column_idx_n;
  }
}

bool
Mixer_HCTL_Edit_Int::event ( QEvent * event_n )
{
  bool res ( true );

  if ( event_n->type () == ::MWdg::evt_values_changed ) {
    context_menu_update ();
  } else {
    res = Mixer_HCTL_Editor::event ( event_n );
  }

  return res;
}

bool
Mixer_HCTL_Edit_Int::eventFilter ( QObject * watched_n, QEvent * event_n )
{
  bool filtered ( false );

  if ( watched_n == _sliders_pad.data () ) {
    if ( event_n->type () == QEvent::KeyPress ) {
      QKeyEvent * ev_key ( static_cast< QKeyEvent * > ( event_n ) );

      filtered = true;
      // Trigger actions
      const QKeySequence key_seq ( ev_key->key () );
      if ( _act_toggle_joined.shortcut () == key_seq ) {
        _act_toggle_joined.trigger ();
      } else if ( _act_level_channels.shortcut () == key_seq ) {
        _act_level_channels.trigger ();
      } else {
        filtered = false;
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

} // namespace MWdg
