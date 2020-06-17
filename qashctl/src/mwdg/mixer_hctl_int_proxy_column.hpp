/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_int_proxy_column_hpp__
#define __INC_mixer_hctl_int_proxy_column_hpp__

#include "mixer_hctl_int_proxy_slider.hpp"
#include "wdg/pad_proxies_column.hpp"
#include <QLocale>
#include <QObject>

namespace MWdg
{

///
/// @brief Mixer_HCTL_Int_Proxy_Column
///
class Mixer_HCTL_Int_Proxy_Column : public ::Wdg::Pad_Proxies_Column
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL_Int_Proxy_Column ();

  ~Mixer_HCTL_Int_Proxy_Column ();

  ::MWdg::Mixer_HCTL_Int_Proxy_Slider *
  mslider_proxy () const;

  // Value string

  QString
  value_string () const;

  QString
  value_min_string () const;

  QString
  value_max_string () const;

  // Protected methods
  protected:
  void
  slider_proxy_changed ();

  void
  show_value_string_changed ();

  void
  update_connections ();

  void
  integer_string ( QString & str_n, long value_n ) const;

  void
  dB_string ( QString & str_n, long dB_value_n ) const;

  void
  percent_string ( QString & str_n, int permille_n ) const;

  // Private attributes
  private:
  QString _str_value_dB;
  QString _str_value_pc;
  QLocale _loc;
};

inline ::MWdg::Mixer_HCTL_Int_Proxy_Slider *
Mixer_HCTL_Int_Proxy_Column::mslider_proxy () const
{
  return static_cast<::MWdg::Mixer_HCTL_Int_Proxy_Slider * > (
      slider_proxy () );
}

} // namespace MWdg

#endif
