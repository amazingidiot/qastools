//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_volume_sliders_joinable_hpp__
#define __INC_wdg2_gw_volume_sliders_joinable_hpp__

#include "wdg2/gw_sliders_joinable.hpp"
#include "qsnd2/controls_proxies.hpp"

namespace Wdg2
{


/// @brief GW_Volume_Sliders_Joinable
///
class GW_Volume_Sliders_Joinable :
	public ::Wdg2::GW_Sliders_Joinable
{
	// Public methods
	public:

	GW_Volume_Sliders_Joinable (
		::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Volume_Sliders_Joinable ( );


	::QSnd2::Proxies_Group1_Slider &
	proxies_grp ( );

	const ::QSnd2::Proxies_Group1_Slider &
	proxies_grp ( ) const;


	// Protected methods
	protected:

	::Wdg2::GW_Slider *
	create_single_slider (
		unsigned int idx_n );

	::Wdg2::GW_Slider_Multi *
	create_multi_slider ( );


	// Private attributes
	private:

	::QSnd2::Proxies_Group1_Slider & _proxies_grp;
};


inline
::QSnd2::Proxies_Group1_Slider &
GW_Volume_Sliders_Joinable::proxies_grp ( )
{
	return _proxies_grp;
}

inline
const ::QSnd2::Proxies_Group1_Slider &
GW_Volume_Sliders_Joinable::proxies_grp ( ) const
{
	return _proxies_grp;
}


} // End of namespace

#endif
