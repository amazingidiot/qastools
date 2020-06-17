/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_pcm_subdevice_info_hpp__
#define __INC_qsnd_pcm_subdevice_info_hpp__

#include "qsnd/alsa.hpp"
#include <QString>

namespace QSnd
{

/// @brief PCM_Subdevice_Info
///
class PCM_Subdevice_Info
{
  // Public methods
  public:
  PCM_Subdevice_Info ( snd_pcm_info_t * pcm_info_n = 0 );

  int
  dev_idx () const;

  const QString &
  dev_name () const;

  int
  acquire_info ( snd_pcm_info_t * pcm_info_n );

  // Private attributes
  private:
  int _dev_index;
  QString _dev_name;
};

inline int
PCM_Subdevice_Info::dev_idx () const
{
  return _dev_index;
}

inline const QString &
PCM_Subdevice_Info::dev_name () const
{
  return _dev_name;
}

} // namespace QSnd

#endif
