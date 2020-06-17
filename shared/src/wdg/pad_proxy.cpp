/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "pad_proxy.hpp"
#include "wdg/pad_proxies_column.hpp"
#include "wdg/pad_proxy_style.hpp"
#include "wdg/pass_events.hpp"
#include <QCoreApplication>
#include <QFocusEvent>
#include <iostream>

namespace Wdg
{

Pad_Proxy::Pad_Proxy ( unsigned char index_n )
: _index ( index_n )
, _has_focus ( false )
, _is_enabled ( true )
, _variant_id ( 0 )
, _style_id ( 0 )
, _widget ( 0 )
, _style ( 0 )
{
}

Pad_Proxy::~Pad_Proxy ()
{
  if ( _style != 0 ) {
    delete _style;
  }
}

::Wdg::Pad_Proxies_Column *
Pad_Proxy::column () const
{
  ::Wdg::Pad_Proxies_Column * res ( 0 );
  QObject * par ( parent () );
  if ( par != 0 ) {
    res = dynamic_cast<::Wdg::Pad_Proxies_Column * > ( par );
  }
  return res;
}

::Wdg::Pad_Proxies_Group *
Pad_Proxy::group () const
{
  Pad_Proxies_Group * res ( 0 );
  Pad_Proxies_Column * sp_col ( column () );
  if ( sp_col != 0 ) {
    res = sp_col->group ();
  }
  return res;
}

void
Pad_Proxy::set_index ( unsigned char idx_n )
{
  _index = idx_n;
}

void
Pad_Proxy::set_has_focus ( bool flag_n )
{
  _has_focus = flag_n;
}

void
Pad_Proxy::set_enabled ( bool flag_n )
{
  if ( flag_n != is_enabled () ) {
    _is_enabled = flag_n;
    emit sig_enabled_changed ( is_enabled () );
  }
}

void
Pad_Proxy::set_widget ( QWidget * wdg_n )
{
  if ( _widget != 0 ) {
    _widget->removeEventFilter ( this );
  }

  _widget = wdg_n;

  if ( _widget != 0 ) {
    _widget->installEventFilter ( this );
  }
}

void
Pad_Proxy::set_item_name ( const QString & name_n )
{
  _item_name = name_n;
}

void
Pad_Proxy::set_group_name ( const QString & name_n )
{
  _group_name = name_n;
}

void
Pad_Proxy::set_tool_tip ( const QString & tip_n )
{
  _tool_tip = tip_n;
}

void
Pad_Proxy::set_variant_id ( unsigned int id_n )
{
  _variant_id = id_n;
}

void
Pad_Proxy::set_style_id ( unsigned int id_n )
{
  _style_id = id_n;
}

void
Pad_Proxy::set_style ( ::Wdg::Pad_Proxy_Style * style_n )
{
  if ( _style != 0 ) {
    delete _style;
  }
  _style = style_n;
}

void
Pad_Proxy::update_value_from_source ()
{
  // Dummy implementation does nothing
}

bool
Pad_Proxy::eventFilter ( QObject * obj_n, QEvent * event_n )
{
  bool res ( QObject::eventFilter ( obj_n, event_n ) );

  if ( !res && ( ( event_n->type () == QEvent::FocusIn ) ||
                 ( event_n->type () == QEvent::FocusOut ) ) ) {
    QFocusEvent * ev_foc ( static_cast< QFocusEvent * > ( event_n ) );

    _has_focus = ev_foc->gotFocus ();
    if ( parent () != 0 ) {
      ::Wdg::Pass_Event_Focus ev_pass ( *ev_foc );
      ev_pass.row_idx = index ();
      QCoreApplication::sendEvent ( parent (), &ev_pass );
    }
  }

  return res;
}

} // namespace Wdg
