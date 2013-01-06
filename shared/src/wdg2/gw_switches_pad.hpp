//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_gw_switches_pad_hpp__
#define __INC_wdg2_gw_switches_pad_hpp__

#include "gw_widget.hpp"
#include "gw_scroll_bar.hpp"
#include <QGraphicsItem>
#include <QScopedPointer>

// Forward declaration
namespace Wdg2 { class GW_SwPad_Group4; }
namespace QSnd2 { class Controls; }

namespace Wdg2
{


/// @brief GW_Switches_Pad
///
class GW_Switches_Pad :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Switches_Pad (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Switches_Pad ( );


	void
	set_size (
		const QSize & size_n );


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


	// Private methods
	private:

	void
	destroy_scene_items ( );

	void
	build_scene_items ( );

	void
	update_geometries ( );

	void
	update_panels_position ( );


	// Private attributes
	private:

	::QSnd2::Controls * _snd_controls;
	int _panels_shift;
	int _panels_shift_max;

	QScopedPointer < ::Wdg2::GW_SwPad_Group4 > _group4;
	QScopedPointer < ::Wdg2::GW_Scroll_Bar > _scrollbar;
};


inline
::QSnd2::Controls *
GW_Switches_Pad::snd_controls ( ) const
{
	return _snd_controls;
}

inline
int
GW_Switches_Pad::panels_shift ( )
{
	return _panels_shift;
}


} // End of namespace

#endif
