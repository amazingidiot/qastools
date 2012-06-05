//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_scrollbar_hpp__
#define __INC_wdg2_gw_scrollbar_hpp__

#include <QGraphicsItem>

namespace Wdg2
{


/// @brief GW_Scrollbar_Button
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
};



/// @brief GW_Scrollbar_Rail
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
};



/// @brief GW_Scrollbar_Handle
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

	// Private attributes
	private:

	QSize _size;
};



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


	// Protected methods
	protected:

	unsigned int
	handle_pos ( );

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


	// Private attributes
	private:

	QSize _size;
	Qt::Orientation _orientation;
	unsigned int _int_span;

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

} // End of namespace

#endif
