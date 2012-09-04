//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_qsnd2_switch.hpp"
#include <iostream>

namespace Wdg2
{


GW_QSnd2_Switch::GW_QSnd2_Switch (
	::QSnd2::Proxy_Switch & switch_proxy_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Switch ( scene_db_n, parent_n ),
_switch_proxy ( switch_proxy_n )
{
	set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_QSnd2_Switch::write_value_to_proxy_cb ) );
	_switch_proxy.set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_QSnd2_Switch::read_value_from_proxy_cb ) );
}

GW_QSnd2_Switch::~GW_QSnd2_Switch ( )
{
	_switch_proxy.set_val_change_callback ( ::Context_Callback() );
}

void
GW_QSnd2_Switch::read_value_from_proxy ( )
{
	set_switch_state ( _switch_proxy.switch_state() );
}

void
GW_QSnd2_Switch::write_value_to_proxy ( )
{
	_switch_proxy.set_switch_state ( switch_state() );
	read_value_from_proxy();
}

void
GW_QSnd2_Switch::read_value_from_proxy_cb (
	void * context_n )
{
	::Wdg2::GW_QSnd2_Switch * gw_switch (
		reinterpret_cast < ::Wdg2::GW_QSnd2_Switch * > ( context_n ) );
	gw_switch->read_value_from_proxy();
}

void
GW_QSnd2_Switch::write_value_to_proxy_cb (
	void * context_n )
{
	::Wdg2::GW_QSnd2_Switch * gw_switch (
		reinterpret_cast < ::Wdg2::GW_QSnd2_Switch * > ( context_n ) );
	gw_switch->write_value_to_proxy();
}


} // End of namespace
