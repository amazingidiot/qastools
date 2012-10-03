//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_mixer_pad.hpp"

namespace Wdg2
{


GW_Mixer_Pad::GW_Mixer_Pad (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_snd_controls ( 0 ),
_pad_size ( 0, 0 ),
_gw_sliders_pad ( scene_db(), this ),
_gw_switches_pad ( scene_db(), this )
{
}

GW_Mixer_Pad::~GW_Mixer_Pad ( )
{
}

void
GW_Mixer_Pad::set_pad_size (
	const QSize & size_n )
{
	if ( _pad_size != size_n ) {
		_pad_size = size_n;
		set_bounding_rect ( _pad_size );
		update_geometries();
	}
}

void
GW_Mixer_Pad::set_snd_controls (
	::QSnd2::Controls * controls_n )
{
	if ( _snd_controls != controls_n ) {
		_snd_controls = controls_n;
		_gw_sliders_pad.set_snd_controls ( controls_n );
		update_geometries();
	}
}

void
GW_Mixer_Pad::update_geometries ( )
{
	if ( snd_controls() != 0 ) {
		QSize sl_size ( pad_size() );
		QSize sw_size ( pad_size() );
		sl_size.rwidth() *= 0.75;
		sw_size.setWidth ( pad_size().width() - sl_size.width() );

		_gw_sliders_pad.set_size ( sl_size );
		_gw_switches_pad.set_size ( sw_size );
		_gw_sliders_pad.show();
		_gw_switches_pad.show();
	} else {
		_gw_sliders_pad.hide();
		_gw_switches_pad.hide();
	}
}


} // End of namespace
