//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_switches.hpp"
#include <iostream>

#include <QPainter>
#include <QStyleOptionGraphicsItem>


namespace Wdg2
{


GW_Switches::GW_Switches (
	::QSnd2::Proxies_Group1_Switch & snd_proxies_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_snd_proxies ( snd_proxies_n )
{
	for ( unsigned int ii=0; ii < _snd_proxies.num_proxies(); ++ii ) {
		_switches.append ( new ::Wdg2::GW_Switch ( this ) );
	}
}

GW_Switches::~GW_Switches ( )
{
}

QRectF
GW_Switches::boundingRect ( ) const
{
	return QRectF ( 0.0, 0.0, 0.0, 0.0 );
}

void
GW_Switches::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;
}

void
GW_Switches::set_sizes (
	const ::Wdg2::GW_Switches_Sizes & sizes_n )
{
	_sizes = sizes_n;

	{
		QSize ssize ( _sizes.switch_width, _sizes.area_height );
		for ( int ii=0; ii < _switches.size(); ++ii ) {
			_switches[ii]->set_switch_size ( ssize );
		}
	}

	update_geometries();
}

void
GW_Switches::update_geometries ( )
{
	const double delta_x ( _sizes.switch_width + _sizes.channels_hgap );
	QPointF spos ( 0.0, 0.0 );
	for ( int ii=0; ii < _switches.size(); ++ii ) {
		_switches[ii]->setPos ( spos );
		spos.rx() += delta_x;
	}
}

unsigned int
GW_Switches::int_width ( ) const
{
	unsigned int rwidth ( 0 );
	if ( _snd_proxies.num_proxies() > 0 ) {
		rwidth += _sizes.switch_width * _snd_proxies.num_proxies();
		rwidth += _sizes.channels_hgap * ( _snd_proxies.num_proxies() - 1 );
	}
	return rwidth;
}



GW_Switch::GW_Switch (
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_switch_size ( 0.0, 0.0 )
{
}

GW_Switch::~GW_Switch ( )
{
}

QRectF
GW_Switch::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), _switch_size );
}

void
GW_Switch::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;

	double pen_width ( 1.0 );
	double pwhalf ( pen_width / 2.0 );
	QRectF prect ( QPointF ( 0.0, 0.0 ), _switch_size );
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
GW_Switch::set_switch_size (
	const QSize & size_n )
{
	prepareGeometryChange();
	_switch_size = size_n;
}


} // End of namespace
