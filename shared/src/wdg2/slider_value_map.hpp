//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_slider_value_map_hpp__
#define __INC_wdg2_slider_value_map_hpp__

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
		long val_min_n = 0,
		long val_max_n = 0 );


	long
	value_min ( ) const;

	long
	value_max ( ) const;

	void
	set_value_range (
		long val_min_n,
		long val_max_n );


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

	long _val_min;
	long _val_max;
	unsigned int _px_span;
};

inline
unsigned int
Slider_Value_Map::px_span ( ) const
{
	return _px_span;
}

inline
long
Slider_Value_Map::value_min ( ) const
{
	return _val_min;
}

inline
long
Slider_Value_Map::value_max ( ) const
{
	return _val_max;
}


} // End of namespace

#endif
