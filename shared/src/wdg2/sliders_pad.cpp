//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "sliders_pad.hpp"
#include "gw_groups.hpp"
#include "qsnd2/controls.hpp"
#include <QEvent>
#include <QKeyEvent>
#include <QFocusEvent>
#include <iostream>


namespace Wdg2
{


Sliders_Pad::Sliders_Pad (
	QWidget * parent_n ) :
QGraphicsView ( parent_n ),
_snd_controls ( 0 ),
_group4 ( 0 )
{
	setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );
	setAlignment ( Qt::AlignLeft | Qt::AlignTop );
	//setFrameStyle ( QFrame::NoFrame );
	//setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	setRenderHints ( QPainter::Antialiasing );
	setScene ( &_scene );
}

Sliders_Pad::~Sliders_Pad ( )
{
}

void
Sliders_Pad::set_snd_controls (
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
Sliders_Pad::update_geometries ( )
{
	if ( _group4 != 0 ) {
		::Wdg2::GW_Group4_Sizes lsizes;
		lsizes.height = viewport()->contentsRect().size().height();
		lsizes.slider_width = 16;
		lsizes.channels_hgap = 8;
		lsizes.group2_hgap = 16;
		lsizes.group3_hgap = 16;
		_group4->set_sizes ( lsizes );
	}
}

void
Sliders_Pad::destroy_scene_items ( )
{
	if ( _group4 != 0 ) {
		delete _group4;
		_group4 = 0;
	}
	_scene.clear();
}

void
Sliders_Pad::build_scene_items ( )
{
	if ( _snd_controls->num_groups() > 0 ) {
		_group4 = new ::Wdg2::GW_Group4 ( *_snd_controls->group ( 0 ), 0 );
		update_geometries();
		_scene.addItem ( _group4 );
	}
}

void
Sliders_Pad::resizeEvent (
	QResizeEvent * event_n )
{
	QGraphicsView::resizeEvent ( event_n );
	update_geometries();

	{
		QRectF srect ( _scene.itemsBoundingRect() );

		// TODO: clean
		/*
		QRectF vprect ( viewport()->rect() );
		QRectF vpcrect ( viewport()->contentsRect() );
		QRectF prect ( rect() );
		QRectF pcrect ( contentsRect() );

		::std::cout << "scene rect  " << srect.width() << "x" << srect.height() << " @ " << srect.x() << ":" << srect.y() << "\n";
		::std::cout << "vport prect " << vprect.width() << "x" << vprect.height() << " @ " << vprect.x() << ":" << vprect.y() << "\n";
		::std::cout << "vport crect " << vpcrect.width() << "x" << vpcrect.height() << " @ " << vpcrect.x() << ":" << vpcrect.y() << "\n";
		::std::cout << "prect  " << prect.width() << "x" << prect.height() << " @ " << prect.x() << ":" << prect.y() << "\n";
		::std::cout << "pcrect " << pcrect.width() << "x" << pcrect.height() << " @ " << pcrect.x() << ":" << pcrect.y() << "\n";
		*/

		if ( !srect.isValid() ){
			srect.setRect ( 0.0, 0.0, 1.0, 1.0 );
		}
		_scene.setSceneRect ( srect );
	}
}


} // End of namespace
