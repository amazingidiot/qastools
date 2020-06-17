/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_switches_proxy_enum_hpp__
#define __INC_mixer_switches_proxy_enum_hpp__

#include "qsnd/mixer_simple_elem.hpp"
#include "wdg/pad_proxy_enum.hpp"

namespace MWdg
{

/// @brief Mixer_Switches_Proxy_Enum
///
class Mixer_Switches_Proxy_Enum : public ::Wdg::Pad_Proxy_Enum
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_Switches_Proxy_Enum ();

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

  QString
  enum_item_name ( int idx_n );

  // Public slots
  public slots:

  void
  update_value_from_source ();

  // Protected methods
  protected:
  void
  enum_index_changed ();

  // Private attributes
  private:
  ::QSnd::Mixer_Simple_Elem * _mixer_simple_elem;

  unsigned int _channel_idx;
  unsigned char _snd_dir;
  bool _is_joined;
  bool _alsa_updating;
};

inline ::QSnd::Mixer_Simple_Elem *
Mixer_Switches_Proxy_Enum::mixer_simple_elem () const
{
  return _mixer_simple_elem;
}

inline unsigned char
Mixer_Switches_Proxy_Enum::snd_dir () const
{
  return _snd_dir;
}

inline unsigned int
Mixer_Switches_Proxy_Enum::channel_idx () const
{
  return _channel_idx;
}

inline bool
Mixer_Switches_Proxy_Enum::is_joined () const
{
  return _is_joined;
}

} // namespace MWdg

#endif
