//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_switch_hpp__
#define __INC_wdg2_gw_switch_hpp__

#include "flags.hpp"
#include "graphical_widget.hpp"
#include "qsnd2/controls_proxies.hpp"
#include <QGraphicsItem>


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
		::QSnd2::Proxy_Switch & switch_proxy_n,
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


	void
	read_proxy_value ( );

	/// @brief Callback version
	static
	void
	read_proxy_value_cb (
		void * context_n );


	// Protected methods
	protected:

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


	// Private attributes
	private:

	enum State_Flags {
		SF_ON   = ( 1 << 0 ),
		SF_DOWN = ( 1 << 1 )
	};

	::QSnd2::Proxy_Switch & _switch_proxy;
	QSize _switch_size;
	QRectF _brect;

	::Flags _widget_flags;
	::Flags _state_flags;
};


inline
const QSize &
GW_Switch::switch_size ( ) const
{
	return _switch_size;
}


} // End of namespace

#endif
