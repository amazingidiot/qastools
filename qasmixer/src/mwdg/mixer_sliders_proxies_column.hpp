/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_sliders_proxies_column_hpp__
#define __INC_mixer_sliders_proxies_column_hpp__

#include "mixer_sliders_proxy_slider.hpp"
#include "mixer_sliders_proxy_switch.hpp"
#include "wdg/pad_proxies_column.hpp"
#include <QLocale>

namespace MWdg
{

/// @brief Mixer_Sliders_Proxies_Column
///
class Mixer_Sliders_Proxies_Column : public ::Wdg::Pad_Proxies_Column
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_Sliders_Proxies_Column ();

  ~Mixer_Sliders_Proxies_Column ();

  ::MWdg::Mixer_Sliders_Proxy_Slider *
  mslider_proxy () const;

  ::MWdg::Mixer_Sliders_Proxy_Switch *
  mswitch_proxy () const;

  void
  update_mixer_values ();

  // Value string

  QString
  value_string () const;

  QString
  value_min_string () const;

  QString
  value_max_string () const;

  bool
  event ( QEvent * event_n );

  // Protected methods
  protected:
  void
  slider_proxy_changed ();

  void
  switch_proxy_changed ();

  void
  show_value_string_changed ();

  void
  update_connections ();

  void
  dB_string ( QString & str_n, long dB_value_n ) const;

  void
  percent_string ( QString & str_n, int permille_n ) const;

  // Private atributes
  private:
  QString _str_value_dB;
  QString _str_value_pc;
  QLocale _loc;
};

inline ::MWdg::Mixer_Sliders_Proxy_Slider *
Mixer_Sliders_Proxies_Column::mslider_proxy () const
{
  return static_cast<::MWdg::Mixer_Sliders_Proxy_Slider * > ( slider_proxy () );
}

inline ::MWdg::Mixer_Sliders_Proxy_Switch *
Mixer_Sliders_Proxies_Column::mswitch_proxy () const
{
  return static_cast<::MWdg::Mixer_Sliders_Proxy_Switch * > ( switch_proxy () );
}

} // namespace MWdg

#endif
