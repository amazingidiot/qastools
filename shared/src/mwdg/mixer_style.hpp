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

#ifndef __INC_mwdg_mixer_style_hpp__
#define __INC_mwdg_mixer_style_hpp__

#include "wdg/ds_widget_style_db.hpp"


namespace MWdg
{


class Mixer_Style
{
	// Public typedefs
	public:

	enum STYLES {
		CUSTOM = ::Wdg::DS_Widget_Style_Db::ST_USER,
		PLAYBACK = CUSTOM,
		CAPTURE = CUSTOM + 1
	};
};


} // End of namespace


#endif
