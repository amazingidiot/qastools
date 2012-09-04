//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_switch_multi.hpp"
#include "theme_painters.hpp"
#include <iostream>

namespace Wdg2
{


GW_Switch_Multi::GW_Switch_Multi (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Switch (
	scene_db_n,
	parent_n )
{
}

GW_Switch_Multi::~GW_Switch_Multi ( )
{
}

void
GW_Switch_Multi::load_settings_multi (
	const ::Wdg2::GW_Switch_Multi_Settings & settings_n )
{
	_settings_multi = settings_n;
	set_switch_size ( QSize ( int_width(), _settings_multi.area_height ) );
}

unsigned int
GW_Switch_Multi::int_width_probe (
	const ::Wdg2::GW_Switch_Multi_Settings & settings_n ) const
{
	unsigned int iwidth ( 0 );
	if ( settings_n.num_switches > 0 ) {
		iwidth += settings_n.switch_width * settings_n.num_switches;
		iwidth += settings_n.channels_gap * ( settings_n.num_switches - 1 );
	}
	return iwidth;
}

unsigned int
GW_Switch_Multi::int_width ( ) const
{
	return int_width_probe ( settings_multi() );
}


} // End of namespace
