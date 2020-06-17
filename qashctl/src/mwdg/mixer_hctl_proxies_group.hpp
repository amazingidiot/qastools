/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_proxies_group_hpp__
#define __INC_mixer_hctl_proxies_group_hpp__

#include "mixer_hctl_proxy.hpp"
#include <QObject>

namespace MWdg
{

///
/// @brief Mixer_HCTL_Proxies_Group
///
class Mixer_HCTL_Proxies_Group : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL_Proxies_Group ( QObject * parent_n );

  ~Mixer_HCTL_Proxies_Group ();

  unsigned int
  num_proxies () const;

  Mixer_HCTL_Proxy *
  proxy ( unsigned int idx_n ) const;

  void
  append_proxy ( Mixer_HCTL_Proxy * proxy_n );

  // Public slots
  public slots:

  void
  update_values ();

  void
  set_joined ( bool flag_n );

  // Private attributes
  private:
  QList< Mixer_HCTL_Proxy * > _proxies;
};

inline unsigned int
Mixer_HCTL_Proxies_Group::num_proxies () const
{
  return _proxies.size ();
}

inline Mixer_HCTL_Proxy *
Mixer_HCTL_Proxies_Group::proxy ( unsigned int idx_n ) const
{
  return _proxies[ idx_n ];
}

inline void
Mixer_HCTL_Proxies_Group::append_proxy ( Mixer_HCTL_Proxy * proxy_n )
{
  if ( proxy_n != 0 ) {
    _proxies.append ( proxy_n );
    proxy_n->setParent ( this );
  }
}

} // namespace MWdg

#endif
