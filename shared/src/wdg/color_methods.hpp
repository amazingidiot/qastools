//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg_color_methods_hpp__
#define __INC_wdg_color_methods_hpp__

#include <QColor>


namespace Wdg
{


QColor
col_mix (
	const QColor & col_1,
	const QColor & col_2,
	int w_1,
	int w_2 );


} // End of namespace


#endif
