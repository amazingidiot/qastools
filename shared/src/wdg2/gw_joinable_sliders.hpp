//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_joinable_sliders_hpp__
#define __INC_wdg2_gw_joinable_sliders_hpp__

#include "wdg2/gw_widget.hpp"
#include "wdg2/slider_value_map.hpp"

// Forward declaration
namespace Wdg2
{
	class GW_Slider;
	class GW_Multi_Slider;
}

namespace Wdg2
{


struct GW_Joinable_Sliders_Sizes
{
	GW_Joinable_Sliders_Sizes ( );

	unsigned int area_height;
	unsigned int slider_width;
	unsigned int channels_hgap;
};

inline
GW_Joinable_Sliders_Sizes::GW_Joinable_Sliders_Sizes ( ) :
area_height ( 0 ),
slider_width ( 0 ),
channels_hgap ( 0 )
{
}


/// @brief GW_Joinable_Sliders
///
class GW_Joinable_Sliders :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Joinable_Sliders (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Joinable_Sliders ( );


	void
	add_slider (
		::Wdg2::GW_Slider * slider_n );

	unsigned int
	num_sliders ( ) const;


	const ::Wdg2::GW_Joinable_Sliders_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Joinable_Sliders_Sizes & sizes_n );

	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_Joinable_Sliders_Sizes & sizes_n ) const;

	unsigned int
	int_width ( ) const;


	bool
	is_joined ( ) const;

	void
	set_is_joined (
		bool flag_n );


	::Wdg2::Slider_Value_Map &
	value_map ( );

	const ::Wdg2::Slider_Value_Map &
	value_map ( ) const;


	// Private methods
	private:

	void
	clear_sliders_single ( );

	void
	clear_slider_joined ( );

	void
	create_slider_joined ( );

	void
	update_geometries ( );


	// Private attributes
	private:

	::Wdg2::Slider_Value_Map _value_map;
	QList < ::Wdg2::GW_Slider * > _sliders;
	::Wdg2::GW_Multi_Slider * _multi_slider;

	::Wdg2::GW_Joinable_Sliders_Sizes _sizes;
};

inline
unsigned int
GW_Joinable_Sliders::num_sliders ( ) const
{
	return _sliders.size();
}

inline
const ::Wdg2::GW_Joinable_Sliders_Sizes &
GW_Joinable_Sliders::sizes ( ) const
{
	return _sizes;
}

inline
bool
GW_Joinable_Sliders::is_joined ( ) const
{
	return ( _multi_slider != 0 );
}

inline
::Wdg2::Slider_Value_Map &
GW_Joinable_Sliders::value_map ( )
{
	return _value_map;
}

inline
const ::Wdg2::Slider_Value_Map &
GW_Joinable_Sliders::value_map ( ) const
{
	return _value_map;
}


} // End of namespace

#endif
