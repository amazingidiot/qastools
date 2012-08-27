//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_multi_slider_hpp__
#define __INC_wdg2_gw_multi_slider_hpp__

#include "wdg2/gw_widget.hpp"
#include "wdg2/gw_slider.hpp"
#include "wdg2/slider_value_map.hpp"

namespace Wdg2
{


struct GW_Multi_Slider_Sizes
{
	GW_Multi_Slider_Sizes ( );

	unsigned int area_height;
	unsigned int slider_width;
	unsigned int channels_hgap;
};

inline
GW_Multi_Slider_Sizes::GW_Multi_Slider_Sizes ( ) :
area_height ( 0 ),
slider_width ( 0 ),
channels_hgap ( 0 )
{
}


/// @brief GW_Multi_Slider
///
class GW_Multi_Slider :
	public ::Wdg2::GW_Slider
{
	// Public methods
	public:

	GW_Multi_Slider (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Multi_Slider ( );


	unsigned int
	num_sliders ( ) const;

	void
	set_num_sliders (
		unsigned int num_n );


	const ::Wdg2::GW_Multi_Slider_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Multi_Slider_Sizes & sizes_n );


	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_Multi_Slider_Sizes & sizes_n ) const;

	unsigned int
	int_width ( ) const;


	// Private methods
	private:

	void
	update_geometries ( );


	// Private attributes
	private:

	unsigned int _num_sliders;
	::Wdg2::GW_Multi_Slider_Sizes _sizes;
};

inline
unsigned int
GW_Multi_Slider::num_sliders ( ) const
{
	return _num_sliders;
}

inline
const ::Wdg2::GW_Multi_Slider_Sizes &
GW_Multi_Slider::sizes ( ) const
{
	return _sizes;
}


} // End of namespace

#endif
