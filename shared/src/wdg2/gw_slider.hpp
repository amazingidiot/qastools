//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_slider_hpp__
#define __INC_wdg2_gw_slider_hpp__

#include <QGraphicsItem>
#include "qsnd2/controls_proxies.hpp"


namespace Wdg2
{

class GW_Multi_Slider;

/// @brief GW_Slider_Rail
///
class GW_Slider_Rail :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Slider_Rail (
		QGraphicsItem * parent_n = 0 );

	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );

	const QSize &
	rail_size ( ) const;

	void
	set_rail_size (
		const QSize & size_n );


	// Private attributes
	private:

	QSize _rail_size;
};

inline
const QSize &
GW_Slider_Rail::rail_size ( ) const
{
	return _rail_size;
}


/// @brief GW_Slider_Handle
///
class GW_Slider_Handle :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Slider_Handle (
		QGraphicsItem * parent_n = 0 );

	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );

	const QSize &
	handle_size ( ) const;

	void
	set_handle_size (
		const QSize & size_n );

	// Private attributes
	private:

	QSize _handle_size;
};

inline
const QSize &
GW_Slider_Handle::handle_size ( ) const
{
	return _handle_size;
}


struct GW_Slider_Sizes
{
	GW_Slider_Sizes ( );

	QSize size;
	QSize handle_size;
};

inline
GW_Slider_Sizes::GW_Slider_Sizes ( ) :
size ( 0, 0 ),
handle_size ( 0, 0 )
{
}


/// @brief GW_Slider
///
class GW_Slider :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Slider (
		::QSnd2::Proxy_Slider & slider_proxy_n,
		QGraphicsItem * parent_n = 0 );


	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	::Wdg2::GW_Multi_Slider *
	levels ( ) const;


	const ::Wdg2::GW_Slider_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Slider_Sizes & sizes_n );


	// Private attributes
	private:

	::QSnd2::Proxy_Slider & _slider_proxy;
	::Wdg2::GW_Slider_Sizes _sizes;

	::Wdg2::GW_Slider_Rail _rail;
	::Wdg2::GW_Slider_Handle _handle;
};

inline
const ::Wdg2::GW_Slider_Sizes &
GW_Slider::sizes ( ) const
{
	return _sizes;
}


} // End of namespace

#endif
