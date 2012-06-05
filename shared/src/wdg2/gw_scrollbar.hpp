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


	// Private attributes
	private:

	QSize _size;
};

inline
const QSize &
GW_Scrollbar::size ( ) const
{
	return _size;
}


} // End of namespace

#endif
