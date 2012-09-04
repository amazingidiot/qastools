//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_switch.hpp"
#include "theme_painters.hpp"
#include <iostream>

namespace Wdg2
{


GW_Switch::GW_Switch (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_ground ( new ::Wdg2::GW_Switch_Ground ( scene_db(), this ) ),
_handle ( new ::Wdg2::GW_Switch_Handle ( scene_db(), this ) ),
_switch_size ( 0, 0 )
{
	setFlags ( QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemHasNoContents );
}

GW_Switch::~GW_Switch ( )
{
}

void
GW_Switch::set_switch_size (
	const QSize & size_n )
{
	if ( _switch_size != size_n ) {
		_switch_size = size_n;
		set_bounding_rect ( _switch_size );
		_ground->set_pxm_size ( _switch_size );
		_handle->set_pxm_size ( _switch_size );
	}
}

void
GW_Switch::set_switch_state (
	bool state_n )
{
	if ( state_n != _state_flags.has_any ( ::Wdg2::GW_IS_ON ) ) {
		_state_flags.set ( ::Wdg2::GW_IS_ON, state_n );
		_ground->set_state_flags ( ::Wdg2::GW_IS_ON, state_n );
		_handle->set_state_flags ( ::Wdg2::GW_IS_ON, state_n );

		_val_change_cb.call_if_valid();
	}
}

void
GW_Switch::toggle_switch_state ( )
{
	set_switch_state ( !switch_state() );
}

void
GW_Switch::set_val_change_callback (
	const ::Context_Callback & cb_n )
{
	_val_change_cb = cb_n;
}

void
GW_Switch::focusInEvent (
	QFocusEvent * event_n )
{
	//::std::cout << "GW_Switch::focusInEvent"  << "\n";
	_state_flags.set ( ::Wdg2::GW_HAS_FOCUS );
	_ground->set_state_flags ( ::Wdg2::GW_HAS_FOCUS );
	_handle->set_state_flags ( ::Wdg2::GW_HAS_FOCUS );
}

void
GW_Switch::focusOutEvent (
	QFocusEvent * event_n )
{
	//::std::cout << "GW_Switch::focusOutEvent"  << "\n";
	_state_flags.set ( ::Wdg2::GW_HAS_FOCUS, false );
	_ground->set_state_flags ( ::Wdg2::GW_HAS_FOCUS, false );
	_handle->set_state_flags ( ::Wdg2::GW_HAS_FOCUS, false );
}

void
GW_Switch::mousePressEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	//::std::cout << "GW_Switch::mousePressEvent"  << "\n";
	_state_flags.set ( ::Wdg2::GW_IS_GRABBED );
	_handle->set_state_flags ( ::Wdg2::GW_IS_GRABBED );
}

void
GW_Switch::mouseReleaseEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	//::std::cout << "GW_Switch::mouseReleaseEvent"  << "\n";
	_state_flags.unset ( ::Wdg2::GW_IS_GRABBED );
	_handle->set_state_flags ( ::Wdg2::GW_IS_GRABBED, false );

	toggle_switch_state();
}




GW_Switch_Ground::GW_Switch_Ground (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Pixmaps ( scene_db_n, 2, parent_n )
{
	pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_TYPE, ::Wdg2::WGT_SWITCH );
	pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_PART, ::Wdg2::WGP_SWITCH_GROUND );
}

void
GW_Switch_Ground::update_pxm_idx ( )
{
	unsigned int idx ( 0 );
	if ( state_flags().has_any ( ::Wdg2::GW_HAS_FOCUS ) ) {
		idx = 1;
	}
	set_pxm_idx ( idx );
}



GW_Switch_Handle::GW_Switch_Handle (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Pixmaps ( scene_db_n, 2, parent_n )
{
	pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_TYPE, ::Wdg2::WGT_SWITCH );
	pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_PART, ::Wdg2::WGP_SWITCH_HANDLE );
}

void
GW_Switch_Handle::update_pxm_idx ( )
{
	unsigned int idx ( 0 );
	if ( state_flags().has_any ( ::Wdg2::GW_IS_ON ) ) {
		idx = 1;
	}
	set_pxm_idx ( idx );
}


} // End of namespace
