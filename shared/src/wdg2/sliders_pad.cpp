//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "sliders_pad.hpp"
#include "gw_groups.hpp"
#include "qsnd2/controls.hpp"
#include <QEvent>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QScrollBar>
#include <QGLWidget>
#include <QCoreApplication>
#include <iostream>


namespace Wdg2
{


Sliders_Pad::Sliders_Pad (
	QWidget * parent_n ) :
QGraphicsView ( parent_n ),
_gw_sliders_pad ( &_scene_db ),
_opengl_enabled ( false )
{
	_etype_deliver_pixmaps = QEvent::registerEventType();
	_scene_db.pxm_server()->set_one_done_callback (
		::Context_Callback ( this, &::Wdg2::Sliders_Pad::notify_pixmaps_finished ) );

	setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );
	setAlignment ( Qt::AlignLeft | Qt::AlignTop );
	setFocusPolicy ( Qt::WheelFocus );
	setFrameStyle ( QFrame::NoFrame );

	setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

	setRenderHints ( QPainter::Antialiasing | QPainter::TextAntialiasing );
	setInteractive ( true );

	setScene ( &_scene );
}

Sliders_Pad::~Sliders_Pad ( )
{
}

void
Sliders_Pad::set_snd_controls (
	::QSnd2::Controls * controls_n )
{
	if ( snd_controls() != 0 ) {
		_scene.removeItem ( &_gw_sliders_pad );
	}

	_gw_sliders_pad.set_snd_controls ( controls_n );

	if ( snd_controls() != 0 ) {
		_scene.addItem ( &_gw_sliders_pad );
		update_geometries();
	}
}

bool
Sliders_Pad::opengl_enabled ( ) const
{
	return _opengl_enabled;
}

void
Sliders_Pad::enable_opengl (
	bool flag_n )
{
	if ( flag_n != _opengl_enabled ) {
		_opengl_enabled = flag_n;
		if ( _opengl_enabled ) {
			setViewport ( new QGLWidget() );
		} else {
			setViewport ( new QWidget() );
		}
	}
}

void
Sliders_Pad::update_geometries ( )
{
	if ( snd_controls() != 0 ) {
		const QSize vps ( maximumViewportSize() );
		_gw_sliders_pad.set_pad_size ( vps );

		QRectF srect ( QPointF ( 0.0, 0.0 ), QSizeF ( vps ) );
		setSceneRect ( srect );
	}
}

void
Sliders_Pad::notify_pixmaps_finished (
	void * context_n )
{
	::Wdg2::Sliders_Pad * cref (
		reinterpret_cast < ::Wdg2::Sliders_Pad * > ( context_n ) );
	QCoreApplication * app ( QCoreApplication::instance() );
	app->postEvent ( cref, new QEvent ( (QEvent::Type)cref->_etype_deliver_pixmaps ) );
}

bool
Sliders_Pad::event (
	QEvent * event_n )
{
	bool res ( false );
	if ( event_n->type() == _etype_deliver_pixmaps ) {
		_scene_db.pxm_server()->deliver_finished_requests();
		res = true;
	} else {
		res = QGraphicsView::event ( event_n );;
	}
	return res;
}

void
Sliders_Pad::resizeEvent (
	QResizeEvent * event_n )
{
	QGraphicsView::resizeEvent ( event_n );
	update_geometries();
}


} // End of namespace
