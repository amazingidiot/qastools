/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_gui_state_hpp__
#define __INC_mixer_gui_state_hpp__

#include <QString>

namespace MWdg
{

/// @brief Identifies a proxy element
///
class Mixer_GUI_State_Proxy
{
  // Public methods
  public:
  Mixer_GUI_State_Proxy ();

  void
  clear ();

  bool
  is_clear () const;

  // Public attributes
  public:
  QString group_name;
  unsigned char snd_dir;
  unsigned char column_idx;
  unsigned char row_idx;
  bool has_focus;
};

inline bool
Mixer_GUI_State_Proxy::is_clear () const
{
  return group_name.isEmpty ();
}

} // namespace MWdg

#endif
