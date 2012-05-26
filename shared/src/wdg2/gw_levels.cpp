//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_levels.hpp"
#include <iostream>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


namespace Wdg2
{


GW_Slider_Rail::GW_Slider_Rail (
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_rail_size ( 0.0, 0.0 )
{
}

QRectF
GW_Slider_Rail::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), QSizeF ( _rail_size ) );
}

void
GW_Slider_Rail::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;

	double pen_width ( 1.0 );
	double pwhalf ( pen_width / 2.0 );
	QRectF prect ( QPointF ( 0.0, 0.0 ), _rail_size );
	prect.adjust ( pwhalf, pwhalf, -pwhalf, -pwhalf );

	{
		QPen ppen ( option_n->palette.color ( QPalette::ButtonText ) );
		ppen.setWidth ( pen_width );
		painter_n->setPen ( ppen );
	}
	painter_n->setBrush ( option_n->palette.color ( QPalette::Button ) );
	painter_n->drawRoundedRect ( prect, 2.0, 2.0 );
}

void
GW_Slider_Rail::set_rail_size (
	const QSize & size_n )
{
	prepareGeometryChange();
	_rail_size = size_n;
}



GW_Slider_Handle::GW_Slider_Handle (
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_handle_size ( 0.0, 0.0 )
{
}

QRectF
GW_Slider_Handle::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), QSizeF ( _handle_size ) );
}

void
GW_Slider_Handle::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;

	double pen_width ( 1.0 );
	double pwhalf ( pen_width / 2.0 );
	QRectF prect ( QPointF ( 0.0, 0.0 ), _handle_size );
	prect.adjust ( pwhalf, pwhalf, -pwhalf, -pwhalf );

	{
		QPen ppen ( option_n->palette.color ( QPalette::ButtonText ) );
		ppen.setWidth ( pen_width );
		painter_n->setPen ( ppen );
	}
	painter_n->setBrush ( option_n->palette.color ( QPalette::Button ) );
	painter_n->drawRoundedRect ( prect, 2.0, 2.0 );
}

void
GW_Slider_Handle::set_handle_size (
	const QSize & size_n )
{
	prepareGeometryChange();
	_handle_size = size_n;
}



GW_Slider::GW_Slider (
	::QSnd2::Proxy_Slider & slider_proxy_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_slider_proxy ( slider_proxy_n ),
_rail ( this ),
_handle ( this )
{
	_rail.setPos ( QPointF ( 0.0, 0.0 ) );
	_handle.setPos ( QPointF ( 0.0, 0.0 ) );
}

QRectF
GW_Slider::boundingRect ( ) const
{
	return QRectF ( 0, 0, 0, 0 );
}

void
GW_Slider::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;
}

void
GW_Slider::set_size (
	const QSize & size_n )
{
	_size = size_n;
	_rail.set_rail_size ( _size );

	{
		QSize hsize ( _size );
		hsize.rheight() /= 10;
		_handle.set_handle_size ( hsize );
	}
}



GW_Levels::GW_Levels (
	::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_snd_proxies ( snd_proxies_n )
{
	for ( unsigned int ii=0; ii < _snd_proxies.num_proxies(); ++ii ) {
		_sliders.append (
			new ::Wdg2::GW_Slider ( *_snd_proxies.slider_proxy ( ii ), this ) );
	}
	_slider_handle = new ::Wdg2::GW_Slider_Handle ( this );
	_slider_handle->setVisible ( false );
}

GW_Levels::~GW_Levels ( )
{
}

QRectF
GW_Levels::boundingRect ( ) const
{
	return QRectF ( 0, 0, 0, 0 );
}

void
GW_Levels::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;
}

void
GW_Levels::set_sizes (
	const ::Wdg2::GW_Levels_Sizes & sizes_n )
{
	_sizes = sizes_n;
	{
		QSize rsize ( _sizes.slider_width, _sizes.area_height );
		for ( int ii=0; ii < _sliders.size(); ++ii ) {
			_sliders[ii]->set_size ( rsize );
		}
	}
	update_geometries();
}

void
GW_Levels::update_geometries ( )
{
	const double delta_x ( _sizes.slider_width + _sizes.channels_hgap );
	{
		QPointF spos ( 0.0, 0.0 );
		for ( int ii=0; ii < _sliders.size(); ++ii ) {
			_sliders[ii]->setPos ( spos );
			spos.rx() += delta_x;
		}
	}
	_slider_handle->setPos ( QPointF ( 0.0, 0.0 ) );
}

unsigned int
GW_Levels::int_width ( ) const
{
	unsigned int rwidth ( 0 );
	if ( _snd_proxies.num_proxies() > 0 ) {
		rwidth += _sizes.slider_width * _snd_proxies.num_proxies();
		rwidth += _sizes.channels_hgap * ( _snd_proxies.num_proxies() - 1 );
	}
	return rwidth;
}



} // End of namespace
