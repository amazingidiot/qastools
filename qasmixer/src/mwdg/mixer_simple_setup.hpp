//
// C++ Interface:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INC_mwdg_mixer_simple_setup_hpp__
#define __INC_mwdg_mixer_simple_setup_hpp__


// Forward declaration
namespace QSnd { class Mixer_Simple; }
namespace dpe { class Image_Allocator; }
namespace Wdg { class DS_Widget_Style_Db; }


namespace MWdg
{


/// @brief Mixer_Simple_Setup
///
class Mixer_Simple_Setup
{
	// Public methods
	public:

	Mixer_Simple_Setup ( );


	// Public attributes
	public:

	bool show_slider_value_labels;
	bool show_stream[2];

	::QSnd::Mixer_Simple * mixer_simple;

	const ::Wdg::DS_Widget_Style_Db * wdg_style_db;
	::dpe::Image_Allocator * image_alloc;
};


} // End of namespace


#endif
