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
_pad_size ( 0, 0 )
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
	if ( size_n != _pad_size ) {
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
	}
}

void
GW_Sliders_Pad::destroy_scene_items ( )
{
	_group4.reset();
}

void
GW_Sliders_Pad::build_scene_items ( )
{
	if ( _snd_controls->num_groups() > 0 ) {
		_group4.reset (
			new ::Wdg2::GW_Group4 ( *_snd_controls->group ( 0 ), this ) );
		_group4->setPos ( QPointF ( 0.0, 0.0 ) );
		update_geometries();
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

		{
			unsigned int sbar_height ( 16 );
			unsigned int sbar_hgap ( 2 );
			unsigned int iwidth ( _group4->int_width_probe ( lsizes ) );
			if ( (int)iwidth > _pad_size.width() ) {
				lsizes.height = _pad_size.height();
				lsizes.height -= sbar_hgap;
				lsizes.height -= sbar_height;

				_scrollbar.reset ( new ::Wdg2::GW_Scrollbar ( this ) );
				_scrollbar->set_size ( QSize ( _pad_size.width(), sbar_height ) );
				_scrollbar->setPos ( QPointF ( 0.0, lsizes.height + sbar_hgap ) );
			} else {
				_scrollbar.reset();
			}
		}

		_group4->set_sizes ( lsizes );
	}
}


} // End of namespace
