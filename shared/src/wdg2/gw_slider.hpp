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
#include "gw_widget_element.hpp"
#include "slider_value_map.hpp"
#include <QScopedPointer>

namespace Wdg2
{



/// @brief GW_Slider_Settings
///
struct GW_Slider_Settings
{
	GW_Slider_Settings ( );

	QSize size;
	unsigned int handle_length;
	Qt::Orientation orientation;
};

inline
GW_Slider_Settings::GW_Slider_Settings ( ) :
size ( 0, 0 ),
handle_length ( 0 ),
orientation ( Qt::Vertical )
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

	GW_Slider (
		::Wdg2::Scene_Database * scene_db_n,
		::Wdg2::GW_Widget_Element * rail_n,
		::Wdg2::GW_Widget_Element * handle_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Slider ( );


	/// @brief Contains the allowed values of int_value()
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


	const ::Wdg2::GW_Slider_Settings &
	settings ( ) const;

	void
	load_settings (
		const ::Wdg2::GW_Slider_Settings & settings_n );


	/// @brief The slider orientation
	///
	Qt::Orientation
	orientation ( ) const;

	void
	set_orientation (
		Qt::Orientation orientation_n );


	// Protected methods
	protected:

	// Geometries

	void
	update_geometries ( );


	// Handle manipulation

	void
	set_handle_pos (
		unsigned int pos_n );

	void
	update_value_from_handle_pos ( );

	void
	update_handle_pos_from_value ( );

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


	// Private methods
	private:

	void
	init (
		::Wdg2::GW_Widget_Element * rail_n = 0,
		::Wdg2::GW_Widget_Element * handle_n = 0 );


	// Private attributes
	private:

	::Wdg2::GW_Slider_Settings _settings;
	unsigned int _rail_span;
	unsigned int _handle_pos;
	unsigned int _handle_pos_span;

	long _int_value;
	::Wdg2::Slider_Value_Map * _value_map;
	::Context_Callback _val_change_cb;

	QScopedPointer < ::Wdg2::GW_Widget_Element > _rail;
	QScopedPointer < ::Wdg2::GW_Widget_Element > _handle;
};

inline
const ::Wdg2::GW_Slider_Settings &
GW_Slider::settings ( ) const
{
	return _settings;
}

inline
Qt::Orientation
GW_Slider::orientation ( ) const
{
	return _settings.orientation;
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




/// @brief GW_Slider_Rail
///
class GW_Slider_Rail :
	public ::Wdg2::GW_Widget_Element_Pixmaps
{
	// Public methods
	public:

	GW_Slider_Rail (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );


	// Protected methods
	protected:

	void
	state_flags_changed ( );
};


/// @brief GW_Slider_Handle
///
class GW_Slider_Handle :
	public ::Wdg2::GW_Widget_Element_Pixmaps
{
	// Public methods
	public:

	GW_Slider_Handle (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );


	// Protected methods
	protected:

	void
	state_flags_changed ( );
};


} // End of namespace

#endif
