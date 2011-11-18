//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
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

