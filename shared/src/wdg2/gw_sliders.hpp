//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_slider_hpp__
#define __INC_wdg2_gw_slider_hpp__

#include <QGraphicsItem>
#include "qsnd2/control_proxies.hpp"


namespace Wdg2
{


/// @brief GW_Sliders
///
class GW_Sliders :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Sliders (
		::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Sliders ( );


	// Private methods
	private:

	// Private attributes
	private:

	::QSnd2::Proxies_Group1_Slider & _snd_proxies;
};


} // End of namespace

#endif
