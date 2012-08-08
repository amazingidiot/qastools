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

namespace Wdg2
{


GW_Label_Pixmaps::GW_Label_Pixmaps (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Pixmaps ( scene_db_n, 1, parent_n )
{
}

void
GW_Label_Pixmaps::set_text (
	const QString & text_n )
{
	_text = text_n;
	update_text_brect();
}

void
GW_Label_Pixmaps::set_font (
	const QFont & font_n )
{
	_font = font_n;
	update_text_brect();
}

void
GW_Label_Pixmaps::update_text_brect ( )
{
	const QFontMetrics fmet ( font() );
	_text_brect = fmet.boundingRect ( text() );
}

bool
GW_Label_Pixmaps::setup_request (
	unsigned int idx_n,
	::dpe2::Key_Values & kvals_n )
{
	bool res ( pxm_size_valid() && !text().isEmpty() );
	if ( res ) {
		double px ( -text_brect().x() );
		double py ( QFontMetrics ( font() ).ascent() );
		kvals_n.set_uint ( ::Wdg2::PRK_WIDTH,  pxm_size().width() );
		kvals_n.set_uint ( ::Wdg2::PRK_HEIGHT, pxm_size().height() );
		kvals_n.set_uint ( ::Wdg2::PRK_WIDGET_TYPE, ::Wdg2::WGT_LABEL );
		kvals_n.set_user ( ::Wdg2::PRK_FONT, new ::dpe2::UValue_Font ( font() ) );
		kvals_n.set_user ( ::Wdg2::PRK_TEXT, new ::dpe2::UValue_String ( text() ) );
		kvals_n.set_double ( ::Wdg2::PRK_TEXT_PX, px );
		kvals_n.set_double ( ::Wdg2::PRK_TEXT_PY, py );
	}
	return res;
}



GW_Label::GW_Label (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_pixmaps ( scene_db(), this ),
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
	const QSize & psize ( _pixmaps.text_brect().size() );
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
	const QRect brect ( fmet.boundingRect ( text() ) );
	return brect.size();
}

void
GW_Label::set_text (
	const QString & text_n )
{
	if ( text() != text_n ) {
		_pixmaps.set_text ( text_n );
		auto_set_size();
	}
}

void
GW_Label::set_font (
	const QFont & font_n )
{
	if ( font() != font_n ) {
		_pixmaps.set_font ( font_n );
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
	if ( alignment() != align_n ) {
		_alignment = align_n;
		update_pixmap();
	}
}

void
GW_Label::update_pixmap ( )
{
	const QFontMetrics fmet ( font() );
	const QRect brect ( fmet.boundingRect ( text() ) );

	int px;
	int py;
	{
		if ( ( alignment() & Qt::AlignLeft ) != 0 ) {
			px = 0;
		} else if ( ( alignment() & Qt::AlignRight ) != 0 ) {
			px = ( size().width() - brect.width() );
		} else {
			px = ( size().width() - brect.width() ) / 2;
		}

		if ( ( alignment() & Qt::AlignTop ) != 0 ) {
			py = 0;
		} else if ( ( alignment() & Qt::AlignBottom ) != 0 ) {
			py = ( size().height() - brect.height() );
		} else {
			py = ( size().height() - brect.height() ) / 2;
		}
	}

	QSize pxm_size ( brect.width(), fmet.height() );
	{
		if ( pxm_size.width() > size().width() ) {
			pxm_size.setWidth ( size().width() );
		}
		if ( pxm_size.height() > size().height() ) {
			pxm_size.setHeight ( size().height() );
		}
	}

	if ( _pixmaps.pxm_size() != pxm_size ) {
		_pixmaps.set_pxm_size ( pxm_size );
	} else {
		_pixmaps.repaint_pixmaps();
	}

	_pixmaps.setPos ( QPointF ( px, py ) );
}


} // End of namespace
