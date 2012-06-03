//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_multi_switch.hpp"
#include <iostream>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


namespace Wdg2
{


GW_Multi_Switch::GW_Multi_Switch (
	::QSnd2::Proxies_Group1_Switch & snd_proxies_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_snd_proxies ( snd_proxies_n )
{
	for ( unsigned int ii=0; ii < proxies_grp().num_proxies(); ++ii ) {
		::Wdg2::GW_Switch * gw_switch (
			new ::Wdg2::GW_Switch ( *proxies_grp().switch_proxy ( ii ), this ) );
		_switches.append ( gw_switch );
	}
}

GW_Multi_Switch::~GW_Multi_Switch ( )
{
}

QRectF
GW_Multi_Switch::boundingRect ( ) const
{
	return QRectF ( 0.0, 0.0, 0.0, 0.0 );
}

void
GW_Multi_Switch::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;
}

void
GW_Multi_Switch::set_sizes (
	const ::Wdg2::GW_Multi_Switch_Sizes & sizes_n )
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
GW_Multi_Switch::update_geometries ( )
{
	const double delta_x ( _sizes.switch_width + _sizes.channels_hgap );
	QPointF spos ( 0.0, 0.0 );
	for ( int ii=0; ii < _switches.size(); ++ii ) {
		_switches[ii]->setPos ( spos );
		spos.rx() += delta_x;
	}
}

unsigned int
GW_Multi_Switch::int_width ( ) const
{
	unsigned int rwidth ( 0 );
	if ( _snd_proxies.num_proxies() > 0 ) {
		rwidth += _sizes.switch_width * _snd_proxies.num_proxies();
		rwidth += _sizes.channels_hgap * ( _snd_proxies.num_proxies() - 1 );
	}
	return rwidth;
}


} // End of namespace
