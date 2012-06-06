//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_scrollbar_hpp__
#define __INC_wdg2_gw_scrollbar_hpp__

#include "flags.hpp"
#include "callbacks.hpp"
#include "graphical_widget.hpp"
#include "slider_value_map.hpp"
#include <QGraphicsItem>

namespace Wdg2
{


/// @brief Button for the GW_Scrollbar
///
class GW_Scrollbar_Button :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Scrollbar_Button (
		QGraphicsItem * parent_n = 0 );


	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	void
	set_size (
		const QSize & size_n );

	Qt::Orientation
	orientation ( ) const;


	// Private attributes
	private:

	QSize _size;
	QRectF _brect;
};



/// @brief Rail for the GW_Scrollbar
///
class GW_Scrollbar_Rail :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Scrollbar_Rail (
		QGraphicsItem * parent_n = 0 );


	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );

	void
	set_size (
		const QSize & size_n );

	Qt::Orientation
	orientation ( ) const;


	// Private attributes
	private:

	QSize _size;
	QRectF _brect;
};



/// @brief Handle for the GW_Scrollbar
///
class GW_Scrollbar_Handle :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Scrollbar_Handle (
		QGraphicsItem * parent_n = 0 );

	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );

	void
	set_size (
		const QSize & size_n );

	Qt::Orientation
	orientation ( ) const;

	::Flags &
	state_flags ( );

	const ::Flags &
	state_flags ( ) const;


	// Private attributes
	private:

	QSize _size;
	QRectF _brect;
	::Flags _state_flags;
};

inline
::Flags &
GW_Scrollbar_Handle::state_flags ( )
{
	return _state_flags;
}

inline
const ::Flags &
GW_Scrollbar_Handle::state_flags ( ) const
{
	return _state_flags;
}


/// @brief GW_Scrollbar
///
class GW_Scrollbar :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Scrollbar (
		QGraphicsItem * parent_n = 0 );

	~GW_Scrollbar ( );


	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	const QSize &
	size ( ) const;

	void
	set_size (
		const QSize & size_n );


	Qt::Orientation
	orientation ( ) const;

	void
	set_orientation (
		Qt::Orientation orientation_n );


	unsigned int
	int_span ( ) const;

	void
	set_int_span (
		unsigned int span_n );


	unsigned int
	int_value ( ) const;

	void
	set_int_value (
		unsigned int value_n );


	const ::Context_Callback &
	val_change_callback ( ) const;

	void
	set_val_change_callback (
		const ::Context_Callback & cb_n );


	// Protected methods
	protected:

	void
	mousePressEvent (
		QGraphicsSceneMouseEvent * event_n );

	void
	mouseReleaseEvent (
		QGraphicsSceneMouseEvent * event_n );

	void
	mouseMoveEvent (
		QGraphicsSceneMouseEvent * event_n );

	void
	wheelEvent (
		QGraphicsSceneWheelEvent * event_n );


	// Private methods
	private:

	void
	update_geometries ( );

	void
	set_handle_pos (
		unsigned int pos_n );

	void
	update_handle_pos_from_value ( );

	void
	update_value_from_handle_pos ( );

	bool
	point_in_handle (
		const QPointF & point_n ) const;

	void
	move_handle (
		int amount_n );


	// Private attributes
	private:

	QSize _size;
	QRectF _brect;
	Qt::Orientation _orientation;

	::Wdg2::Slider_Value_Map _value_map;
	unsigned int _int_span;
	unsigned int _int_value;

	unsigned int _rail_start;
	unsigned int _handle_pos;
	unsigned int _handle_pos_span;
	unsigned int _handle_len;

	::Context_Callback _val_change_cb;

	::Wdg2::GW_Scrollbar_Button _btn_low;
	::Wdg2::GW_Scrollbar_Button _btn_high;
	::Wdg2::GW_Scrollbar_Rail _rail;
	::Wdg2::GW_Scrollbar_Handle _handle;
};

inline
Qt::Orientation
GW_Scrollbar::orientation ( ) const
{
	return _orientation;
}

inline
const QSize &
GW_Scrollbar::size ( ) const
{
	return _size;
}

inline
unsigned int
GW_Scrollbar::int_span ( ) const
{
	return _int_span;
}

inline
unsigned int
GW_Scrollbar::int_value ( ) const
{
	return _int_value;
}

inline
const ::Context_Callback &
GW_Scrollbar::val_change_callback ( ) const
{
	return _val_change_cb;
}


} // End of namespace

#endif
