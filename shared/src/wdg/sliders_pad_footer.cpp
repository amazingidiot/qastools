//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "sliders_pad_footer.hpp"
#include "sliders_pad_data.hpp"

#include <QPainter>
#include <iostream>


namespace Wdg
{


Sliders_Pad_Footer::Sliders_Pad_Footer (
	Sliders_Pad_Data * sp_data_n,
	Sliders_Pad_Style * sp_style_n,
	QWidget * parent_n ) :
Sliders_Pad_Header ( sp_data_n, sp_style_n, parent_n )
{
	hd_data().upside_down = true;
	hd_data().column_labels = true;
	hd_data().label_sliding = false;

	update_painter_states();
}


} // End of namespace
