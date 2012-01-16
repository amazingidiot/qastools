//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "mixer_hctl_editor_data.hpp"

#include "qastools_config.hpp"
//#include "wdg/ds_switch_painter_svg.hpp"

#include <iostream>


namespace MWdg
{


Mixer_HCTL_Editor_Data::Mixer_HCTL_Editor_Data ( ) :
ctl_info_db ( 0 ),
wdg_style_db ( 0 ),
snd_elem_group ( 0 ),
elem_idx ( 0 ),
image_alloc ( 0 )
{
}


Mixer_HCTL_Editor_Data::~Mixer_HCTL_Editor_Data ( )
{
}


} // End of namespace
