//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "ds_widget_style_db.hpp"


namespace Wdg
{


DS_Widget_Style_Db::DS_Widget_Style_Db ( )
{
	QPalette pal;
	palettes.insert ( ST_NORMAL, pal );
}


} // End of namespace

