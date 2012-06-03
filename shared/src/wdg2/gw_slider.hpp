//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_slider_hpp__
#define __INC_wdg2_gw_slider_hpp__

#include "flags.hpp"
#include "graphical_widget.hpp"
#include "slider_value_map.hpp"
#include "qsnd2/controls_proxies.hpp"
#include <QGraphicsItem>


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


	const ::Wdg2::Flags &
	state_flags ( ) const;

	::Wdg2::Flags &
	state_flags ( );


	// Private attributes
	private:

	QSize _rail_size;
	::Wdg2::Flags _state_flags;
};

inline
const QSize &
GW_Slider_Rail::rail_size ( ) const
{
	return _rail_size;
}

inline
const ::Wdg2::Flags &
GW_Slider_Rail::state_flags ( ) const
{
	return _state_flags;
}

inline
::Wdg2::Flags &
GW_Slider_Rail::state_flags ( )
{
	return _state_flags;
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


	const ::Wdg2::Flags &
	state_flags ( ) const;

	::Wdg2::Flags &
	state_flags ( );


	// Private attributes
	private:

	QSize _handle_size;
	::Wdg2::Flags _state_flags;
};

inline
const QSize &
GW_Slider_Handle::handle_size ( ) const
{
	return _handle_size;
}

inline
const ::Wdg2::Flags &
GW_Slider_Handle::state_flags ( ) const
{
	return _state_flags;
}

inline
::Wdg2::Flags &
GW_Slider_Handle::state_flags ( )
{
	return _state_flags;
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

	~GW_Slider ( );


	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	const ::Wdg2::Slider_Value_Map &
	value_map ( ) const;


	const ::Wdg2::GW_Slider_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Slider_Sizes & sizes_n );

	void
	read_proxy_value ( );

	/// @brief Callback version
	static
	void
	read_proxy_value_cb (
		void * context_n );


	// Protected methods
	protected:

	/// @brief measured in pixels from the top (upper end) of the slider
	///
	unsigned int
	handle_pos ( );

	/// @brief measured in pixels from the bottom (lower end) of the slider
	///
	unsigned int
	handle_px_pos ( );

	void
	set_handle_px_pos (
		int px_pos_n );

	unsigned int
	px_pos_from_handle_pos (
		int slider_pos_n ) const;

	unsigned int
	handle_pos_from_px_pos (
		int px_pos_n ) const;

	bool
	point_in_handle (
		const QPointF & point_n ) const;

	void
	focusInEvent (
		QFocusEvent * event_n );

	void
	focusOutEvent (
		QFocusEvent * event_n );

	void
	mousePressEvent (
		QGraphicsSceneMouseEvent * event_n );

	void
	mouseReleaseEvent (
		QGraphicsSceneMouseEvent * event_n );

	void
	mouseMoveEvent (
		QGraphicsSceneMouseEvent * event_n );


	// Private attributes
	private:

	::QSnd2::Proxy_Slider & _slider_proxy;
	::Wdg2::GW_Slider_Sizes _sizes;
	/// @brief handle_px_pos() must be <= this
	unsigned int _px_span;

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
