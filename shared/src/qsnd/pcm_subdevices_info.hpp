/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_pcm_subdevices_info_hpp__
#define __INC_qsnd_pcm_subdevices_info_hpp__

#include "qsnd/alsa.hpp"
#include "qsnd/pcm_subdevice_info.hpp"
#include <QList>
#include <QString>

namespace QSnd
{

/// @brief PCM_Subdevices_Info
///
class PCM_Subdevices_Info
{
  // Public methods
  public:
  PCM_Subdevices_Info ();

  ~PCM_Subdevices_Info ();

  unsigned int
  stream_dir () const;

  unsigned int
  num_subdevices () const;

  unsigned int
  num_subdevices_avail () const;

  const PCM_Subdevice_Info *
  subdevice_info ( unsigned int idx_n ) const;

  void
  clear ();

  int
  acquire_subdevices ( snd_ctl_t * snd_ctl_handle_n,
                       int dev_idx_n,
                       unsigned int stream_dir_n );

  // Private attributes
  private:
  unsigned int _stream_dir;
  unsigned int _num_sdevs;
  unsigned int _num_sdevs_avail;

  QList<::QSnd::PCM_Subdevice_Info * > _sdevs_info;
};

inline unsigned int
PCM_Subdevices_Info::stream_dir () const
{
  return _stream_dir;
}

inline unsigned int
PCM_Subdevices_Info::num_subdevices () const
{
  return _num_sdevs;
}

inline unsigned int
PCM_Subdevices_Info::num_subdevices_avail () const
{
  return _num_sdevs_avail;
}

inline const ::QSnd::PCM_Subdevice_Info *
PCM_Subdevices_Info::subdevice_info ( unsigned int idx_n ) const
{
  return _sdevs_info[ idx_n ];
}

} // namespace QSnd

#endif
