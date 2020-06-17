/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_proxy.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include <QApplication>
#include <QFocusEvent>

namespace MWdg
{

Mixer_HCTL_Proxy::Mixer_HCTL_Proxy ( QObject * parent_n )
: QObject ( parent_n )
, _snd_elem ( 0 )
, _elem_idx ( 0 )
, _is_enabled ( true )
, _is_joined ( false )
, _has_focus ( false )
{
}

Mixer_HCTL_Proxy::~Mixer_HCTL_Proxy () {}

void
Mixer_HCTL_Proxy::set_snd_elem ( ::QSnd::Mixer_HCTL_Elem * elem_n )
{
  if ( _snd_elem != 0 ) {
    disconnect ( _snd_elem, 0, this, 0 );
  }

  _snd_elem = elem_n;

  if ( _snd_elem != 0 ) {
    connect ( _snd_elem,
              SIGNAL ( sig_values_changed () ),
              this,
              SLOT ( update_value_from_source () ) );
  }
}

void
Mixer_HCTL_Proxy::set_elem_idx ( unsigned int idx_n )
{
  _elem_idx = idx_n;
}

void
Mixer_HCTL_Proxy::set_joined ( bool flag_n )
{
  _is_joined = flag_n;
}

void
Mixer_HCTL_Proxy::set_enabled ( bool flag_n )
{
  if ( flag_n != is_enabled () ) {
    _is_enabled = flag_n;
    emit sig_enabled_changed ( is_enabled () );
  }
}

void
Mixer_HCTL_Proxy::update_value_from_source ()
{
  // Dummy implementation
}

bool
Mixer_HCTL_Proxy::joined_by_key () const
{
  bool res ( true );
  if ( ( QApplication::keyboardModifiers () & Qt::ControlModifier ) == 0 ) {
    res = false;
  }
  res = ( res && has_focus () );
  return res;
}

bool
Mixer_HCTL_Proxy::eventFilter ( QObject * obj_n, QEvent * event_n )
{
  bool res ( QObject::eventFilter ( obj_n, event_n ) );

  if ( !res ) {
    if ( event_n->type () == QEvent::FocusIn ) {
      _has_focus = true;
    } else if ( event_n->type () == QEvent::FocusOut ) {
      _has_focus = false;
    }
  }

  return res;
}

} // namespace MWdg
