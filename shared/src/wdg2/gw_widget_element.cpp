//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_widget_element.hpp"
#include <cassert>

namespace Wdg2
{


GW_Widget_Element::GW_Widget_Element ( )
{
}

GW_Widget_Element::~GW_Widget_Element ( )
{
}

void
GW_Widget_Element::set_size (
	const QSize & size_n )
{
	if ( _size != size_n ) {
		_size = size_n;
		this->size_changed();
	}
}

void
GW_Widget_Element::set_pos (
	const QPoint & pos_n )
{
	if ( _pos != pos_n ) {
		_pos = pos_n;
		this->pos_changed();
	}
}

void
GW_Widget_Element::set_state_flags (
	unsigned int flags_n,
	bool on_n )
{
	const ::Flags fprev ( _state_flags );
	_state_flags.set ( flags_n, on_n );
	if ( _state_flags != fprev ) {
		this->state_flags_changed();
	}
}

void
GW_Widget_Element::size_changed ( )
{
	// Default implementation
}

void
GW_Widget_Element::pos_changed ( )
{
	// Default implementation
}

void
GW_Widget_Element::state_flags_changed ( )
{
	// Default implementation
}



GW_Widget_Element_Pixmaps::GW_Widget_Element_Pixmaps (
	::Wdg2::Scene_Database * scene_db_n,
	unsigned int num_pixmaps_n,
	QGraphicsItem * parent_n ) :
_gw_pixmaps ( scene_db_n, num_pixmaps_n, parent_n )
{
}

GW_Widget_Element_Pixmaps::~GW_Widget_Element_Pixmaps ( )
{
}

void
GW_Widget_Element_Pixmaps::size_changed ( )
{
	gw_pixmaps()->set_size ( size() );
}

void
GW_Widget_Element_Pixmaps::pos_changed ( )
{
	gw_pixmaps()->setPos ( pos() );
}


} // End of namespace
