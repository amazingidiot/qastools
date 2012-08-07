//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_slider_hpp__
#define __INC_wdg2_gw_slider_hpp__

#include "flags.hpp"
#include "callbacks.hpp"
#include "gw_widget.hpp"
#include "gw_pixmaps.hpp"
#include "slider_value_map.hpp"

namespace Wdg2
{


/// @brief GW_Slider_Rail
///
class GW_Slider_Rail :
	public ::Wdg2::GW_Pixmaps
{
	// Public methods
	public:

	GW_Slider_Rail (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );


	// Protected methods
	protected:

	void
	update_pxm_idx ( );

	bool
	setup_request (
		unsigned int idx_n,
		::dpe2::Key_Values & kvals_n );
};


/// @brief GW_Slider_Handle
///
class GW_Slider_Handle :
	public ::Wdg2::GW_Pixmaps
{
	// Public methods
	public:

	GW_Slider_Handle (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );


	// Protected methods
	protected:

	void
	update_pxm_idx ( );

	bool
	setup_request (
		unsigned int idx_n,
		::dpe2::Key_Values & kvals_n );
};


/// @brief GW_Slider_Sizes
///
struct GW_Slider_Sizes
{
	GW_Slider_Sizes ( );

	QSize size;
	unsigned int handle_length;
};

inline
GW_Slider_Sizes::GW_Slider_Sizes ( ) :
size ( 0, 0 ),
handle_length ( 0 )
{
}



/// @brief GW_Slider
///
class GW_Slider :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Slider (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Slider ( );


	/// @brief Determines the range of int_value()
	///
	::Wdg2::Slider_Value_Map *
	value_map ( ) const;

	void
	set_value_map (
		::Wdg2::Slider_Value_Map * map_n );


	/// @brief The current integer value
	///
	long
	int_value ( ) const;

	/// @brief Sets the int value -> slider position
	///
	/// The val_change_callback() won't be called by this,
	/// even if the value changed.
	void
	set_int_value (
		long value_n );


	/// @brief Gets called when the value changed
	///
	const ::Context_Callback &
	val_change_callback ( ) const;

	void
	set_val_change_callback (
		const ::Context_Callback & cb_n );


	const ::Wdg2::GW_Slider_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Slider_Sizes & sizes_n );


	/// @brief The slider orientation
	///
	Qt::Orientation
	orientation ( ) const;

	void
	set_orientation (
		Qt::Orientation orientation_n );


	// Protected methods
	protected:

	void
	update_handle_pos_from_value ( );


	void
	update_geometries ( );

	void
	set_handle_pos (
		unsigned int pos_n );

	void
	update_value_from_handle_pos ( );

	bool
	point_in_handle (
		const QPointF & point_n ) const;

	void
	move_handle (
		int amount_n );


	// Scene events

	void
	focusInEvent (
		QFocusEvent * event_n );

	void
	focusOutEvent (
		QFocusEvent * event_n );

	void
	mousePressEvent (
		QGraphicsSceneMouseEvent * event_n );

	void
	mouseReleaseEvent (
		QGraphicsSceneMouseEvent * event_n );

	void
	mouseMoveEvent (
		QGraphicsSceneMouseEvent * event_n );

	void
	wheelEvent (
		QGraphicsSceneWheelEvent * event_n );


	// Private attributes
	private:

	::Wdg2::GW_Slider_Sizes _sizes;

	Qt::Orientation _orientation;
	unsigned int _rail_span;
	unsigned int _handle_pos;
	unsigned int _handle_pos_span;

	long _int_value;
	::Wdg2::Slider_Value_Map * _value_map;

	::Context_Callback _val_change_cb;

	::Wdg2::GW_Slider_Rail _rail_wdg;
	::Wdg2::GW_Slider_Handle _handle_wdg;
};

inline
const ::Wdg2::GW_Slider_Sizes &
GW_Slider::sizes ( ) const
{
	return _sizes;
}

inline
Qt::Orientation
GW_Slider::orientation ( ) const
{
	return _orientation;
}

inline
::Wdg2::Slider_Value_Map *
GW_Slider::value_map ( ) const
{
	return _value_map;
}

inline
long
GW_Slider::int_value ( ) const
{
	return _int_value;
}


} // End of namespace

#endif
