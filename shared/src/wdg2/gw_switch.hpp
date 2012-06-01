//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_switch_hpp__
#define __INC_wdg2_gw_switch_hpp__

#include <QGraphicsItem>
#include "qsnd2/controls_proxies.hpp"


namespace Wdg2
{


/// @brief GW_Switch
///
class GW_Switch :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Switch (
		QGraphicsItem * parent_n = 0 );

	~GW_Switch ( );


	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	const QSize &
	switch_size ( ) const;

	void
	set_switch_size (
		const QSize & size_n );


	// Private methods
	private:

	// Private attributes
	private:

	QSize _switch_size;
};

inline
const QSize &
GW_Switch::switch_size ( ) const
{
	return _switch_size;
}


} // End of namespace

#endif
