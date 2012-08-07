//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "theme_sbevel.hpp"
#include "theme_painters.hpp"
#include <iostream>

namespace Wdg2
{


class Painter_SBevel_Slider :
	public ::Wdg2::Theme_Painter_Slider
{
	// Public methods
	public:

	void
	paint_handle (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n );

	void
	paint_rail (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n );
};

void
Painter_SBevel_Slider::paint_handle (
	::dpe2::Pixmap & pxmap_n,
	const ::dpe2::Key_Values & kvals_n )
{
	const unsigned int sflags (
		kvals_n.val_uint ( ::Wdg2::PRK_WIDGET_STATE_FLAGS ) );

	double pen_width ( 2.0 );
	double pwhalf ( pen_width / 2.0 );
	double crad ( 2.0 );
	QRectF prect ( 0.0, 0.0, pxmap_n.width(), pxmap_n.height() );
	prect.adjust ( pwhalf, pwhalf, -pwhalf, -pwhalf );

	pxmap_n.qimage().fill ( 0 );
	{
		QPainter pnt ( &pxmap_n.qimage() );
		pnt.setRenderHints ( QPainter::Antialiasing );
		{
			QPen ppen ( Qt::SolidLine );
			ppen.setWidth ( pen_width );
			ppen.setColor ( qpalette().color ( QPalette::ButtonText ) );
			pnt.setPen ( ppen );
		}
		{
			QBrush pbrush ( Qt::SolidPattern );
			pbrush.setColor ( qpalette().color ( QPalette::Button ) );
			pnt.setBrush ( qpalette().color ( QPalette::Button ) );
		}
		pnt.drawRoundedRect ( prect, crad, crad );
	}
}

void
Painter_SBevel_Slider::paint_rail (
	::dpe2::Pixmap & pxmap_n,
	const ::dpe2::Key_Values & kvals_n )
{
	unsigned int sflags (
		kvals_n.val_uint ( ::Wdg2::PRK_WIDGET_STATE_FLAGS ) );

	double pen_width ( 1.0 );
	if ( ( sflags & ::Wdg2::GW_HAS_FOCUS ) ) {
		pen_width = 2;
	}
	double crad ( 2.0 );
	QRectF prect ( 0.0, 0.0, pxmap_n.width(), pxmap_n.height() );
	double adjx ( pen_width / 2.0 + 3.0 );
	double adjy ( pen_width / 2.0 + 3.0 );
	prect.adjust ( adjx, adjy, -adjx, -adjy );

	pxmap_n.qimage().fill ( 0 );
	{
		QPainter pnt ( &pxmap_n.qimage() );
		pnt.setRenderHints ( QPainter::Antialiasing );
		{
			QPen ppen ( Qt::SolidPattern );
			ppen.setColor ( qpalette().color ( QPalette::ButtonText ) );
			ppen.setWidth ( pen_width );
			pnt.setPen ( ppen );
		}
		{
			QBrush pbrush ( Qt::SolidPattern );
			pbrush.setColor ( qpalette().color ( QPalette::Button ) );
			pnt.setBrush ( qpalette().color ( QPalette::Button ) );
		}
		pnt.drawRoundedRect ( prect, crad, crad );
	}
}


Theme_SBevel::Theme_SBevel ( )
{
	add_painter ( new Painter_SBevel_Slider );
}


} // End of namespace
