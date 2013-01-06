//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "pad_focus_info.hpp"


namespace Wdg
{


Pad_Focus_Info::Pad_Focus_Info ( )
{
	clear();
}


void
Pad_Focus_Info::clear ( )
{
	has_focus = false;
	group_idx = 0;
	column_idx = 0;
	row_idx = 0;
}


} // End of namespace

