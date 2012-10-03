//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_mixer_pad_hpp__
#define __INC_wdg2_gw_mixer_pad_hpp__

#include "gw_widget.hpp"
#include "gw_sliders_pad.hpp"
#include "gw_switches_pad.hpp"

// Forward declarations
namespace QSnd2 { class Controls; }

namespace Wdg2
{


/// @brief GW_Mixer_Pad
///
class GW_Mixer_Pad :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Mixer_Pad (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Mixer_Pad ( );


	void
	set_size (
		const QSize & size_n );


	::QSnd2::Controls *
	snd_controls ( ) const;

	void
	set_snd_controls (
		::QSnd2::Controls * controls_n );


	// Private methods
	private:

	void
	update_geometries ( );


	// Private attributes
	private:

	::QSnd2::Controls * _snd_controls;
	::Wdg2::GW_Sliders_Pad _gw_sliders_pad;
	::Wdg2::GW_Switches_Pad _gw_switches_pad;
};


inline
::QSnd2::Controls *
GW_Mixer_Pad::snd_controls ( ) const
{
	return _gw_sliders_pad.snd_controls();
}


} // End of namespace

#endif
