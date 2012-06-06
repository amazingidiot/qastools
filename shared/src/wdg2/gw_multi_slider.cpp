//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_multi_slider.hpp"
#include <iostream>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


namespace Wdg2
{


GW_Multi_Slider::GW_Multi_Slider (
	::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_proxies_grp ( snd_proxies_n )
{
	{
		::QSnd2::Integer_Pair vrange;
		_proxies_grp.int_range ( vrange );
		_value_map.set_value_range ( vrange[0], vrange[1] );
	}
	for ( unsigned int ii=0; ii < proxies_grp().num_proxies(); ++ii ) {
		::Wdg2::GW_Slider * gw_slider (
			new ::Wdg2::GW_Slider ( *proxies_grp().slider_proxy ( ii ), this ) );
		_sliders.append ( gw_slider );
	}
	_slider_handle = new ::Wdg2::GW_Slider_Handle ( this );
	_slider_handle->setVisible ( false );
}

GW_Multi_Slider::~GW_Multi_Slider ( )
{
}

QRectF
GW_Multi_Slider::boundingRect ( ) const
{
	return QRectF ( 0, 0, 0, 0 );
}

void
GW_Multi_Slider::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;
}

void
GW_Multi_Slider::set_sizes (
	const ::Wdg2::GW_Multi_Slider_Sizes & sizes_n )
{
	_sizes = sizes_n;
	{
		::Wdg2::GW_Slider_Sizes lsizes;
		lsizes.size.setWidth ( _sizes.slider_width );
		lsizes.size.setHeight ( _sizes.area_height );
		lsizes.handle_size.setWidth ( _sizes.slider_width );
		lsizes.handle_size.setHeight ( _sizes.area_height / 10 );
		_value_map.set_px_span ( lsizes.size.height() - lsizes.handle_size.height() );
		for ( int ii=0; ii < _sliders.size(); ++ii ) {
			_sliders[ii]->set_sizes ( lsizes );
		}
	}
	update_geometries();
}

void
GW_Multi_Slider::update_geometries ( )
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
GW_Multi_Slider::int_width ( ) const
{
	return int_width_probe ( _sizes );
}

unsigned int
GW_Multi_Slider::int_width_probe (
	const ::Wdg2::GW_Multi_Slider_Sizes & sizes_n ) const
{
	unsigned int iwidth ( 0 );
	if ( proxies_grp().num_proxies() > 0 ) {
		iwidth += sizes_n.slider_width * proxies_grp().num_proxies();
		iwidth += sizes_n.channels_hgap * ( proxies_grp().num_proxies() - 1 );
	}
	return iwidth;
}


} // End of namespace
