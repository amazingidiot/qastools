//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "slider_value_map.hpp"
#include "int_math.hpp"
#include <cmath>
#include <iostream>

namespace Wdg2
{


Slider_Value_Map::Slider_Value_Map (
	::QSnd2::Proxies_Group1_Slider & proxies_grp_n ) :
_proxies_grp ( proxies_grp_n )
{
	update_mapping();
}

Slider_Value_Map::~Slider_Value_Map ( )
{
}

void
Slider_Value_Map::set_px_span (
	unsigned int num_px_n )
{
	if ( _px_span != num_px_n ) {
		_px_span = num_px_n;
		update_mapping();
	}
}

void
Slider_Value_Map::update_mapping ( )
{
	_px_map.clear();
	_value_map.clear();

	::QSnd2::Integer_Pair vrange;
	_proxies_grp.int_range ( vrange );
	const unsigned long px_span ( _px_span );
	const unsigned long val_span ( ::Wdg2::integer_distance ( vrange[0], vrange[1] ) );
	if ( ( px_span != 0 ) && ( val_span != 0 ) ) {

		// Pixel to value mapping
		_px_map.insert ( Map_Px::value_type ( 0, vrange[0] ) );
		if ( px_span >= val_span ) {
			// Average free pixel space between two values
			const unsigned int px_delta_av ( ( px_span - val_span ) / ( val_span ) );
			const unsigned int px_delta_mod ( ( px_span - val_span ) % ( val_span ) );
			unsigned int px_error ( 0 );
			unsigned int px_pos ( 0 );
			long int_value ( vrange[0] );
			for ( unsigned long ii=0; ii < (val_span-1); ++ii ) {
				{
					unsigned int px_delta ( px_delta_av );
					px_error += px_delta_mod;
					if ( px_error >= val_span ) {
						px_error -= val_span;
						++px_delta;
					}
					px_pos += px_delta;
					++px_pos;
				}
				++int_value;
				_px_map.insert ( Map_Px::value_type ( px_pos, int_value ) );
			}
		} else {
			const unsigned long val_delta_av ( ( val_span - px_span ) / ( px_span ) );
			const unsigned long val_delta_mod ( ( val_span - px_span ) % ( px_span ) );
			unsigned long val_error ( 0 );
			unsigned int px_pos ( 0 );
			long int_value ( vrange[0] );
			for ( unsigned long ii=0; ii < (px_span-1); ++ii ) {
				{
					unsigned long val_delta ( val_delta_av );
					val_error += val_delta_mod;
					if ( val_error >= px_span ) {
						val_error -= px_span;
						++val_delta;
					}
					int_value += val_delta;
					++int_value;
				}
				++px_pos;
				_px_map.insert ( Map_Px::value_type ( px_pos, int_value ) );
			}
			// TODO
		}
		_px_map.insert ( Map_Px::value_type ( px_span, vrange[1] ) );

		// Value to pixel mapping
		Map_Px::const_iterator it_end ( _px_map.end() );
		Map_Px::const_iterator it ( _px_map.begin() );
		for ( ; it != it_end; ++it ) {
			_value_map.insert ( Map_Value::value_type ( it->second, it->first ) );
		}
	}
}

long
Slider_Value_Map::value_from_px (
	unsigned int px_n ) const
{
	long res;
	Map_Px::const_iterator it_le ( _px_map.lower_bound ( px_n ) );
	Map_Px::const_iterator it_end ( _px_map.end() );
	if ( it_le == it_end ) {
		// highest known value
		::QSnd2::Integer_Pair vrange;
		_proxies_grp.int_range ( vrange );
		res = vrange[1];
	} else {
		Map_Px::const_iterator it_gr ( it_le );
		++it_gr;
		if ( it_gr == it_end ) {
			// highest value
			res = it_le->second;
		} else {
			// nearest value
			const unsigned int px_delta_l ( px_n - it_le->first );
			const unsigned int px_delta_g ( it_gr->first - px_n );
			if ( px_delta_l <= px_delta_g ) {
				res = it_le->second;
			} else {
				res = it_gr->second;
			}
		}
	}
	return res;
}

unsigned int
Slider_Value_Map::px_from_value (
	long value_n ) const
{
	long res;
	Map_Value::const_iterator it_le ( _value_map.lower_bound ( value_n ) );
	Map_Value::const_iterator it_end ( _value_map.end() );
	if ( it_le == it_end ) {
		// highest possible px value
		res = _px_span;
	} else {
		Map_Value::const_iterator it_gr ( it_le );
		++it_gr;
		if ( it_gr == it_end ) {
			// highest value
			res = it_le->second;
		} else {
			// nearest value
			const unsigned int px_delta_l ( value_n - it_le->first );
			const unsigned int px_delta_g ( it_gr->first - value_n );
			if ( px_delta_l <= px_delta_g ) {
				res = it_le->second;
			} else {
				res = it_gr->second;
			}
		}
	}
	return res;
}


} // End of namespace
