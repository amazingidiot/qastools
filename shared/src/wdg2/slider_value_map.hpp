//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_levels_hpp__
#define __INC_wdg2_gw_levels_hpp__

#include "qsnd2/controls_proxies.hpp"
#include <map>

namespace Wdg2
{


/// @brief Slider_Value_Map
///
/// px distances are measure from the bottom of a slider
/// rising upwards.
///
class Slider_Value_Map
{
	// Public methods
	public:

	Slider_Value_Map (
		::QSnd2::Proxies_Group1_Slider & proxies_grp_n );

	~Slider_Value_Map ( );

	unsigned int
	px_span ( ) const;

	void
	set_px_span (
		unsigned int num_px_n );

	long
	value_from_px (
		unsigned int px_n ) const;

	unsigned int
	px_from_value (
		long value_n ) const;


	// Private methods
	private:

	void
	update_mapping ( );


	// Private attributes
	private:

	typedef ::std::map < unsigned int, long > Map_Px;
	typedef ::std::map < long, unsigned int > Map_Value;
	Map_Px _px_map;
	Map_Value _value_map;

	::QSnd2::Proxies_Group1_Slider & _proxies_grp;
	unsigned int _px_span;
};

inline
unsigned int
Slider_Value_Map::px_span ( ) const
{
	return _px_span;
}


} // End of namespace

#endif
