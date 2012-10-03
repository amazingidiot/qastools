//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_qsnd2_switch_multi.hpp"
#include <iostream>

namespace Wdg2
{


GW_QSnd2_Switch_Multi::GW_QSnd2_Switch_Multi (
	::QSnd2::Proxies_Group_Switches & proxies_group_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Switch_Multi ( scene_db_n, parent_n ),
_proxies_group ( proxies_group_n )
{
	set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_QSnd2_Switch_Multi::write_value_to_proxy_cb ) );
	_proxies_group.set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_QSnd2_Switch_Multi::read_value_from_proxy_cb ) );
}

GW_QSnd2_Switch_Multi::~GW_QSnd2_Switch_Multi ( )
{
	_proxies_group.clear_val_change_callback();
}

void
GW_QSnd2_Switch_Multi::read_value_from_proxy ( )
{
	set_switch_state ( _proxies_group.switches_state_joined() );
}

void
GW_QSnd2_Switch_Multi::write_value_to_proxy ( )
{
	_proxies_group.set_switches_state_joined ( switch_state() );
	read_value_from_proxy();
}

void
GW_QSnd2_Switch_Multi::read_value_from_proxy_cb (
	void * context_n )
{
	::Wdg2::GW_QSnd2_Switch_Multi * gw_switch (
		reinterpret_cast < ::Wdg2::GW_QSnd2_Switch_Multi * > ( context_n ) );
	gw_switch->read_value_from_proxy();
}

void
GW_QSnd2_Switch_Multi::write_value_to_proxy_cb (
	void * context_n )
{
	::Wdg2::GW_QSnd2_Switch_Multi * gw_switch (
		reinterpret_cast < ::Wdg2::GW_QSnd2_Switch_Multi * > ( context_n ) );
	gw_switch->write_value_to_proxy();
}



} // End of namespace
