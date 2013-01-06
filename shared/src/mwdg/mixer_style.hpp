//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_mwdg_mixer_style_hpp__
#define __INC_mwdg_mixer_style_hpp__

#include "wdg/ds_widget_style_db.hpp"


namespace MWdg
{


class Mixer_Style
{
	// Public typedefs
	public:

	enum Style_Type {
		CUSTOM   = ::Wdg::DS_Widget_Style_Db::ST_USER,
		PLAYBACK = CUSTOM,
		CAPTURE  = CUSTOM + 1
	};


	// Public methods
	public:

	QPalette
	style_palette (
		::MWdg::Mixer_Style::Style_Type style_n );
};


} // End of namespace


#endif
