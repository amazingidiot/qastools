//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_sliders_joinable_hpp__
#define __INC_wdg2_gw_sliders_joinable_hpp__

#include "wdg2/gw_widget.hpp"
#include "wdg2/slider_value_map.hpp"
#include "flags.hpp"

// Forward declaration
namespace Wdg2
{
	class GW_Slider;
	class GW_Slider_Multi;
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


/// @brief GW_Sliders_Joinable
///
class GW_Sliders_Joinable :
	public ::Wdg2::GW_Widget
{
	// Public typedefs
	public:

	enum State_Flags {
		SF_NULL = 0,
		SF_SEPARATE = ( 1 << 0 ),
		SF_JOINED = ( 1 << 1 )
	};


	// Public methods
	public:

	GW_Sliders_Joinable (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Sliders_Joinable ( );


	unsigned int
	num_sliders ( ) const;

	void
	set_num_sliders (
		unsigned int num_n );


	bool
	is_separate ( ) const;

	bool
	is_joined ( ) const;

	void
	select_separate ( );

	void
	select_joined ( );


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



	::Wdg2::Slider_Value_Map &
	value_map ( );

	const ::Wdg2::Slider_Value_Map &
	value_map ( ) const;


	// Private methods
	private:

	void
	update_geometries ( );

	void
	clear_single_sliders ( );

	void
	clear_multi_slider ( );

	void
	init_single_sliders ( );

	void
	init_multi_slider ( );

	virtual
	::Wdg2::GW_Slider *
	create_single_slider (
		unsigned int idx_n ) = 0;

	virtual
	::Wdg2::GW_Slider_Multi *
	create_multi_slider ( ) = 0;


	// Private attributes
	private:

	::Wdg2::Slider_Value_Map _value_map;
	QList < ::Wdg2::GW_Slider * > _sliders;
	::Wdg2::GW_Slider_Multi * _slider_multi;

	unsigned int _num_sliders;
	::Flags _state_flags;
	::Wdg2::GW_Joinable_Sliders_Sizes _sizes;
};

inline
unsigned int
GW_Sliders_Joinable::num_sliders ( ) const
{
	return _num_sliders;
}

inline
const ::Wdg2::GW_Joinable_Sliders_Sizes &
GW_Sliders_Joinable::sizes ( ) const
{
	return _sizes;
}

inline
bool
GW_Sliders_Joinable::is_separate ( ) const
{
	return _state_flags.has_any ( SF_SEPARATE );
}

inline
bool
GW_Sliders_Joinable::is_joined ( ) const
{
	return _state_flags.has_any ( SF_JOINED );
}

inline
::Wdg2::Slider_Value_Map &
GW_Sliders_Joinable::value_map ( )
{
	return _value_map;
}

inline
const ::Wdg2::Slider_Value_Map &
GW_Sliders_Joinable::value_map ( ) const
{
	return _value_map;
}


} // End of namespace

#endif
