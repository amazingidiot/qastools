/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "pcm_subdevice_info.hpp"
#include <iostream>

namespace QSnd
{

PCM_Subdevice_Info::PCM_Subdevice_Info ( snd_pcm_info_t * pcm_info_n )
{
  acquire_info ( pcm_info_n );
}

int
PCM_Subdevice_Info::acquire_info ( snd_pcm_info_t * pcm_info_n )
{
  if ( pcm_info_n != 0 ) {
    _dev_index = snd_pcm_info_get_subdevice ( pcm_info_n );
    _dev_name = snd_pcm_info_get_subdevice_name ( pcm_info_n );
  } else {
    _dev_index = 0;
    _dev_name.clear ();
  }

  return 0;
}

} // namespace QSnd
