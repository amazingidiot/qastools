//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_sliders_pad.hpp"
#include "gw_groups.hpp"
#include "qsnd2/controls.hpp"
#include <iostream>
#include <cmath>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

namespace Wdg2
{


GW_Sliders_Pad::GW_Sliders_Pad (
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_snd_controls ( 0 ),
_pad_size ( 0, 0 ),
_panels_shift ( 0 ),
_panels_shift_max ( 0 )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );
}

GW_Sliders_Pad::~GW_Sliders_Pad ( )
{
	set_snd_controls ( 0 );
}

QRectF
GW_Sliders_Pad::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), QSizeF ( _pad_size ) );
}

void
GW_Sliders_Pad::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;
}

void
GW_Sliders_Pad::set_pad_size (
	const QSize & size_n )
{
	if ( _pad_size != size_n ) {
		prepareGeometryChange();
		_pad_size = size_n;
		update_geometries();
	}
}

void
GW_Sliders_Pad::set_snd_controls (
	::QSnd2::Controls * controls_n )
{
	if ( _snd_controls != 0 ) {
		destroy_scene_items();
	}

	_snd_controls = controls_n;

	if ( _snd_controls != 0 ) {
		build_scene_items();
		update_geometries();
	}
}

void
GW_Sliders_Pad::set_panels_shift (
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
		update_panels_position();
	}
}

void
GW_Sliders_Pad::read_panels_shift ( )
{
	set_panels_shift ( _scrollbar->int_value() );
}

void
GW_Sliders_Pad::read_panels_shift_cb (
	void * context_n )
{
	::Wdg2::GW_Sliders_Pad & pad (
		*reinterpret_cast < ::Wdg2::GW_Sliders_Pad * > ( context_n ) );
	pad.read_panels_shift();
}

void
GW_Sliders_Pad::destroy_scene_items ( )
{
	if ( _group4 != 0 ) {
		_group4->setParentItem ( 0 );
		_group4.reset();
	}
	if ( _scrollbar != 0 ) {
		_scrollbar->setParentItem ( 0 );
		_scrollbar.reset();
	}
}

void
GW_Sliders_Pad::build_scene_items ( )
{
	if ( _snd_controls->num_groups() > 0 ) {
		_group4.reset (
			new ::Wdg2::GW_Group4 ( *_snd_controls->group ( 0 ), this ) );
	}
}

void
GW_Sliders_Pad::update_geometries ( )
{
	if ( _group4 != 0 ) {
		::Wdg2::GW_Group4_Sizes lsizes;
		lsizes.height = _pad_size.height();
		lsizes.slider_width = 16;
		lsizes.channels_hgap = 8;
		lsizes.group2_hgap = 16;
		lsizes.group3_hgap = 16;

		const unsigned int sbar_height ( 16 );
		const unsigned int sbar_hgap ( 2 );
		{
			unsigned int iwidth ( _group4->int_width_probe ( lsizes ) );
			if ( (int)iwidth > _pad_size.width() ) {
				lsizes.height = _pad_size.height();
				lsizes.height -= sbar_hgap + sbar_height;
				if ( _scrollbar == 0 ) {
					_scrollbar.reset ( new ::Wdg2::GW_Scrollbar ( this ) );
					_scrollbar->set_val_change_callback (
						::Context_Callback ( this, ::Wdg2::GW_Sliders_Pad::read_panels_shift_cb ) );
				}
			} else {
				if ( _scrollbar != 0 ) {
					_scrollbar.reset();
				}
			}
		}

		_group4->set_sizes ( lsizes );
		if ( _scrollbar != 0 ) {
			_panels_shift_max = _group4->int_width() - _pad_size.width();
			_scrollbar->setPos ( QPointF ( 0.0, lsizes.height + sbar_hgap ) );
			_scrollbar->set_size ( QSize ( _pad_size.width(), sbar_height ) );
			_scrollbar->set_int_span ( _panels_shift_max );
			_panels_shift = _scrollbar->int_value();
		} else {
			_panels_shift_max = 0;
			_panels_shift = 0;
		}

		update_panels_position();
	}
}

void
GW_Sliders_Pad::update_panels_position ( )
{
	if ( _group4 != 0 ) {
		_group4->setPos ( QPointF ( -double ( _panels_shift ), 0.0 ) );
	}
}


} // End of namespace
