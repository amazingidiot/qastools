//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_gw_sliders_pad_hpp__
#define __INC_wdg2_gw_sliders_pad_hpp__

#include "gw_scroll_area.hpp"
#include <QScopedPointer>

// Forward declarations
namespace Wdg2 {
	class GW_SlPad_Group4;
}
namespace QSnd2 {
	class Controls;
}

namespace Wdg2
{


/// @brief GW_Sliders_Pad
///
class GW_Sliders_Pad :
	public ::Wdg2::GW_Scroll_Area
{
	// Public methods
	public:

	GW_Sliders_Pad (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Sliders_Pad ( );


	::QSnd2::Controls *
	snd_controls ( ) const;

	void
	set_snd_controls (
		::QSnd2::Controls * controls_n );


	int
	panels_shift ( );

	void
	set_panels_shift (
		long amount_n );

	void
	read_panels_shift ( );

	static
	void
	read_panels_shift_cb (
		void * context_n );


	// Protected methods
	protected:

	unsigned int
	viewport_resize (
		QSize size_off_n,
		QSize size_on_n );


	// Private methods
	private:

	void
	destroy_items ( );

	void
	build_items ( );


	// Private attributes
	private:

	::QSnd2::Controls * _snd_controls;
	QScopedPointer < ::Wdg2::GW_SlPad_Group4 > _group4;
};


inline
::QSnd2::Controls *
GW_Sliders_Pad::snd_controls ( ) const
{
	return _snd_controls;
}


} // End of namespace

#endif
