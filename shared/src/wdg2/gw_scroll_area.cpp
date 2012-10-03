//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_scroll_area.hpp"
#include <iostream>

namespace Wdg2
{


GW_Scroll_Area::GW_Scroll_Area (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_scroll_orientation ( Qt::Horizontal ),
_panels_shift ( 0 ),
_panels_shift_max ( 0 ),
_viewport ( scene_db_n, this ),
_widget ( 0 )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );
	_viewport.setFlags ( QGraphicsItem::ItemHasNoContents | QGraphicsItem::ItemClipsChildrenToShape );
}

GW_Scroll_Area::~GW_Scroll_Area ( )
{
}

void
GW_Scroll_Area::set_size (
	const QSize & size_n )
{
	if ( size() != size_n ) {
		::Wdg2::GW_Widget::set_size ( size_n );
		update_geometries();
	}
}

void
GW_Scroll_Area::set_widget (
	::Wdg2::GW_Widget * widget_n )
{
	if ( _widget != widget_n ) {
		if ( _widget != 0 ) {
			delete _widget;
		}
		_widget = widget_n;
		if ( _widget != 0 ) {
			_widget->setParentItem ( &_viewport );
		}
		update_geometries();
	}
}

::Wdg2::GW_Widget *
GW_Scroll_Area::take_widget ( )
{
	::Wdg2::GW_Widget * res ( _widget );
	if ( _widget != 0 ) {
		_widget->setParentItem ( 0 );
		_widget = 0;
		update_geometries();
	}
	return res;
}

void
GW_Scroll_Area::set_scroll_orientation (
	Qt::Orientation orientation_n )
{
	if ( orientation_n != Qt::Horizontal ) {
		orientation_n = Qt::Vertical;
	}
	if ( _scroll_orientation != orientation_n ) {
		_scroll_orientation = orientation_n;
		if ( _widget != 0 ) {
			update_geometries();
		}
	}
}

void
GW_Scroll_Area::set_panels_shift (
	long amount_n )
{
	if ( amount_n < 0 ) {
		amount_n = 0;
	}
	if ( amount_n > (long)_panels_shift_max ) {
		amount_n = _panels_shift_max;
	}
	if ( _panels_shift != amount_n ) {
		_panels_shift = amount_n;
		update_widget_position();
	}
}

void
GW_Scroll_Area::read_panels_shift ( )
{
	set_panels_shift ( _scrollbar->int_value() );
}

void
GW_Scroll_Area::read_panels_shift_cb (
	void * context_n )
{
	::Wdg2::GW_Scroll_Area & pad (
		*reinterpret_cast < ::Wdg2::GW_Scroll_Area * > ( context_n ) );
	pad.read_panels_shift();
}

void
GW_Scroll_Area::update_geometries ( )
{
	const unsigned int sbar_width ( 16 );
	const unsigned int sbar_gap ( 2 );
	const unsigned int sbar_wspace ( sbar_width + sbar_gap );

	unsigned int scroll_length ( 0 );
	QSize size_off ( 0, 0 );
	QSize size_on ( 0, 0 );

	if ( ( size().width() > 0 ) && ( size().height() ) ) {
		size_off = size();
		size_on = size();
	}
	if ( scroll_orientation() == Qt::Horizontal ) {
		if ( (unsigned int)size_on.height() > sbar_wspace ) {
			size_on.rheight() -= sbar_wspace;
			++scroll_length;
		}
	} else {
		if ( (unsigned int)size_on.width() > sbar_wspace ) {
			size_on.rwidth() -= sbar_wspace;
			++scroll_length;
		}
	}

	if ( scroll_length != 0 ) {
		scroll_length = this->viewport_resize ( size_off, size_on );
	}

	_panels_shift_max = 0;
	_panels_shift = 0;
	if ( scroll_length > 0 ) {
		_viewport.set_size ( size_on );
		if ( scroll_length > (unsigned int)size_on.width() ) {
			_panels_shift_max = ( scroll_length - size_on.width() );
		}
		// Create scrollbar on demand and set it up
		if ( _scrollbar == 0 ) {
			_scrollbar.reset ( new ::Wdg2::GW_Scrollbar ( scene_db(), this ) );
			_scrollbar->set_val_change_callback (
				::Context_Callback ( this, ::Wdg2::GW_Scroll_Area::read_panels_shift_cb ) );
		}
		_scrollbar->set_orientation ( scroll_orientation() );
		{
			// position and size
			QPointF sb_pos ( 0, 0 );
			QSize sb_size ( size_off );
			if ( scroll_orientation() == Qt::Horizontal ) {
				sb_pos.setY ( size_on.height() + sbar_gap );
				sb_size.setHeight ( sbar_width );
			} else {
				sb_pos.setX ( size_on.width() + sbar_gap );
				sb_size.setWidth ( sbar_width );
			}
			_scrollbar->setPos ( sb_pos );
			_scrollbar->set_size ( sb_size );
		}
		_scrollbar->set_int_span ( _panels_shift_max );
		_panels_shift = _scrollbar->int_value();
	} else {
		_viewport.set_size ( size_off );
		_scrollbar.reset ( 0 );
	}

	update_widget_position();
}

void
GW_Scroll_Area::update_widget_position ( )
{
	if ( _widget != 0 ) {
		QPointF pos ( 0.0, 0.0 );
		const double dshift ( -double ( _panels_shift ) );
		if ( scroll_orientation() == Qt::Horizontal ) {
			pos.setX ( dshift );
		} else {
			pos.setY ( dshift );
		}
		_widget->setPos ( pos );
	}
}

unsigned int
GW_Scroll_Area::viewport_resize (
	QSize size_off_n,
	QSize size_on_n )
{
	// Default implementation
	(void) size_off_n;
	(void) size_on_n;
	return 0;
}


} // End of namespace
