//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_volume_multi_slider_hpp__
#define __INC_wdg2_gw_volume_multi_slider_hpp__

#include "wdg2/gw_multi_slider.hpp"
#include "qsnd2/controls_proxies.hpp"

namespace Wdg2
{


/// @brief GW_Volume_Multi_Slider
///
class GW_Volume_Multi_Slider :
	public ::Wdg2::GW_Multi_Slider
{
	// Public methods
	public:

	GW_Volume_Multi_Slider (
		::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Volume_Multi_Slider ( );


	::QSnd2::Proxies_Group1_Slider &
	proxies_grp ( );

	const ::QSnd2::Proxies_Group1_Slider &
	proxies_grp ( ) const;


	// Private attributes
	private:

	::QSnd2::Proxies_Group1_Slider & _proxies_grp;
};


inline
::QSnd2::Proxies_Group1_Slider &
GW_Volume_Multi_Slider::proxies_grp ( )
{
	return _proxies_grp;
}

inline
const ::QSnd2::Proxies_Group1_Slider &
GW_Volume_Multi_Slider::proxies_grp ( ) const
{
	return _proxies_grp;
}


} // End of namespace

#endif
