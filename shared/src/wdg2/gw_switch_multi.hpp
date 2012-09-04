//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_switch_multi_hpp__
#define __INC_wdg2_gw_switch_multi_hpp__

#include "wdg2/gw_widget.hpp"
#include "wdg2/gw_switch.hpp"

namespace Wdg2
{


struct GW_Switch_Multi_Settings
{
	GW_Switch_Multi_Settings ( );

	unsigned int num_switches;
	unsigned int area_height;
	unsigned int switch_width;
	unsigned int channels_gap;
};

inline
GW_Switch_Multi_Settings::GW_Switch_Multi_Settings ( ) :
num_switches ( 0 ),
area_height ( 0 ),
switch_width ( 0 ),
channels_gap ( 0 )
{
}


/// @brief GW_Switch_Multi
///
class GW_Switch_Multi :
	public ::Wdg2::GW_Switch
{
	// Public methods
	public:

	GW_Switch_Multi (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Switch_Multi ( );


	const ::Wdg2::GW_Switch_Multi_Settings &
	settings_multi ( ) const;

	void
	load_settings_multi (
		const ::Wdg2::GW_Switch_Multi_Settings & settings_n );


	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_Switch_Multi_Settings & settings_n ) const;

	unsigned int
	int_width ( ) const;


	// Private attributes
	private:

	::Wdg2::GW_Switch_Multi_Settings _settings_multi;
};

inline
const ::Wdg2::GW_Switch_Multi_Settings &
GW_Switch_Multi::settings_multi ( ) const
{
	return _settings_multi;
}


} // End of namespace

#endif
