//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_switches_hpp__
#define __INC_wdg2_gw_switches_hpp__

#include <QGraphicsItem>
#include "qsnd2/controls_proxies.hpp"


namespace Wdg2
{


/// @brief GW_Switches
///
class GW_Switches :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Switches (
		::QSnd2::Proxies_Group1_Switch & snd_proxies_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Switches ( );


	// Private methods
	private:

	// Private attributes
	private:

	::QSnd2::Proxies_Group1_Switch & _snd_proxies;
};


} // End of namespace

#endif
