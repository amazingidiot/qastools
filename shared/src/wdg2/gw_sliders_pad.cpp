//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "gw_sliders_pad.hpp"
#include "gw_sliders_pad_groups.hpp"
#include "qsnd2/controls.hpp"
#include <iostream>
#include <cmath>
#include <QGraphicsScene>

namespace Wdg2
{


GW_Sliders_Pad::GW_Sliders_Pad (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Scroll_Area ( scene_db_n, parent_n ),
_snd_controls ( 0 )
{
}

GW_Sliders_Pad::~GW_Sliders_Pad ( )
{
	set_snd_controls ( 0 );
}

void
GW_Sliders_Pad::set_snd_controls (
	::QSnd2::Controls * controls_n )
{
	if ( _snd_controls != 0 ) {
		destroy_items();
	}
	_snd_controls = controls_n;
	if ( _snd_controls != 0 ) {
		build_items();
	}
}

void
GW_Sliders_Pad::destroy_items ( )
{
	QGraphicsScene * qscene ( scene() );
	if ( _group4 != 0 ) {
		if ( qscene != 0 ) {
			qscene->removeItem ( _group4.data() );
		}
		take_widget();
		_group4.reset();
	}
}

void
GW_Sliders_Pad::build_items ( )
{
	if ( _snd_controls->num_groups() > 0 ) {
		_group4.reset (
			new ::Wdg2::GW_SlPad_Group4 ( *_snd_controls->group ( 0 ), scene_db() ) );
		if ( _group4->num_children() > 0 ) {
			set_widget ( _group4.data() );
		} else {
			_group4.reset();
		}
	}
}

unsigned int
GW_Sliders_Pad::viewport_resize (
	QSize size_off_n,
	QSize size_on_n )
{
	unsigned int res ( 0 );
	if ( _group4 != 0 ) {
		::Wdg2::GW_SlPad_Group4_Sizes lsizes;
		lsizes.height = size_off_n.height();
		lsizes.slider_width = 16;
		lsizes.channels_gap = 8;
		lsizes.group2_hgap = 16;
		lsizes.group3_hgap = 16;

		const unsigned int iwidth ( _group4->int_width_probe ( lsizes ) );
		if ( iwidth > (unsigned int)size_off_n.width() ) {
			lsizes.height = size_on_n.height();
			res = iwidth;
		}

		_group4->set_sizes ( lsizes );
	}
	return res;
}


} // End of namespace
