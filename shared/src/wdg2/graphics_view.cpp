//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "graphics_view.hpp"
#include "gw_widget.hpp"
#include <QEvent>
#include <QGLWidget>
#include <QCoreApplication>
#include <iostream>

namespace Wdg2
{


Graphics_View::Graphics_View (
	::Wdg2::Scene_Database * scene_db_n,
	QWidget * parent_n ) :
QGraphicsView ( parent_n ),
_scene_db ( scene_db_n ),
_opengl_enabled ( false )
{
	_etype_deliver_pixmaps = QEvent::registerEventType();
	_scene_db->pxm_server()->set_one_done_callback (
		::Context_Callback ( this, &::Wdg2::Graphics_View::pixmaps_finished_cb ) );

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

Graphics_View::~Graphics_View ( )
{
}

void
Graphics_View::set_widget (
	::Wdg2::GW_Widget * widget_n )
{
	if ( _widget != 0 ) {
		_scene.removeItem ( _widget.data() );
	}
	_widget.reset ( widget_n );
	if ( _widget != 0 ) {
		_scene.addItem ( _widget.data() );
		update_geometries();
	}
}

bool
Graphics_View::opengl_enabled ( ) const
{
	return _opengl_enabled;
}

void
Graphics_View::enable_opengl (
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
Graphics_View::update_geometries ( )
{
	const QSize vps ( maximumViewportSize() );
	if ( _widget != 0 ) {
		_widget->set_size ( vps );
	}
	QRectF srect ( QPointF ( 0.0, 0.0 ), QSizeF ( vps ) );
	setSceneRect ( srect );
}

void
Graphics_View::pixmaps_finished_cb (
	void * context_n )
{
	::Wdg2::Graphics_View * cref (
		reinterpret_cast < ::Wdg2::Graphics_View * > ( context_n ) );
	QCoreApplication * app ( QCoreApplication::instance() );
	app->postEvent (
		cref, new QEvent ( (QEvent::Type)cref->_etype_deliver_pixmaps ) );
}

bool
Graphics_View::event (
	QEvent * event_n )
{
	bool res ( true );
	if ( event_n->type() == _etype_deliver_pixmaps ) {
		_scene_db->pxm_server()->deliver_finished_requests();
	} else {
		res = QGraphicsView::event ( event_n );;
	}
	return res;
}

void
Graphics_View::resizeEvent (
	QResizeEvent * event_n )
{
	QGraphicsView::resizeEvent ( event_n );
	update_geometries();
}


} // End of namespace
