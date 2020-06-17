/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_pad_proxy_switch_hpp__
#define __INC_wdg_pad_proxy_switch_hpp__

#include "wdg/pad_proxy.hpp"

namespace Wdg
{

/// @brief Pad_Proxy_Switch
///
class Pad_Proxy_Switch : public ::Wdg::Pad_Proxy
{
  Q_OBJECT;

  // Public methods
  public:
  Pad_Proxy_Switch ();

  ~Pad_Proxy_Switch ();

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
  toggle_switch_state ();

  // Protected methods
  protected:
  virtual void
  switch_state_changed ();

  // Private attributes
  private:
  bool _switch_state;
};

inline bool
Pad_Proxy_Switch::switch_state () const
{
  return _switch_state;
}

} // namespace Wdg

#endif
