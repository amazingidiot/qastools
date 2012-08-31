//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_sliders_pad_hpp__
#define __INC_wdg2_gw_sliders_pad_hpp__

#include "gw_widget.hpp"
#include "gw_scrollbar.hpp"
#include <QGraphicsItem>
#include <QScopedPointer>

// Forward declaration
namespace Wdg2 { class GW_SlPad_Group4; }
namespace QSnd2 { class Controls; }

namespace Wdg2
{


/// @brief GW_Sliders_Pad
///
class GW_Sliders_Pad :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Sliders_Pad (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Sliders_Pad ( );

	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	const QSize &
	pad_size ( ) const;

	void
	set_pad_size (
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
	QSize _pad_size;
	int _panels_shift;
	int _panels_shift_max;

	QScopedPointer < ::Wdg2::GW_SlPad_Group4 > _group4;
	QScopedPointer < ::Wdg2::GW_Scrollbar > _scrollbar;
};


inline
const QSize &
GW_Sliders_Pad::pad_size ( ) const
{
	return _pad_size;
}

inline
::QSnd2::Controls *
GW_Sliders_Pad::snd_controls ( ) const
{
	return _snd_controls;
}

inline
int
GW_Sliders_Pad::panels_shift ( )
{
	return _panels_shift;
}


} // End of namespace

#endif
