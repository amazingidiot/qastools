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
