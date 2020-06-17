/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_pad_proxy_slider_hpp__
#define __INC_wdg_pad_proxy_slider_hpp__

#include "wdg/pad_proxy.hpp"

namespace Wdg
{

/// @brief Pad_Proxy_Slider
///
class Pad_Proxy_Slider : public ::Wdg::Pad_Proxy
{
  Q_OBJECT;

  // Public methods
  public:
  Pad_Proxy_Slider ();

  ~Pad_Proxy_Slider ();

  unsigned long
  slider_index () const;

  unsigned long
  slider_index_max () const;

  // Signals
  signals:

  void
  sig_slider_index_changed ( unsigned long idx_n );

  void
  sig_slider_index_max_changed ( unsigned long idx_n );

  // Public slots
  public slots:

  void
  set_slider_index ( unsigned long idx_n );

  void
  set_slider_index_max ( unsigned long idx_n );

  // Protected methods
  protected:
  virtual void
  slider_index_changed ();

  virtual void
  slider_index_max_changed ();

  // Private attributes
  private:
  unsigned long _slider_index;
  unsigned long _slider_index_max;
};

inline unsigned long
Pad_Proxy_Slider::slider_index () const
{
  return _slider_index;
}

inline unsigned long
Pad_Proxy_Slider::slider_index_max () const
{
  return _slider_index_max;
}

} // namespace Wdg

#endif
