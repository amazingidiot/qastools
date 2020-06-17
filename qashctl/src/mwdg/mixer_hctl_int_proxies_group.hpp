/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_int_proxies_group_hpp__
#define __INC_mixer_hctl_int_proxies_group_hpp__

#include "wdg/pad_proxies_group.hpp"

// Forward declaration
namespace QSnd
{
class Mixer_HCTL_Elem;
}
namespace MWdg
{
class Mixer_HCTL_Int_Proxy_Column;
}

namespace MWdg
{

/// @brief Mixer_HCTL_Int_Proxies_Group
///
class Mixer_HCTL_Int_Proxies_Group : public ::Wdg::Pad_Proxies_Group
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL_Int_Proxies_Group ( ::QSnd::Mixer_HCTL_Elem * snd_elem_n,
                                 QObject * parent_n );

  ~Mixer_HCTL_Int_Proxies_Group ();

  // Notify parent on value change

  bool
  notify_value_change () const;

  void
  set_notify_value_change ( bool flag_n );

  bool
  is_joined () const;

  bool
  volumes_equal () const;

  Mixer_HCTL_Int_Proxy_Column *
  mcolumn ( unsigned int idx_n ) const;

  // Public slots
  public slots:

  void
  set_joined ( bool flag_n );

  void
  level_volumes ( unsigned int column_n = 0 );

  void
  update_values ();

  // Private attributes
  private:
  ::QSnd::Mixer_HCTL_Elem * _snd_elem;
  bool _notify_value_change;
};

inline bool
Mixer_HCTL_Int_Proxies_Group::notify_value_change () const
{
  return _notify_value_change;
}

} // namespace MWdg

#endif
