/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_sliders_status_widget_hpp__
#define __INC_mixer_sliders_status_widget_hpp__

#include "mwdg/mixer_sliders_proxies_group.hpp"
#include "mwdg/slider_status_widget.hpp"

namespace MWdg
{

/// @brief Mixer_Sliders_Status_Widget
///
class Mixer_Sliders_Status_Widget : public Slider_Status_Widget
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_Sliders_Status_Widget ( QWidget * parent_n = 0 );

  ~Mixer_Sliders_Status_Widget ();

  ::MWdg::Mixer_Sliders_Proxy_Slider *
  proxy_slider () const;

  // Element info

  QString
  elem_name () const;

  bool
  elem_has_volume () const;

  bool
  elem_has_dB () const;

  long
  elem_volume_value () const;

  void
  elem_set_volume ( long value_n ) const;

  long
  elem_volume_min () const;

  long
  elem_volume_max () const;

  long
  elem_dB_value () const;

  void
  elem_set_nearest_dB ( long dB_value_n ) const;

  long
  elem_dB_min () const;

  long
  elem_dB_max () const;

  // Slider selection

  void
  select_slider ( unsigned int grp_idx_n, unsigned int col_idx_n );

  // Protected slots
  protected slots:

  void
  proxy_destroyed ();

  // Protected methods
  protected:
  void
  set_slider_proxy ( ::MWdg::Mixer_Sliders_Proxy_Slider * proxy_n );

  // Private attributes
  private:
  ::MWdg::Mixer_Sliders_Proxy_Slider * _proxy_slider;
};

inline ::MWdg::Mixer_Sliders_Proxy_Slider *
Mixer_Sliders_Status_Widget::proxy_slider () const
{
  return _proxy_slider;
}

} // namespace MWdg

#endif
