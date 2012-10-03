//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_qsnd2_switches_joinable.hpp"
#include "gw_qsnd2_switch.hpp"
#include "gw_qsnd2_switch_multi.hpp"
#include <iostream>

namespace Wdg2
{


GW_QSnd2_Switches_Joinable::GW_QSnd2_Switches_Joinable (
	::QSnd2::Proxies_Group_Switches & proxies_grp_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Switches_Joinable ( scene_db_n, parent_n ),
_proxies_grp ( proxies_grp_n )
{
	set_num_switches ( proxies_grp().num_children() );
}

GW_QSnd2_Switches_Joinable::~GW_QSnd2_Switches_Joinable ( )
{
}

::Wdg2::GW_Switch *
GW_QSnd2_Switches_Joinable::create_switch_single (
	unsigned int idx_n )
{
	::Wdg2::GW_QSnd2_Switch * gw_switch;
	{
		::QSnd2::Proxy_Switch & sproxy ( *proxies_grp().switch_proxy ( idx_n ) );
		gw_switch = new ::Wdg2::GW_QSnd2_Switch ( sproxy, scene_db() );
	}
	gw_switch->read_value_from_proxy();
	return gw_switch;
}

::Wdg2::GW_Switch_Multi *
GW_QSnd2_Switches_Joinable::create_switch_multi ( )
{
	::Wdg2::GW_QSnd2_Switch_Multi * gw_switch (
		new ::Wdg2::GW_QSnd2_Switch_Multi ( proxies_grp(), scene_db() ) );
	gw_switch->read_value_from_proxy();
	return gw_switch;
}


} // End of namespace
