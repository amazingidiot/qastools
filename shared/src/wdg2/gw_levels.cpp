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


GW_Levels::GW_Levels (
	::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_snd_proxies ( snd_proxies_n )
{
	for ( unsigned int ii=0; ii < _snd_proxies.num_proxies(); ++ii ) {
		_slider_rails.append ( new ::Wdg2::GW_Slider_Rail ( this ) );
		_slider_handles.append ( new ::Wdg2::GW_Slider_Handle ( this ) );
	}
	_slider_handle = new ::Wdg2::GW_Slider_Handle ( this );
}

GW_Levels::~GW_Levels ( )
{
}

QRectF
GW_Levels::boundingRect ( ) const
{
	return QRectF();
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
		for ( int ii=0; ii < _slider_rails.size(); ++ii ) {
			_slider_rails[ii]->set_rail_size ( rsize );
		}
	}
	{
		QSize hsize ( _sizes.slider_width, _sizes.area_height / 10 );
		for ( int ii=0; ii < _slider_rails.size(); ++ii ) {
			_slider_handles[ii]->set_handle_size ( hsize );;
		}
		_slider_handle->set_handle_size ( hsize );
	}

	update_geometries();
}

void
GW_Levels::update_geometries ( )
{
	const double delta_x ( _sizes.slider_width + _sizes.channels_hgap );
	{
		QPointF spos ( 0.0, 0.0 );
		for ( int ii=0; ii < _slider_rails.size(); ++ii ) {
			_slider_rails[ii]->setPos ( spos );
			spos.rx() += delta_x;
		}
	}
	{
		QPointF spos ( 0.0, 0.0 );
		for ( int ii=0; ii < _slider_handles.size(); ++ii ) {
			_slider_handles[ii]->setPos ( spos );
			spos.rx() += delta_x;
		}
	}
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



GW_Slider_Rail::GW_Slider_Rail (
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n )
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
QGraphicsItem ( parent_n )
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


} // End of namespace
