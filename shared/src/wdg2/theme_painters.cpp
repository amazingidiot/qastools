//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "theme_painters.hpp"
#include <iostream>
#include <sstream>

namespace Wdg2
{


bool
Theme_Painter_Label::is_responsible (
	const ::dpe2::Key_Values & vset_n )
{
	bool vgood ( true );
	const unsigned int wgt (
		vset_n.val_uint ( ::Wdg2::PRK_WIDGET_TYPE, &vgood ) );
	if ( wgt != ::Wdg2::WGT_LABEL ) {
		vgood = false;
	}
	return vgood;
}

void
Theme_Painter_Label::paint (
	::dpe2::Pixmap & pxmap_n,
	const ::dpe2::Key_Values & kvals_n )
{
	const unsigned int iwidth ( kvals_n.val_uint ( ::Wdg2::PRK_WIDTH ) );
	const unsigned int iheight ( kvals_n.val_uint ( ::Wdg2::PRK_HEIGHT ) );
	::dpe2::UValue_String * ustr (
		dynamic_cast < ::dpe2::UValue_String * > (
		kvals_n.val_user ( ::Wdg2::PRK_TEXT ) ) );
	::dpe2::UValue_Font * ufont (
		dynamic_cast < ::dpe2::UValue_Font * > (
		kvals_n.val_user ( ::Wdg2::PRK_FONT ) ) );
	const double pos_x ( kvals_n.val_double ( ::Wdg2::PRK_TEXT_POS_X ) );
	const double pos_y ( kvals_n.val_double ( ::Wdg2::PRK_TEXT_POS_Y ) );

	bool vgood ( true );
	vgood = vgood && valid_size ( iwidth, iheight );
	vgood = vgood && ( ustr != 0 );
	vgood = vgood && ( ufont != 0 );

	if ( vgood ) {
		pxmap_n.set_size ( iwidth, iheight );
		pxmap_n.qimage().fill ( 0 );
		{
			QPainter pnt ( &pxmap_n.qimage() );
			pnt.setRenderHints ( QPainter::Antialiasing | QPainter::TextAntialiasing );
			{
				QPen ppen ( qpalette().color ( QPalette::WindowText ) );
				pnt.setPen ( ppen );
			}
			pnt.setBrush ( Qt::NoBrush );
			pnt.setFont ( ufont->font() );
			pnt.drawText ( QPointF ( pos_x, pos_y ), ustr->str() );
		}
	}
}



bool
Theme_Painter_Switch::is_responsible (
	const ::dpe2::Key_Values & vset_n )
{
	bool vgood ( true );
	const unsigned int wgt (
		vset_n.val_uint ( ::Wdg2::PRK_WIDGET_TYPE, &vgood ) );
	if ( wgt != ::Wdg2::WGT_SWITCH ) {
		vgood = false;
	}
	return vgood;
}

void
Theme_Painter_Switch::paint (
	::dpe2::Pixmap & pxmap_n,
	const ::dpe2::Key_Values & kvals_n )
{
	bool vgood ( true );
	const unsigned int part (
		kvals_n.val_uint ( ::Wdg2::PRK_WIDGET_PART , &vgood ) );
	if ( vgood ) {
		switch ( part ) {
			case ::Wdg2::WGP_SWITCH_GROUND:
				this->paint_ground ( pxmap_n, kvals_n );
				break;
			case ::Wdg2::WGP_SWITCH_HANDLE:
				this->paint_handle ( pxmap_n, kvals_n );
				break;
			default:
				break;
		}
	}
}



bool
Theme_Painter_Slider::is_responsible (
	const ::dpe2::Key_Values & vset_n )
{
	bool vgood ( true );
	const unsigned int wgt (
		vset_n.val_uint ( ::Wdg2::PRK_WIDGET_TYPE, &vgood ) );
	if ( wgt != ::Wdg2::WGT_SLIDER ) {
		vgood = false;
	}
	return vgood;
}

void
Theme_Painter_Slider::paint (
	::dpe2::Pixmap & pxmap_n,
	const ::dpe2::Key_Values & kvals_n )
{
	bool vgood ( true );
	const unsigned int part (
		kvals_n.val_uint ( ::Wdg2::PRK_WIDGET_PART , &vgood ) );
	if ( vgood ) {
		switch ( part ) {
			case ::Wdg2::WGP_SLIDER_RAIL:
				this->paint_rail ( pxmap_n, kvals_n );
				break;
			case ::Wdg2::WGP_SLIDER_HANDLE:
				this->paint_handle ( pxmap_n, kvals_n );
				break;
			default:
				break;
		}
	}
}




bool
Theme_Painter_Scrollbar::is_responsible (
	const ::dpe2::Key_Values & vset_n )
{
	bool vgood ( true );
	const unsigned int wgt (
		vset_n.val_uint ( ::Wdg2::PRK_WIDGET_TYPE, &vgood ) );
	if ( wgt != ::Wdg2::WGT_SCROLLBAR ) {
		vgood = false;
	}
	return vgood;
}

void
Theme_Painter_Scrollbar::paint (
	::dpe2::Pixmap & pxmap_n,
	const ::dpe2::Key_Values & kvals_n )
{
	bool vgood ( true );
	const unsigned int part (
		kvals_n.val_uint ( ::Wdg2::PRK_WIDGET_PART , &vgood ) );
	if ( vgood ) {
		switch ( part ) {
			case ::Wdg2::WGP_SCROLLBAR_RAIL:
				this->paint_rail ( pxmap_n, kvals_n );
				break;
			case ::Wdg2::WGP_SCROLLBAR_HANDLE:
				this->paint_handle ( pxmap_n, kvals_n );
				break;
			case ::Wdg2::WGP_SCROLLBAR_BTN_LEFT:
				this->paint_button ( pxmap_n, kvals_n, false, false );
				break;
			case ::Wdg2::WGP_SCROLLBAR_BTN_BOTTOM:
				this->paint_button ( pxmap_n, kvals_n, false, true );
				break;
			case ::Wdg2::WGP_SCROLLBAR_BTN_RIGHT:
				this->paint_button ( pxmap_n, kvals_n, true, false );
				break;
			case ::Wdg2::WGP_SCROLLBAR_BTN_TOP:
				this->paint_button ( pxmap_n, kvals_n, true, true );
				break;
			default:
				break;
		}
	}
}


} // End of namespace
