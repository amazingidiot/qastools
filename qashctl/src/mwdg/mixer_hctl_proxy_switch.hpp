/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_proxy_switch_hpp__
#define __INC_mixer_hctl_proxy_switch_hpp__

#include "mixer_hctl_proxy.hpp"
#include <QObject>

namespace MWdg
{

/// @brief Mixer_HCTL_Proxy_Switch
///
class Mixer_HCTL_Proxy_Switch : public Mixer_HCTL_Proxy
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL_Proxy_Switch ( QObject * parent_n );

  bool
  switch_state () const;

  // Signals
  signals:

  void
  sig_switch_state_changed ( bool state_n );

  // Public slots
  public slots:

  void
  set_switch_state ( bool state_n );

  void
  update_value_from_source ();

  // Protected methods
  protected:
  void
  switch_state_changed ();

  // Private attributes
  private:
  bool _switch_state;
  bool _updating_state;
};

inline bool
Mixer_HCTL_Proxy_Switch::switch_state () const
{
  return _switch_state;
}

} // namespace MWdg

#endif
