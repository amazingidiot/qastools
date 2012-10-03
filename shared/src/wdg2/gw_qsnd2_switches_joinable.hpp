//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_qsnd2_switches_joinable_hpp__
#define __INC_wdg2_gw_qsnd2_switches_joinable_hpp__

#include "wdg2/gw_switches_joinable.hpp"
#include "qsnd2/controls_proxies.hpp"

namespace Wdg2
{


/// @brief GW_QSnd2_Switches_Joinable
///
class GW_QSnd2_Switches_Joinable :
	public ::Wdg2::GW_Switches_Joinable
{
	// Public methods
	public:

	GW_QSnd2_Switches_Joinable (
		::QSnd2::Proxies_Group_Switches & proxies_grp_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_QSnd2_Switches_Joinable ( );


	::QSnd2::Proxies_Group_Switches &
	proxies_grp ( );

	const ::QSnd2::Proxies_Group_Switches &
	proxies_grp ( ) const;


	// Protected methods
	protected:

	::Wdg2::GW_Switch *
	create_switch_single (
		unsigned int idx_n );

	::Wdg2::GW_Switch_Multi *
	create_switch_multi ( );


	// Private attributes
	private:

	::QSnd2::Proxies_Group_Switches & _proxies_grp;
};


inline
::QSnd2::Proxies_Group_Switches &
GW_QSnd2_Switches_Joinable::proxies_grp ( )
{
	return _proxies_grp;
}

inline
const ::QSnd2::Proxies_Group_Switches &
GW_QSnd2_Switches_Joinable::proxies_grp ( ) const
{
	return _proxies_grp;
}


} // End of namespace

#endif
