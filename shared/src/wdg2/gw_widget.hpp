//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_graphical_widget_hpp__
#define __INC_wdg2_graphical_widget_hpp__

#include <QGraphicsItem>

// Forward declaration
namespace Wdg2
{
	class Scene_Database;
}

namespace Wdg2
{


enum GW_State_Flags
{
	GW_VALUE_READ = ( 1 << 0 ),
	GW_HAS_FOCUS  = ( 1 << 1 ),
	GW_IS_GRABBED = ( 1 << 2 ),
	GW_IS_ON      = ( 1 << 3 )
};



/// @brief GW_Widget
///
class GW_Widget :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Widget (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	virtual
	~GW_Widget ( );


	::Wdg2::Scene_Database *
	scene_db ( ) const;


	const QSize &
	size ( ) const;

	virtual
	void
	set_size (
		const QSize & size_n );


	// Bounding rectangle

	QRectF
	boundingRect ( ) const;

	void
	set_bounding_rect (
		const QRectF & rect_n );

	void
	set_bounding_rect (
		const QSizeF & size_n );

	const QRectF
	bounding_rect ( ) const;


	// Paint method

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	// Private attributes
	private:

	QRectF _brect;
	QSize _size;
	::Wdg2::Scene_Database * _scene_db;
};


inline
::Wdg2::Scene_Database *
GW_Widget::scene_db ( ) const
{
	return _scene_db;
}

inline
const QRectF
GW_Widget::bounding_rect ( ) const
{
	return _brect;
}

inline
const QSize &
GW_Widget::size ( ) const
{
	return _size;
}


} // End of namespace

#endif
