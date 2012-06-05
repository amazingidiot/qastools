//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_levels_hpp__
#define __INC_wdg2_gw_levels_hpp__

#include <QGraphicsItem>
#include "qsnd2/controls_proxies.hpp"
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
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Multi_Slider (
		::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Multi_Slider ( );

	::QSnd2::Proxies_Group1_Slider &
	proxies_grp ( );

	const ::QSnd2::Proxies_Group1_Slider &
	proxies_grp ( ) const;


	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	const ::Wdg2::GW_Multi_Slider_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Multi_Slider_Sizes & sizes_n );

	unsigned int
	int_width ( ) const;

	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_Multi_Slider_Sizes & sizes_n ) const;

	const ::Wdg2::Slider_Value_Map &
	value_map ( ) const;


	// Private methods
	private:

	void
	update_geometries ( );


	// Private attributes
	private:

	::Wdg2::GW_Multi_Slider_Sizes _sizes;

	::Wdg2::Slider_Value_Map _value_map;
	QList < ::Wdg2::GW_Slider * > _sliders;
	::Wdg2::GW_Slider_Handle * _slider_handle;
};


inline
::QSnd2::Proxies_Group1_Slider &
GW_Multi_Slider::proxies_grp ( )
{
	return _value_map.proxies_grp();
}

inline
const ::QSnd2::Proxies_Group1_Slider &
GW_Multi_Slider::proxies_grp ( ) const
{
	return _value_map.proxies_grp();
}

inline
const ::Wdg2::GW_Multi_Slider_Sizes &
GW_Multi_Slider::sizes ( ) const
{
	return _sizes;
}

inline
const ::Wdg2::Slider_Value_Map &
GW_Multi_Slider::value_map ( ) const
{
	return _value_map;
}


} // End of namespace

#endif
