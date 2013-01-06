//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
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

