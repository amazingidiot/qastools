//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_sliders_pad_hpp__
#define __INC_wdg2_gw_sliders_pad_hpp__

#include "gw_scrollbar.hpp"
#include <QGraphicsItem>
#include <QScopedPointer>

// Forward declaration
namespace Wdg2 { class GW_Group4; }
namespace QSnd2 { class Controls; }

namespace Wdg2
{


/// @brief GW_Sliders_Pad
///
class GW_Sliders_Pad :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Sliders_Pad (
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


	// Private methods
	private:

	void
	destroy_scene_items ( );

	void
	build_scene_items ( );

	void
	update_geometries ( );


	// Private attributes
	private:

	::QSnd2::Controls * _snd_controls;
	QSize _pad_size;

	QScopedPointer < ::Wdg2::GW_Group4 > _group4;
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


} // End of namespace

#endif
