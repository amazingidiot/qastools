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

namespace Wdg2
{


struct GW_Levels_Sizes
{
	GW_Levels_Sizes ( );

	unsigned int area_height;
	unsigned int slider_width;
	unsigned int channels_hgap;
};

inline
GW_Levels_Sizes::GW_Levels_Sizes ( ) :
area_height ( 0 ),
slider_width ( 0 ),
channels_hgap ( 0 )
{
}


/// @brief GW_Levels
///
class GW_Levels :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Levels (
		::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Levels ( );

	::QSnd2::Proxies_Group1_Slider &
	snd_proxies ( );

	const ::QSnd2::Proxies_Group1_Slider &
	snd_proxies ( ) const;


	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	const ::Wdg2::GW_Levels_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Levels_Sizes & sizes_n );

	unsigned int
	int_width ( ) const;


	// Private methods
	private:

	void
	update_geometries ( );


	// Private attributes
	private:

	::QSnd2::Proxies_Group1_Slider & _snd_proxies;
	::Wdg2::GW_Levels_Sizes _sizes;

	QList < ::Wdg2::GW_Slider * > _sliders;
	::Wdg2::GW_Slider_Handle * _slider_handle;
};


inline
::QSnd2::Proxies_Group1_Slider &
GW_Levels::snd_proxies ( )
{
	return _snd_proxies;
}

inline
const ::QSnd2::Proxies_Group1_Slider &
GW_Levels::snd_proxies ( ) const
{
	return _snd_proxies;
}

inline
const ::Wdg2::GW_Levels_Sizes &
GW_Levels::sizes ( ) const
{
	return _sizes;
}



} // End of namespace

#endif
