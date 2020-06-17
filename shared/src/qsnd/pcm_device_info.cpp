/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "pcm_device_info.hpp"
#include <iostream>

namespace QSnd
{

PCM_Device_Info::PCM_Device_Info ()
{
  clear ();
}

void
PCM_Device_Info::clear ()
{
  _dev_index = -1;

  _dev_id.clear ();
  _dev_name.clear ();

  _sdevs_info[ 0 ].clear ();
  _sdevs_info[ 1 ].clear ();
}

int
PCM_Device_Info::acquire_device_info ( snd_ctl_t * snd_ctl_handle_n,
                                       int device_idx_n )
{
  int err ( 0 );

  clear ();

  if ( snd_ctl_handle_n == 0 ) {
    return -1;
  }

  _dev_index = device_idx_n;

  snd_pcm_info_t * pcm_info;
  snd_pcm_info_alloca ( &pcm_info );

  // Read Playback info
  snd_pcm_info_set_device ( pcm_info, _dev_index );
  snd_pcm_info_set_subdevice ( pcm_info, 0 );
  snd_pcm_info_set_stream ( pcm_info, SND_PCM_STREAM_PLAYBACK );

  err = snd_ctl_pcm_info ( snd_ctl_handle_n, pcm_info );
  if ( err < 0 ) {
    snd_pcm_info_set_device ( pcm_info, _dev_index );
    snd_pcm_info_set_subdevice ( pcm_info, 0 );
    snd_pcm_info_set_stream ( pcm_info, SND_PCM_STREAM_CAPTURE );
  }

  err = snd_ctl_pcm_info ( snd_ctl_handle_n, pcm_info );
  if ( err >= 0 ) {

    _dev_id = snd_pcm_info_get_id ( pcm_info );
    _dev_name = snd_pcm_info_get_name ( pcm_info );

    for ( unsigned int ii = 0; ii < 2; ++ii ) {
      _sdevs_info[ ii ].acquire_subdevices ( snd_ctl_handle_n, _dev_index, ii );
    }
  }

  if ( err < 0 ) {
    clear ();
  }

  return err;
}

} // namespace QSnd
