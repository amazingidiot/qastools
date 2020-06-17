/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_sliders_proxy_slider_hpp__
#define __INC_mixer_sliders_proxy_slider_hpp__

#include "qsnd/mixer_simple_elem.hpp"
#include "wdg/pad_proxy_slider.hpp"
#include <QString>

namespace MWdg
{

/// @brief Mixer_Sliders_Proxy_Slider
///
class Mixer_Sliders_Proxy_Slider : public ::Wdg::Pad_Proxy_Slider
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_Sliders_Proxy_Slider ();

  // Mixer simple elem

  ::QSnd::Mixer_Simple_Elem *
  mixer_simple_elem () const;

  void
  set_mixer_simple_elem ( ::QSnd::Mixer_Simple_Elem * selem_n );

  // Snd dir

  unsigned char
  snd_dir () const;

  void
  set_snd_dir ( unsigned char dir_n );

  // Channel idx

  unsigned int
  channel_idx () const;

  void
  set_channel_idx ( unsigned int idx_n );

  // Is joined flag

  bool
  is_joined () const;

  void
  set_is_joined ( bool flag_n );

  // Volume

  long
  volume_max () const;

  long
  volume_min () const;

  long
  volume_value () const;

  int
  volume_permille () const;

  // Decibel

  bool
  has_dB () const;

  long
  ask_dB_vol ( long dB_value_n, int dir_n = -1 );

  long
  ask_dB_vol_nearest ( long dB_value_n );

  long
  ask_vol_dB ( long volume_n );

  long
  dB_value () const;

  void
  set_dB_value ( long dB_val_n );

  long
  dB_max () const;

  long
  dB_min () const;

  bool
  eventFilter ( QObject * obj_n, QEvent * event_n );

  // Public signals
  signals:

  void
  sig_dB_value_changed ( long dB_value_n );

  // Public slots
  public slots:

  void
  set_volume_value ( long value_n );

  void
  update_value_from_source ();

  // Protected methods
  protected:
  void
  update_limits ();

  void
  dB_value_changed ();

  void
  volume_value_changed ();

  void
  slider_index_changed ();

  // Private attributes
  private:
  ::QSnd::Mixer_Simple_Elem * _mixer_simple_elem;

  long _volume_val;
  long _volume_min;
  long _volume_max;
  long _dB_val;

  unsigned int _channel_idx;
  unsigned char _snd_dir;

  bool _is_joined;
  bool _alsa_updating;
};

inline ::QSnd::Mixer_Simple_Elem *
Mixer_Sliders_Proxy_Slider::mixer_simple_elem () const
{
  return _mixer_simple_elem;
}

inline unsigned char
Mixer_Sliders_Proxy_Slider::snd_dir () const
{
  return _snd_dir;
}

inline unsigned int
Mixer_Sliders_Proxy_Slider::channel_idx () const
{
  return _channel_idx;
}

inline bool
Mixer_Sliders_Proxy_Slider::is_joined () const
{
  return _is_joined;
}

inline long
Mixer_Sliders_Proxy_Slider::volume_min () const
{
  return _volume_min;
}

inline long
Mixer_Sliders_Proxy_Slider::volume_value () const
{
  return _volume_val;
}

inline long
Mixer_Sliders_Proxy_Slider::volume_max () const
{
  return _volume_max;
}

inline bool
Mixer_Sliders_Proxy_Slider::has_dB () const
{
  if ( mixer_simple_elem () == 0 ) {
    return false;
  }
  return mixer_simple_elem ()->has_dB ( snd_dir () );
}

inline long
Mixer_Sliders_Proxy_Slider::ask_dB_vol ( long dB_value_n, int dir_n )
{
  return mixer_simple_elem ()->ask_dB_vol ( snd_dir (), dB_value_n, dir_n );
}

inline long
Mixer_Sliders_Proxy_Slider::ask_dB_vol_nearest ( long dB_value_n )
{
  return mixer_simple_elem ()->ask_dB_vol_nearest ( snd_dir (), dB_value_n );
}

inline long
Mixer_Sliders_Proxy_Slider::ask_vol_dB ( long volume_n )
{
  return mixer_simple_elem ()->ask_vol_dB ( snd_dir (), volume_n );
}

inline long
Mixer_Sliders_Proxy_Slider::dB_value () const
{
  return _dB_val;
}

inline long
Mixer_Sliders_Proxy_Slider::dB_min () const
{
  return mixer_simple_elem ()->dB_min ( snd_dir () );
}

inline long
Mixer_Sliders_Proxy_Slider::dB_max () const
{
  return mixer_simple_elem ()->dB_max ( snd_dir () );
}

} // namespace MWdg

#endif
