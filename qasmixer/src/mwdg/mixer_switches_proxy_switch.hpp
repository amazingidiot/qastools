/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_switches_proxy_switch_hpp__
#define __INC_mixer_switches_proxy_switch_hpp__

#include "qsnd/mixer_simple_elem.hpp"
#include "wdg/pad_proxy_switch.hpp"

namespace MWdg
{

/// @brief Mixer_Switches_Proxy_Switch
///
class Mixer_Switches_Proxy_Switch : public ::Wdg::Pad_Proxy_Switch
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_Switches_Proxy_Switch ();

  ::QSnd::Mixer_Simple_Elem *
  mixer_simple_elem () const;

  void
  set_mixer_simple_elem ( ::QSnd::Mixer_Simple_Elem * selem_n );

  unsigned char
  snd_dir () const;

  void
  set_snd_dir ( unsigned char dir_n );

  unsigned int
  channel_idx () const;

  void
  set_channel_idx ( unsigned int idx_n );

  // Is joined flag

  bool
  is_joined () const;

  void
  set_is_joined ( bool flag_n );

  bool
  eventFilter ( QObject * obj_n, QEvent * event_n );

  // Public slots
  public slots:

  void
  update_value_from_source ();

  // Protected methods
  protected:
  void
  switch_state_changed ();

  // Private attributes
  private:
  ::QSnd::Mixer_Simple_Elem * _mixer_simple_elem;

  unsigned int _channel_idx;
  unsigned char _snd_dir;

  bool _is_joined;
  bool _alsa_updating;
};

inline ::QSnd::Mixer_Simple_Elem *
Mixer_Switches_Proxy_Switch::mixer_simple_elem () const
{
  return _mixer_simple_elem;
}

inline unsigned char
Mixer_Switches_Proxy_Switch::snd_dir () const
{
  return _snd_dir;
}

inline unsigned int
Mixer_Switches_Proxy_Switch::channel_idx () const
{
  return _channel_idx;
}

inline bool
Mixer_Switches_Proxy_Switch::is_joined () const
{
  return _is_joined;
}

} // namespace MWdg

#endif
