//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "ds_widget_painter.hpp"

#include "dpe/image.hpp"
#include "dpe/image_set_meta.hpp"
#include <cstring>


namespace Wdg
{
namespace Painter
{


DS_Widget_Painter::DS_Widget_Painter (
	unsigned int group_type_n,
	unsigned int group_variant_n ) :
::dpe::Painter ( group_type_n, group_variant_n ),
_wdg_style_db ( 0 )
{
}


void
DS_Widget_Painter::set_wdg_style_db (
	::Wdg::DS_Widget_Style_Db * style_db_n )
{
	_wdg_style_db = style_db_n;
}


int
DS_Widget_Painter::create_image_data (
	::dpe::Image * img_n,
	const ::dpe::Image_Set_Meta * meta_n )
{
	if ( !meta_n->size.isValid() ) {
		return -1;
	}

	img_n->set_size (
		meta_n->size.width(),
		meta_n->size.height(),
		meta_n->size.width() * 4 );
	memset ( img_n->data(), 0, img_n->byte_count() ); // Clear

	return 0;
}


} // End of namespace
} // End of namespace

