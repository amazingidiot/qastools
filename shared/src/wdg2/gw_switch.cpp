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


GW_Switch_Ground::GW_Switch_Ground (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Pixmaps ( scene_db_n, 2, parent_n )
{
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

bool
GW_Switch_Ground::setup_request (
	unsigned int idx_n,
	::dpe2::Key_Values & kvals_n )
{
	bool res ( pxm_size_valid() );
	if ( res ) {
		kvals_n.set_uint ( ::Wdg2::PRK_WIDTH,  pxm_size().width() );
		kvals_n.set_uint ( ::Wdg2::PRK_HEIGHT, pxm_size().height() );
		kvals_n.set_uint ( ::Wdg2::PRK_WIDGET_TYPE, ::Wdg2::WGT_SWITCH );
		kvals_n.set_uint ( ::Wdg2::PRK_WIDGET_PART, ::Wdg2::WGP_SWITCH_GROUND );
		{
			::Flags sflags;
			if ( idx_n == 1 ) {
				sflags.set ( ::Wdg2::GW_HAS_FOCUS );
			}
			kvals_n.set_uint ( ::Wdg2::PRK_WIDGET_STATE_FLAGS, sflags.flags() );
		}
	}
	return res;
}



GW_Switch_Handle::GW_Switch_Handle (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Pixmaps ( scene_db_n, 2, parent_n )
{
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

bool
GW_Switch_Handle::setup_request (
	unsigned int idx_n,
	::dpe2::Key_Values & kvals_n )
{
	bool res ( pxm_size_valid() );
	if ( res ) {
		kvals_n.set_uint ( ::Wdg2::PRK_WIDTH,  pxm_size().width() );
		kvals_n.set_uint ( ::Wdg2::PRK_HEIGHT, pxm_size().height() );
		kvals_n.set_uint ( ::Wdg2::PRK_WIDGET_TYPE, ::Wdg2::WGT_SWITCH );
		kvals_n.set_uint ( ::Wdg2::PRK_WIDGET_PART, ::Wdg2::WGP_SWITCH_HANDLE );
		{
			::Flags sflags;
			if ( idx_n == 1 ) {
				sflags.set ( ::Wdg2::GW_IS_ON );
			}
			kvals_n.set_uint ( ::Wdg2::PRK_WIDGET_STATE_FLAGS, sflags.flags() );
		}
	}
	return res;
}



GW_Switch::GW_Switch (
	::QSnd2::Proxy_Switch & switch_proxy_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_switch_proxy ( switch_proxy_n ),
_switch_size ( 0.0, 0.0 ),
_ground ( scene_db(), this ),
_handle ( scene_db(), this )
{
	_switch_proxy.set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_Switch::read_proxy_value_cb ) );
	setFlags ( QGraphicsItem::ItemIsFocusable );
}

GW_Switch::~GW_Switch ( )
{
}

void
GW_Switch::set_switch_size (
	const QSize & size_n )
{
	if ( size_n != _switch_size ) {
		_switch_size = size_n;
		set_bounding_rect ( _switch_size );
		_ground.set_pxm_size ( _switch_size );
		_handle.set_pxm_size ( _switch_size );
		read_proxy_value();
	}
}

void
GW_Switch::read_proxy_value ( )
{
	const bool pstate ( _switch_proxy.switch_state() );
	if ( pstate != _state_flags.has_any ( ::Wdg2::GW_IS_ON ) ) {
		_state_flags.set ( ::Wdg2::GW_IS_ON, pstate );
		_ground.set_state_flags ( ::Wdg2::GW_IS_ON, pstate );
		_handle.set_state_flags ( ::Wdg2::GW_IS_ON, pstate );
		update();
	}
}

void
GW_Switch::read_proxy_value_cb (
	void * context_n )
{
	::Wdg2::GW_Switch & gw_switch (
		*reinterpret_cast < ::Wdg2::GW_Switch * > ( context_n ) );
	gw_switch.read_proxy_value();
}

void
GW_Switch::focusInEvent (
	QFocusEvent * event_n )
{
	//::std::cout << "GW_Switch::focusInEvent"  << "\n";
	_state_flags.set ( ::Wdg2::GW_HAS_FOCUS );
	_ground.set_state_flags ( ::Wdg2::GW_HAS_FOCUS );
	_handle.set_state_flags ( ::Wdg2::GW_HAS_FOCUS );
}

void
GW_Switch::focusOutEvent (
	QFocusEvent * event_n )
{
	//::std::cout << "GW_Switch::focusOutEvent"  << "\n";
	_state_flags.set ( ::Wdg2::GW_HAS_FOCUS, false );
	_ground.set_state_flags ( ::Wdg2::GW_HAS_FOCUS, false );
	_handle.set_state_flags ( ::Wdg2::GW_HAS_FOCUS, false );
}

void
GW_Switch::mousePressEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	//::std::cout << "GW_Switch::mousePressEvent"  << "\n";
	_state_flags.set ( ::Wdg2::GW_IS_GRABBED );
	_handle.set_state_flags ( ::Wdg2::GW_IS_GRABBED );
}

void
GW_Switch::mouseReleaseEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	//::std::cout << "GW_Switch::mouseReleaseEvent"  << "\n";
	_state_flags.unset ( ::Wdg2::GW_IS_GRABBED );
	_handle.set_state_flags ( ::Wdg2::GW_IS_GRABBED, false );

	_switch_proxy.toggle_switch_state();
	read_proxy_value();
}


} // End of namespace
