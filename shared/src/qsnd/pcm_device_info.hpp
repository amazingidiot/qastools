/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_pcm_device_info_hpp__
#define __INC_qsnd_pcm_device_info_hpp__

#include "qsnd/alsa.hpp"
#include "qsnd/pcm_subdevices_info.hpp"
#include <QList>
#include <QString>

namespace QSnd
{

/// @brief PCM_Device_Info
///
class PCM_Device_Info
{
  // Public methods
  public:
  PCM_Device_Info ();

  int
  dev_index () const;

  const QString &
  dev_id () const;

  const QString &
  dev_name () const;

  const ::QSnd::PCM_Subdevices_Info &
  subdevices_info ( unsigned int idx_n ) const;

  void
  clear ();

  int
  acquire_device_info ( snd_ctl_t * snd_ctl_handle, int device_idx_n );

  // Private attributes
  private:
  int _dev_index;

  QString _dev_id;
  QString _dev_name;

  ::QSnd::PCM_Subdevices_Info _sdevs_info[ 2 ];
};

inline int
PCM_Device_Info::dev_index () const
{
  return _dev_index;
}

inline const QString &
PCM_Device_Info::dev_id () const
{
  return _dev_id;
}

inline const QString &
PCM_Device_Info::dev_name () const
{
  return _dev_name;
}

inline const ::QSnd::PCM_Subdevices_Info &
PCM_Device_Info::subdevices_info ( unsigned int stream_dir_n ) const
{
  return _sdevs_info[ stream_dir_n ];
}

} // namespace QSnd

#endif
