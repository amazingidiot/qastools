//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "mixer_sliders_status_widget.hpp"

#include "wdg/sliders_pad.hpp"
#include <iostream>


namespace MWdg
{


Mixer_Sliders_Status_Widget::Mixer_Sliders_Status_Widget (
	QWidget * parent_n ) :
Slider_Status_Widget ( parent_n ),
_proxy_slider ( 0 )
{
}


Mixer_Sliders_Status_Widget::~Mixer_Sliders_Status_Widget ( )
{
}


void
Mixer_Sliders_Status_Widget::select_slider (
	unsigned int grp_idx_n,
	unsigned int col_idx_n )
{
	if ( sliders_pad() != 0 ) {
		::MWdg::Mixer_Sliders_Proxy_Slider * proxy_new ( _proxy_slider );
		if ( grp_idx_n < sliders_pad()->num_proxies_groups() ) {
			::Wdg::Pad_Proxies_Group * pgroup (
				sliders_pad()->proxies_group ( grp_idx_n ) );

			if ( col_idx_n < pgroup->num_sliders() ) {
				proxy_new = dynamic_cast < ::MWdg::Mixer_Sliders_Proxy_Slider * > (
					pgroup->column ( col_idx_n )->slider_proxy() );
			}
		}
		set_slider_proxy ( proxy_new );
	}
}


void
Mixer_Sliders_Status_Widget::proxy_destroyed ( )
{
	set_slider_proxy ( 0 );
}


void
Mixer_Sliders_Status_Widget::set_slider_proxy (
	::MWdg::Mixer_Sliders_Proxy_Slider * proxy_n )
{
	//::std::cout << "Mixer_Sliders_Status_Widget::set_slider_proxy " << proxy_n  << "\n";

	if ( _proxy_slider != proxy_n ) {

		// Disconnect previous proxy
		if ( _proxy_slider != 0 ) {
			if ( _proxy_slider->mixer_simple_elem() != 0 ) {
				disconnect ( _proxy_slider->mixer_simple_elem(), 0, this, 0 );
			}
			disconnect ( _proxy_slider, 0, this, 0 );
		}

		_proxy_slider = proxy_n;

		setup_values();

		if ( _proxy_slider != 0 ) {
			if ( _proxy_slider->mixer_simple_elem() != 0 ) {
				connect ( _proxy_slider->mixer_simple_elem(), SIGNAL ( sig_values_changed() ),
					this, SLOT ( update_values() ) );
			}
			connect ( _proxy_slider, SIGNAL ( destroyed (  QObject * ) ),
				this, SLOT ( proxy_destroyed() ) );

			update_values();
		}
	}
}


QString
Mixer_Sliders_Status_Widget::elem_name ( ) const
{
	QString res;
	if ( _proxy_slider != 0 ) {
		res = _proxy_slider->group_name();
		if ( res != _proxy_slider->item_name() ) {
			res += " - ";
			res += _proxy_slider->item_name();
		}
	}
	return res;
}


bool
Mixer_Sliders_Status_Widget::elem_has_volume ( ) const
{
	return ( _proxy_slider != 0 );
}


bool
Mixer_Sliders_Status_Widget::elem_has_dB ( ) const
{
	bool res ( false );
	if ( _proxy_slider != 0 ) {
		res = _proxy_slider->has_dB();
	}
	return res;
}


long
Mixer_Sliders_Status_Widget::elem_volume_value ( ) const
{
	long res ( 0 );
	if ( _proxy_slider != 0 ) {
		res = _proxy_slider->volume_value();
	}
	return res;
}


void
Mixer_Sliders_Status_Widget::elem_set_volume (
	long value_n ) const
{
	if ( _proxy_slider != 0 ) {
		_proxy_slider->set_volume_value ( value_n );
	}
}


long
Mixer_Sliders_Status_Widget::elem_volume_min ( ) const
{
	long res ( 0 );
	if ( _proxy_slider != 0 ) {
		res = _proxy_slider->volume_min();
	}
	return res;
}


long
Mixer_Sliders_Status_Widget::elem_volume_max ( ) const
{
	long res ( 0 );
	if ( _proxy_slider != 0 ) {
		res = _proxy_slider->volume_max();
	}
	return res;
}


long
Mixer_Sliders_Status_Widget::elem_dB_value ( ) const
{
	long res ( 0 );
	if ( _proxy_slider != 0 ) {
		res = _proxy_slider->dB_value();
	}
	return res;
}


void
Mixer_Sliders_Status_Widget::elem_set_nearest_dB (
	long dB_value_n ) const
{
	if ( _proxy_slider != 0 ) {
		const long vol_near (
			_proxy_slider->ask_dB_vol_nearest ( dB_value_n ) );
		_proxy_slider->set_volume_value ( vol_near );
	}
}


long
Mixer_Sliders_Status_Widget::elem_dB_min ( ) const
{
	long res ( 0 );
	if ( _proxy_slider != 0 ) {
		res = _proxy_slider->dB_min();
	}
	return res;
}


long
Mixer_Sliders_Status_Widget::elem_dB_max ( ) const
{
	long res ( 0 );
	if ( _proxy_slider != 0 ) {
		res = _proxy_slider->dB_max();
	}
	return res;
}


} // End of namespace

