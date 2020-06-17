/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "pad_proxies_column.hpp"
#include "wdg/event_types.hpp"
#include "wdg/pad_proxies_group.hpp"
#include "wdg/pad_proxy_enum.hpp"
#include "wdg/pad_proxy_slider.hpp"
#include "wdg/pad_proxy_switch.hpp"
#include "wdg/pass_events.hpp"
#include <QCoreApplication>
#include <QFocusEvent>
#include <iostream>

namespace Wdg
{

Pad_Proxies_Column::Pad_Proxies_Column ( unsigned int col_idx_n )
: _column_index ( col_idx_n )
, _proxy_slider ( 0 )
, _proxy_switch ( 0 )
, _proxy_enum ( 0 )
, _has_focus ( false )
, _show_value_string ( true )
{
}

Pad_Proxies_Column::~Pad_Proxies_Column ()
{
  clear_proxies ();
}

::Wdg::Pad_Proxies_Group *
Pad_Proxies_Column::group () const
{
  ::Wdg::Pad_Proxies_Group * res ( 0 );
  QObject * par ( parent () );
  if ( par != 0 ) {
    res = dynamic_cast<::Wdg::Pad_Proxies_Group * > ( par );
  }
  return res;
}

void
Pad_Proxies_Column::set_column_index ( unsigned int idx_n )
{
  _column_index = idx_n;
}

void
Pad_Proxies_Column::clear_proxies ()
{
  if ( _proxy_slider != 0 ) {
    delete _proxy_slider;
    _proxy_slider = 0;
  }
  if ( _proxy_switch != 0 ) {
    delete _proxy_switch;
    _proxy_switch = 0;
  }
  if ( _proxy_enum != 0 ) {
    delete _proxy_enum;
    _proxy_enum = 0;
  }
}

void
Pad_Proxies_Column::set_slider_proxy ( ::Wdg::Pad_Proxy_Slider * proxy_n )
{
  if ( proxy_n != _proxy_slider ) {
    if ( proxy_n != 0 ) {
      proxy_n->setParent ( this );
    }
    _proxy_slider = proxy_n;
    this->slider_proxy_changed ();
  }
}

void
Pad_Proxies_Column::set_switch_proxy ( ::Wdg::Pad_Proxy_Switch * proxy_n )
{
  if ( proxy_n != _proxy_switch ) {
    if ( proxy_n != 0 ) {
      proxy_n->setParent ( this );
    }
    _proxy_switch = proxy_n;
    this->switch_proxy_changed ();
  }
}

void
Pad_Proxies_Column::set_enum_proxy ( ::Wdg::Pad_Proxy_Enum * proxy_n )
{
  if ( proxy_n != _proxy_enum ) {
    if ( proxy_n != 0 ) {
      proxy_n->setParent ( this );
    }
    _proxy_enum = proxy_n;
    this->enum_proxy_changed ();
  }
}

void
Pad_Proxies_Column::set_show_value_string ( bool flag_n )
{
  if ( flag_n != _show_value_string ) {
    _show_value_string = flag_n;
    this->show_value_string_changed ();
  }
}

QString
Pad_Proxies_Column::value_string () const
{
  QString res;
  if ( slider_proxy () != 0 ) {
    const QString mask ( "%1" );
    res = mask.arg ( slider_proxy ()->slider_index () );
  }
  return res;
}

QString
Pad_Proxies_Column::value_min_string () const
{
  QString res;
  if ( slider_proxy () != 0 ) {
    const QString mask ( "%1" );
    res = mask.arg ( 0 );
  }
  return res;
}

QString
Pad_Proxies_Column::value_max_string () const
{
  QString res;
  if ( slider_proxy () != 0 ) {
    const QString mask ( "%1" );
    res = mask.arg ( slider_proxy ()->slider_index_max () );
  }
  return res;
}

void
Pad_Proxies_Column::slider_proxy_changed ()
{
  // Dummy implementation
}

void
Pad_Proxies_Column::switch_proxy_changed ()
{
  // Dummy implementation
}

void
Pad_Proxies_Column::enum_proxy_changed ()
{
  // Dummy implementation
}

void
Pad_Proxies_Column::show_value_string_changed ()
{
  // Dummy implementation
}

bool
Pad_Proxies_Column::event ( QEvent * event_n )
{
  if ( event_n->type () == ::Wdg::evt_pass_event_focus ) {
    ::Wdg::Pass_Event_Focus * ev_fp (
        static_cast<::Wdg::Pass_Event_Focus * > ( event_n ) );

    _has_focus = ev_fp->ev_focus.gotFocus ();
    if ( parent () != 0 ) {
      ev_fp->column_idx = column_index ();
      QCoreApplication::sendEvent ( parent (), event_n );
    }
    return true;
  }

  return QObject::event ( event_n );
}

} // namespace Wdg
