/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_sliders_proxy_slider.hpp"
#include "wdg/pass_events.hpp"
#include "wdg/uint_mapper.hpp"
#include <QApplication>
#include <iostream>

namespace MWdg
{

Mixer_Sliders_Proxy_Slider::Mixer_Sliders_Proxy_Slider ()
: _mixer_simple_elem ( 0 )
, _volume_val ( 0 )
, _volume_min ( 0 )
, _volume_max ( 0 )
, _dB_val ( 0 )
, _channel_idx ( 0 )
, _snd_dir ( 0 )
, _alsa_updating ( false )
{
  update_limits ();
}

void
Mixer_Sliders_Proxy_Slider::set_snd_dir ( unsigned char dir_n )
{
  _snd_dir = dir_n;
  update_limits ();
}

void
Mixer_Sliders_Proxy_Slider::set_channel_idx ( unsigned int idx_n )
{
  _channel_idx = idx_n;
}

void
Mixer_Sliders_Proxy_Slider::set_mixer_simple_elem (
    ::QSnd::Mixer_Simple_Elem * selem_n )
{
  _mixer_simple_elem = selem_n;
  update_limits ();
}

void
Mixer_Sliders_Proxy_Slider::set_is_joined ( bool flag_n )
{
  _is_joined = flag_n;
}

void
Mixer_Sliders_Proxy_Slider::set_dB_value ( long dB_val_n )
{
  if ( has_dB () ) {
    if ( _dB_val != dB_val_n ) {
      _dB_val = dB_val_n;
      this->dB_value_changed ();
      emit sig_dB_value_changed ( _dB_val );
    }
  }
}

void
Mixer_Sliders_Proxy_Slider::update_limits ()
{
  unsigned long idx_max ( 0 );
  if ( mixer_simple_elem () == 0 ) {
    _volume_min = 0;
    _volume_max = 0;
  } else {
    _volume_min = mixer_simple_elem ()->volume_min ( snd_dir () );
    _volume_max = mixer_simple_elem ()->volume_max ( snd_dir () );
    idx_max = ::Wdg::integer_distance ( _volume_min, _volume_max );
  }

  set_slider_index_max ( idx_max );
}

void
Mixer_Sliders_Proxy_Slider::set_volume_value ( long value_n )
{
  if ( _volume_val != value_n ) {
    _volume_val = value_n;
    this->volume_value_changed ();
  }
}

int
Mixer_Sliders_Proxy_Slider::volume_permille () const
{
  int res;
  unsigned long val;
  unsigned long max;
  if ( _volume_val >= 0 ) {
    val = _volume_val;
    max = _volume_max;
  } else {
    val = -_volume_val;
    max = -_volume_min;
  }

  if ( _volume_max >= ( 1000 * 1000 ) ) {
    max /= 1000;
  } else {
    val *= 1000;
  }
  res = val / max;
  if ( _volume_val < 0 ) {
    res = -res;
  }
  return res;
}

void
Mixer_Sliders_Proxy_Slider::dB_value_changed ()
{
  //::std::cout << "Mixer_Sliders_Proxy_Slider::dB_value_changed " <<
  //slider_value() << "\n";

  if ( ( mixer_simple_elem () != 0 ) && ( !_alsa_updating ) ) {
    if ( is_joined () ) {
      mixer_simple_elem ()->set_dB_all ( snd_dir (), dB_value () );
    } else {
      mixer_simple_elem ()->set_dB ( snd_dir (), channel_idx (), dB_value () );
    }
  }

  //::std::cout << "Mixer_Sliders_Proxy_Slider::dB_value_changed " << "done" <<
  //"\n";
}

void
Mixer_Sliders_Proxy_Slider::volume_value_changed ()
{
  //::std::cout << "Mixer_Sliders_Proxy_Slider::volume_value_changed " <<
  //volume_value() << "\n";
  {
    unsigned long idx (
        ::Wdg::integer_distance ( volume_min (), volume_value () ) );
    set_slider_index ( idx );
  }

  if ( ( mixer_simple_elem () != 0 ) && ( !_alsa_updating ) ) {
    bool key_mod (
        ( QApplication::keyboardModifiers () & Qt::ControlModifier ) != 0 );
    key_mod = ( key_mod && has_focus () );
    if ( is_joined () || key_mod ) {
      mixer_simple_elem ()->set_volume_all ( snd_dir (), volume_value () );
    } else {
      mixer_simple_elem ()->set_volume (
          snd_dir (), channel_idx (), volume_value () );
    }
  }
}

void
Mixer_Sliders_Proxy_Slider::slider_index_changed ()
{
  //::std::cout << "Mixer_Sliders_Proxy_Slider::slider_index_changed " <<
  //slider_index() << "\n";

  {
    long vol ( _volume_min );
    vol += slider_index ();
    set_volume_value ( vol );
  }
}

void
Mixer_Sliders_Proxy_Slider::update_value_from_source ()
{
  if ( ( mixer_simple_elem () != 0 ) && ( !_alsa_updating ) ) {
    _alsa_updating = true;
    if ( has_dB () ) {
      set_dB_value (
          mixer_simple_elem ()->dB_value ( snd_dir (), channel_idx () ) );
    }
    set_volume_value (
        mixer_simple_elem ()->volume ( snd_dir (), channel_idx () ) );
    _alsa_updating = false;
  }
}

bool
Mixer_Sliders_Proxy_Slider::eventFilter ( QObject * obj_n, QEvent * event_n )
{
  bool res ( Pad_Proxy_Slider::eventFilter ( obj_n, event_n ) );

  if ( !res ) {
    if ( ( event_n->type () == QEvent::KeyPress ) ||
         ( event_n->type () == QEvent::KeyRelease ) ||
         ( event_n->type () == QEvent::ShortcutOverride ) ) {
      QKeyEvent * ev_kev ( static_cast< QKeyEvent * > ( event_n ) );
      // Pass certain key events to the parent proxy

      if ( parent () != 0 ) {
        switch ( ev_kev->key () ) {
        case Qt::Key_Space:
        case Qt::Key_VolumeMute: {
          ::Wdg::Pass_Event_Key ev_pass ( *ev_kev, 0 );
          QCoreApplication::sendEvent ( parent (), &ev_pass );
        } break;
        default:
          break;
        }
      }
    }
  }

  return res;
}

} // namespace MWdg
