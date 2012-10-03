//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_slider_multi.hpp"
#include "theme_painters.hpp"
#include <iostream>

namespace Wdg2
{


GW_Slider_Multi::GW_Slider_Multi (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Slider (
	scene_db_n,
	new ::Wdg2::GW_Slider_Multi_Rail ( scene_db_n ),
	new ::Wdg2::GW_Slider_Multi_Handle ( scene_db_n ),
	parent_n )
{
}

GW_Slider_Multi::~GW_Slider_Multi ( )
{
}

void
GW_Slider_Multi::load_settings_multi (
	const ::Wdg2::GW_Slider_Multi_Settings & settings_n )
{
	_settings_multi = settings_n;
	{
		::Wdg2::GW_Slider_Settings sl_settings ( settings() );
		sl_settings.size = QSize ( int_width(), _settings_multi.area_height );
		sl_settings.handle_length = _settings_multi.area_height / 10;
		::Wdg2::GW_Slider::load_settings ( sl_settings  );
	}
}

unsigned int
GW_Slider_Multi::int_width_probe (
	const ::Wdg2::GW_Slider_Multi_Settings & settings_n ) const
{
	unsigned int iwidth ( 0 );
	if ( settings_n.num_sliders > 0 ) {
		iwidth += settings_n.slider_width * settings_n.num_sliders;
		iwidth += settings_n.channels_gap * ( settings_n.num_sliders - 1 );
	}
	return iwidth;
}

unsigned int
GW_Slider_Multi::int_width ( ) const
{
	return int_width_probe ( settings_multi() );
}



GW_Slider_Multi_Rail::GW_Slider_Multi_Rail (
	::Wdg2::Scene_Database * scene_db_n,
	::Wdg2::GW_Slider_Multi * parent_n ) :
_wdg ( scene_db_n, parent_n )
{
	_wdg.setFlags ( QGraphicsItem::ItemHasNoContents );
}

GW_Slider_Multi_Rail::~GW_Slider_Multi_Rail ( )
{
	clear_rails();
}

void
GW_Slider_Multi_Rail::clear_rails ( )
{
	for ( int ii=0; ii < _rails.size(); ++ii ) {
		delete _rails[ii];
	}
	_rails.clear();
}

void
GW_Slider_Multi_Rail::size_changed ( )
{
	reload_settings();
}

void
GW_Slider_Multi_Rail::pos_changed ( )
{
	_wdg.setPos ( pos() );
}

void
GW_Slider_Multi_Rail::state_flags_changed ( )
{
	unsigned int idx ( 0 );
	if ( state_flags().has_any ( ::Wdg2::GW_HAS_FOCUS ) ) {
		idx = 1;
	}
	for ( int ii=0; ii < _rails.size(); ++ii ) {
		_rails[ii]->set_pxm_idx ( idx );
	}
}

void
GW_Slider_Multi_Rail::reload_settings ( )
{
	const ::Wdg2::GW_Slider_Multi_Settings & settings_multi (
		slider_multi()->settings_multi() );

	if ( _rails.size() != (int)settings_multi.num_sliders ) {
		clear_rails();
		for ( unsigned int ii=0; ii < settings_multi.num_sliders; ++ii ) {
			::Wdg2::GW_Pixmaps * rail (
				new ::Wdg2::GW_Pixmaps ( _wdg.scene_db(), 2, &_wdg ) );
			rail->pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_TYPE, ::Wdg2::WGT_SLIDER );
			rail->pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_PART, ::Wdg2::WGP_SLIDER_RAIL );
			_rails.append ( rail );
		}
	}

	if ( _rails.size() > 0 ) {
		const double delta_x ( settings_multi.slider_width + settings_multi.channels_gap );
		QPointF spos ( 0.0, 0.0 );

		QSize rail_size ( settings_multi.slider_width, settings_multi.area_height );
		for ( int ii=0; ii < _rails.size(); ++ii ) {
			::Wdg2::GW_Pixmaps * rail ( _rails[ii] );
			rail->set_size ( rail_size );
			rail->setPos ( spos );
			spos.rx() += delta_x;
		}
	}
}



GW_Slider_Multi_Handle::GW_Slider_Multi_Handle (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Slider_Handle ( scene_db_n, parent_n )
{
	// TODO:
	//gw_pixmaps()->pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_TYPE, ::Wdg2::WGT_SLIDER );
	//gw_pixmaps()->pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_PART, ::Wdg2::WGP_SLIDER_HANDLE );
}


} // End of namespace
