/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "pad_proxies_group.hpp"
#include "wdg/event_types.hpp"
#include "wdg/pad_proxies_column.hpp"
#include "wdg/pad_proxy_slider.hpp"
#include "wdg/pad_proxy_switch.hpp"
#include "wdg/pass_events.hpp"
#include <QCoreApplication>
#include <iostream>

namespace Wdg
{

Pad_Proxies_Group::Pad_Proxies_Group ( QObject * parent_n )
: QObject ( parent_n )
, _pad ( 0 )
, _group_index ( 0 )
, _style_id ( 0 )
, _num_sliders ( 0 )
, _num_switches ( 0 )
, _focus_column ( 0 )
, _focus_row ( 0 )
, _has_focus ( false )
{
}

Pad_Proxies_Group::~Pad_Proxies_Group ()
{
  clear_columns ();
}

void
Pad_Proxies_Group::set_pad ( QObject * pad_n )
{
  _pad = pad_n;
}

void
Pad_Proxies_Group::set_group_index ( unsigned int idx_n )
{
  _group_index = idx_n;
}

void
Pad_Proxies_Group::clear_columns ()
{
  if ( _columns.size () > 0 ) {
    for ( int ii = 0; ii < _columns.size (); ++ii ) {
      delete _columns[ ii ];
    }
    _columns.clear ();

    _num_sliders = 0;
    _num_switches = 0;
  }
}

void
Pad_Proxies_Group::append_column ( ::Wdg::Pad_Proxies_Column * column_n )
{
  if ( column_n != 0 ) {

    column_n->setParent ( this );
    column_n->set_column_index ( _columns.size () );

    _columns.append ( column_n );
    if ( column_n->has_slider () != 0 ) {
      ++_num_sliders;
    }
    if ( column_n->has_switch () != 0 ) {
      ++_num_switches;
    }
  }
}

void
Pad_Proxies_Group::set_group_name ( const QString & name_n )
{
  _group_name = name_n;
}

void
Pad_Proxies_Group::set_tool_tip ( const QString & tip_n )
{
  _tool_tip = tip_n;
}

void
Pad_Proxies_Group::set_style_id ( unsigned int style_id_n )
{
  _style_id = style_id_n;
}

bool
Pad_Proxies_Group::take_focus ( unsigned int column_n, unsigned int row_n )
{
  bool success ( false );
  if ( column_n >= num_columns () ) {
    // Use first column as fallback
    column_n = 0;
  }
  if ( column_n < num_columns () ) {
    ::Wdg::Pad_Proxies_Column * col ( column ( column_n ) );
    ::Wdg::Pad_Proxy * proxy ( 0 );
    if ( row_n == 0 ) {
      proxy = col->slider_proxy ();
    } else {
      proxy = col->switch_proxy ();
      if ( proxy == 0 ) {
        proxy = col->slider_proxy ();
      }
    }

    if ( proxy != 0 ) {
      QWidget * wdg ( proxy->widget () );
      if ( wdg != 0 ) {
        if ( wdg->isEnabled () ) {
          wdg->setFocus ();
          success = true;
        }
      }
    }
  }
  return success;
}

bool
Pad_Proxies_Group::event ( QEvent * event_n )
{
  if ( event_n->type () == ::Wdg::evt_pass_event_focus ) {
    ::Wdg::Pass_Event_Focus * ev_fp (
        static_cast<::Wdg::Pass_Event_Focus * > ( event_n ) );

    _focus_column = ev_fp->column_idx;
    _focus_row = ev_fp->row_idx;
    _has_focus = ev_fp->ev_focus.gotFocus ();

    if ( pad () != 0 ) {
      ev_fp->group_idx = group_index ();
      QCoreApplication::sendEvent ( pad (), event_n );
    }
    return true;
  }

  return QObject::event ( event_n );
}

} // namespace Wdg
