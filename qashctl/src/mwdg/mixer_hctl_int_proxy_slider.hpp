/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_int_proxy_slider_hpp__
#define __INC_mixer_hctl_int_proxy_slider_hpp__

#include "wdg/pad_proxy_slider.hpp"
#include "wdg/uint_mapper.hpp"
#include <QObject>

// Forward declaration
namespace QSnd
{
class Mixer_HCTL_Elem;
}

namespace MWdg
{

/// @brief Mixer_HCTL_Int_Proxy_Slider
///
class Mixer_HCTL_Int_Proxy_Slider : public ::Wdg::Pad_Proxy_Slider
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL_Int_Proxy_Slider ( ::QSnd::Mixer_HCTL_Elem * elem_n,
                                unsigned int elem_idx_n );

  ~Mixer_HCTL_Int_Proxy_Slider ();

  // Snd element

  ::QSnd::Mixer_HCTL_Elem *
  snd_elem () const;

  unsigned int
  elem_idx () const;

  // Joined

  bool
  is_joined () const;

  void
  set_joined ( bool flag_n );

  bool
  joined_by_key () const;

  // Integer value

  long
  integer_min () const;

  long
  integer_max () const;

  long
  integer_value () const;

  unsigned long
  integer_to_index ( long integer_n ) const;

  long
  index_to_integer ( unsigned long index_n ) const;

  // Signals
  signals:

  void
  sig_integer_value_changed ( long value_n );

  void
  sig_integer_value_changed ( int value_n );

  // Public slots
  public slots:

  void
  set_integer_value ( long value_n );

  void
  set_integer_value ( int value_n );

  void
  update_value_from_source ();

  // Protected methods
  protected:
  void
  integer_value_changed ();

  void
  slider_index_changed ();

  // Private attributes
  private:
  ::QSnd::Mixer_HCTL_Elem * _snd_elem;
  unsigned int _elem_idx;

  long _integer_value;
  bool _is_joined;
  bool _updating_state;
};

inline ::QSnd::Mixer_HCTL_Elem *
Mixer_HCTL_Int_Proxy_Slider::snd_elem () const
{
  return _snd_elem;
}

inline unsigned int
Mixer_HCTL_Int_Proxy_Slider::elem_idx () const
{
  return _elem_idx;
}

inline bool
Mixer_HCTL_Int_Proxy_Slider::is_joined () const
{
  return _is_joined;
}

inline long
Mixer_HCTL_Int_Proxy_Slider::integer_value () const
{
  return _integer_value;
}

inline unsigned long
Mixer_HCTL_Int_Proxy_Slider::integer_to_index ( long integer_n ) const
{
  unsigned long res ( ::Wdg::integer_distance ( integer_min (), integer_n ) );
  return res;
}

inline long
Mixer_HCTL_Int_Proxy_Slider::index_to_integer ( unsigned long index_n ) const
{
  long res ( integer_min () );
  res += index_n;
  return res;
}

} // namespace MWdg

#endif
