/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_proxy_enum_hpp__
#define __INC_mixer_hctl_proxy_enum_hpp__

#include "mixer_hctl_proxy.hpp"
#include <QObject>

namespace MWdg
{

///
/// @brief Mixer_HCTL_Proxy_Enum
///
class Mixer_HCTL_Proxy_Enum : public Mixer_HCTL_Proxy
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL_Proxy_Enum ( QObject * parent_n );

  unsigned int
  enum_index () const;

  unsigned int
  enum_num_items () const;

  const char *
  enum_item_name ( unsigned int index_n );

  // Signals
  signals:

  void
  sig_enum_index_changed ( unsigned int index_n );

  void
  sig_enum_index_changed ( int index_n );

  // Public slots
  public slots:

  void
  set_enum_index ( unsigned int index_n );

  void
  set_enum_index ( int index_n );

  void
  update_value_from_source ();

  // Protected methods
  protected:
  void
  enum_index_changed ();

  // Private attributes
  private:
  unsigned int _enum_index;

  bool _updating_state;
};

inline unsigned int
Mixer_HCTL_Proxy_Enum::enum_index () const
{
  return _enum_index;
}

} // namespace MWdg

#endif
