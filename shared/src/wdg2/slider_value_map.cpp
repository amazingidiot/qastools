//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "slider_value_map.hpp"
#include "int_math.hpp"
#include <cmath>
#include <iostream>

namespace Wdg2
{


Slider_Value_Map::Slider_Value_Map (
	long val_min_n,
	long val_max_n ) :
_val_min ( val_min_n ),
_val_max ( val_max_n ),
_px_span ( 0 )
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
Slider_Value_Map::set_value_range (
	long val_min_n,
	long val_max_n )
{
	if ( ( _val_min != val_min_n ) || ( _val_max != val_max_n ) ) {
		_val_min = val_min_n;
		_val_max = val_max_n;
		update_mapping();
	}
}

void
Slider_Value_Map::update_mapping ( )
{
	_px_map.clear();
	_value_map.clear();

	const unsigned long px_span ( _px_span );
	const unsigned long val_span ( ::Wdg2::integer_distance ( value_min(), value_max() ) );
	if ( ( px_span != 0 ) && ( val_span != 0 ) ) {

		// Pixel to value mapping
		_px_map.insert ( Map_Px::value_type ( 0, value_min() ) );
		if ( px_span >= val_span ) {
			// Average free pixel space between two values
			const unsigned int px_delta_av ( ( px_span - val_span ) / ( val_span ) );
			const unsigned int px_delta_mod ( ( px_span - val_span ) % ( val_span ) );
			unsigned int px_error ( 0 );
			unsigned int px_pos ( 0 );
			long int_value ( value_min() );
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
			long int_value ( value_min() );
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
		_px_map.insert ( Map_Px::value_type ( px_span, value_max() ) );

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
	Map_Px::const_iterator it_geq ( _px_map.lower_bound ( px_n ) );
	if ( it_geq == _px_map.end() ) {
		// highest known value
		res = value_max();
	} else {
		if ( it_geq == _px_map.begin() ) {
			// lowest value
			res = it_geq->second;
		} else {
			// nearest value
			Map_Px::const_iterator it_les ( it_geq );
			--it_les;
			const unsigned int px_delta_l ( px_n - it_les->first );
			const unsigned int px_delta_g ( it_geq->first - px_n );
			if ( px_delta_l <= px_delta_g ) {
				res = it_les->second;
			} else {
				res = it_geq->second;
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
	Map_Value::const_iterator it_geq ( _value_map.lower_bound ( value_n ) );
	if ( it_geq == _value_map.end() ) {
		// highest possible px value
		res = _px_span;
	} else {
		if ( it_geq == _value_map.begin() ) {
			// highest value
			res = it_geq->second;
		} else {
			// nearest value
			Map_Value::const_iterator it_les ( it_geq );
			--it_les;
			const unsigned int px_delta_l ( value_n - it_les->first );
			const unsigned int px_delta_g ( it_geq->first - value_n );
			if ( px_delta_l <= px_delta_g ) {
				res = it_les->second;
			} else {
				res = it_geq->second;
			}
		}
	}
	return res;
}


} // End of namespace
