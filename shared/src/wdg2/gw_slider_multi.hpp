//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_slider_multi_hpp__
#define __INC_wdg2_gw_slider_multi_hpp__

#include "wdg2/gw_widget.hpp"
#include "wdg2/gw_slider.hpp"
#include "wdg2/slider_value_map.hpp"

namespace Wdg2
{


struct GW_Slider_Multi_Settings
{
	GW_Slider_Multi_Settings ( );

	unsigned int num_sliders;
	unsigned int area_height;
	unsigned int slider_width;
	unsigned int channels_gap;
};

inline
GW_Slider_Multi_Settings::GW_Slider_Multi_Settings ( ) :
num_sliders ( 0 ),
area_height ( 0 ),
slider_width ( 0 ),
channels_gap ( 0 )
{
}


/// @brief GW_Slider_Multi
///
class GW_Slider_Multi :
	public ::Wdg2::GW_Slider
{
	// Public methods
	public:

	GW_Slider_Multi (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Slider_Multi ( );


	const ::Wdg2::GW_Slider_Multi_Settings &
	settings_multi ( ) const;

	void
	load_settings_multi (
		const ::Wdg2::GW_Slider_Multi_Settings & settings_n );


	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_Slider_Multi_Settings & settings_n ) const;

	unsigned int
	int_width ( ) const;


	// Private attributes
	private:

	::Wdg2::GW_Slider_Multi_Settings _settings_multi;
};

inline
const ::Wdg2::GW_Slider_Multi_Settings &
GW_Slider_Multi::settings_multi ( ) const
{
	return _settings_multi;
}


/// @brief GW_Slider_Multi_Rail
///
class GW_Slider_Multi_Rail :
	public ::Wdg2::GW_Widget_Element
{
	// Public methods
	public:

	GW_Slider_Multi_Rail (
		::Wdg2::Scene_Database * scene_db_n,
		::Wdg2::GW_Slider_Multi * parent_n = 0 );

	~GW_Slider_Multi_Rail ( );

	::Wdg2::GW_Widget *
	gw_widget ( );

	::Wdg2::GW_Slider_Multi *
	slider_multi ( );


	// Protected methods
	protected:

	void
	size_changed ( );

	void
	pos_changed ( );

	void
	state_flags_changed ( );


	// Private methods
	private:

	void
	clear_rails ( );

	void
	reload_settings ( );


	// Private attributes
	private:

	::Wdg2::GW_Widget _wdg;
	QList < ::Wdg2::GW_Pixmaps * > _rails;
};

inline
::Wdg2::GW_Widget *
GW_Slider_Multi_Rail::gw_widget ( )
{
	return &_wdg;
}

inline
::Wdg2::GW_Slider_Multi *
GW_Slider_Multi_Rail::slider_multi ( )
{
	return static_cast < ::Wdg2::GW_Slider_Multi * > ( _wdg.parentItem() );
}


/// @brief GW_Slider_Multi_Handle
///
class GW_Slider_Multi_Handle :
	public ::Wdg2::GW_Slider_Handle
{
	// Public methods
	public:

	GW_Slider_Multi_Handle (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	::Wdg2::GW_Slider_Multi *
	slider_multi ( );
};

inline
::Wdg2::GW_Slider_Multi *
GW_Slider_Multi_Handle::slider_multi ( )
{
	return static_cast < ::Wdg2::GW_Slider_Multi * > ( gw_widget()->parentItem() );
}


} // End of namespace

#endif
