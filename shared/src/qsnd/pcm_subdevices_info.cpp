/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "pcm_subdevices_info.hpp"
#include <iostream>

namespace QSnd
{

PCM_Subdevices_Info::PCM_Subdevices_Info ()
: _stream_dir ( 0 )
, _num_sdevs ( 0 )
, _num_sdevs_avail ( 0 )
{
}

PCM_Subdevices_Info::~PCM_Subdevices_Info ()
{
  clear ();
}

void
PCM_Subdevices_Info::clear ()
{
  _stream_dir = 0;
  _num_sdevs = 0;
  _num_sdevs_avail = 0;

  if ( _sdevs_info.size () > 0 ) {
    for ( int ii = 0; ii < _sdevs_info.size (); ++ii ) {
      delete _sdevs_info[ ii ];
    }
    _sdevs_info.clear ();
  }
}

int
PCM_Subdevices_Info::acquire_subdevices ( snd_ctl_t * snd_ctl_handle_n,
                                          int dev_idx_n,
                                          unsigned int stream_dir_n )
{
  int err ( 0 );

  clear ();
  _stream_dir = stream_dir_n;

  snd_pcm_stream_t pcm_stream ( SND_PCM_STREAM_PLAYBACK );
  if ( _stream_dir > 0 ) {
    pcm_stream = SND_PCM_STREAM_CAPTURE;
  }

  snd_pcm_info_t * pcm_info;
  snd_pcm_info_alloca ( &pcm_info );

  snd_pcm_info_set_device ( pcm_info, dev_idx_n );
  snd_pcm_info_set_subdevice ( pcm_info, 0 );
  snd_pcm_info_set_stream ( pcm_info, pcm_stream );

  err = snd_ctl_pcm_info ( snd_ctl_handle_n, pcm_info );
  if ( err >= 0 ) {

    _num_sdevs = snd_pcm_info_get_subdevices_count ( pcm_info );
    _num_sdevs_avail = snd_pcm_info_get_subdevices_avail ( pcm_info );

    // Read subdevices info
    for ( unsigned int ii = 0; ii < _num_sdevs; ++ii ) {

      snd_pcm_info_set_device ( pcm_info, dev_idx_n );
      snd_pcm_info_set_subdevice ( pcm_info, ii );
      snd_pcm_info_set_stream ( pcm_info, pcm_stream );

      PCM_Subdevice_Info * sdev_info ( new PCM_Subdevice_Info );
      err = snd_ctl_pcm_info ( snd_ctl_handle_n, pcm_info );
      if ( err >= 0 ) {
        sdev_info->acquire_info ( pcm_info );
      }
      _sdevs_info.append ( sdev_info );
    }
  }

  return err;
}

} // namespace QSnd
