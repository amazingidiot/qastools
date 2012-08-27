//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_label.hpp"
#include "theme_painters.hpp"
#include <QFontMetrics>
#include <iostream>
#include <sstream>

namespace Wdg2
{


GW_Label_Ground::GW_Label_Ground (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Pixmaps ( scene_db_n, 1, parent_n )
{
	pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_TYPE, ::Wdg2::WGT_LABEL );
	pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_PART, ::Wdg2::WGP_LABEL_GROUND );
	update_text_brect();
}

void
GW_Label_Ground::set_text (
	const QString & text_n )
{
	_text = text_n;
	update_text_brect();
}

void
GW_Label_Ground::set_font (
	const QFont & font_n )
{
	_font = font_n;
	update_text_brect();
}

QRect
GW_Label_Ground::calc_brect (
	const QFontMetrics & fmet_n,
	const QString & text_n )
{
	QRect brect ( fmet_n.boundingRect ( text_n ) );
	{
		int twidth ( fmet_n.width ( text_n ) );
		twidth -= _text_brect.x();
		if ( twidth > _text_brect.width() ) {
			brect.setWidth ( twidth );
		}
	}
	return brect;
}

void
GW_Label_Ground::update_text_brect ( )
{
	const QFontMetrics fmet ( font() );
	_text_brect = calc_brect ( fmet, text() );
	_font_height = fmet.height();
}

bool
GW_Label_Ground::setup_pxm_request (
	unsigned int idx_n,
	::dpe2::Key_Values & kvals_n )
{
	bool res ( !text().isEmpty() &&
		::Wdg2::GW_Pixmaps::setup_pxm_request ( idx_n, kvals_n ) );
	if ( res ) {
		double pos_x ( -text_brect().x() );
		double pos_y ( QFontMetrics ( font() ).ascent() );
		kvals_n.set_double ( ::Wdg2::PRK_TEXT_POS_X, pos_x );
		kvals_n.set_double ( ::Wdg2::PRK_TEXT_POS_Y, pos_y );
		kvals_n.set_user ( ::Wdg2::PRK_FONT, new ::dpe2::UValue_Font ( font() ) );
		kvals_n.set_user ( ::Wdg2::PRK_TEXT, new ::dpe2::UValue_String ( text() ) );
	}
	return res;
}



GW_Label::GW_Label (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_ground ( scene_db(), this ),
_alignment ( Qt::AlignLeft | Qt::AlignVCenter )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );
}

void
GW_Label::set_size (
	const QSize & size_n )
{
	if ( _size != size_n ) {
		_size = size_n;
		set_bounding_rect ( _size );
		update_pixmap();
	}
}

void
GW_Label::auto_set_size ( )
{
	const QSize psize (
		_ground.text_brect().width(),
		_ground.font_height() );
	if ( size() != psize ) {
		set_size ( psize );
	} else {
		update_pixmap();
	}
}

QSize
GW_Label::probe_size (
	const QString & text_n )
{
	const QFontMetrics fmet ( font() );
	const QRect brect ( _ground.calc_brect ( fmet, text_n ) );
	return QSize ( brect.width(), fmet.height() );
}

void
GW_Label::set_text (
	const QString & text_n )
{
	if ( text() != text_n ) {
		_ground.set_text ( text_n );
		auto_set_size();
	}
}

void
GW_Label::set_font (
	const QFont & font_n )
{
	if ( font() != font_n ) {
		_ground.set_font ( font_n );
		auto_set_size();
	}
}

void
GW_Label::set_alignment (
	Qt::Alignment align_n )
{
	const Qt::Alignment halign ( Qt::AlignLeft | Qt::AlignRight | Qt::AlignHCenter );
	if ( ( align_n & halign ) == 0 ) {
		align_n |= ( alignment() & halign );
	}
	const Qt::Alignment valign ( Qt::AlignTop | Qt::AlignBottom | Qt::AlignVCenter );
	if ( ( align_n & valign ) == 0 ) {
		align_n |= ( alignment() & valign );
	}
	if ( _alignment != align_n ) {
		_alignment = align_n;
		update_pixmap();
	}
}

void
GW_Label::update_pixmap ( )
{
	const QRect & brect ( _ground.text_brect() );

	int pos_x;
	int pos_y;
	{
		if ( ( alignment() & Qt::AlignLeft ) != 0 ) {
			pos_x = 0;
		} else if ( ( alignment() & Qt::AlignRight ) != 0 ) {
			pos_x = ( size().width() - brect.width() );
		} else {
			pos_x = ( size().width() - brect.width() ) / 2;
		}

		if ( ( alignment() & Qt::AlignTop ) != 0 ) {
			pos_y = 0;
		} else if ( ( alignment() & Qt::AlignBottom ) != 0 ) {
			pos_y = ( size().height() - brect.height() );
		} else {
			pos_y = ( size().height() - brect.height() ) / 2;
		}
	}

	QSize pxm_size ( brect.width(), _ground.font_height() );
	{
		if ( pxm_size.width() > size().width() ) {
			pxm_size.setWidth ( size().width() );
		}
		if ( pxm_size.height() > size().height() ) {
			pxm_size.setHeight ( size().height() );
		}
	}

	if ( _ground.pxm_size() != pxm_size ) {
		_ground.set_pxm_size ( pxm_size );
	} else {
		_ground.repaint_pixmaps();
	}

	_ground.setPos ( QPointF ( pos_x, pos_y ) );
}


} // End of namespace
