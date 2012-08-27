//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_scrollbar_hpp__
#define __INC_wdg2_gw_scrollbar_hpp__

#include "flags.hpp"
#include "callbacks.hpp"
#include "gw_widget.hpp"
#include "gw_pixmaps.hpp"
#include "slider_value_map.hpp"
#include "gw_slider.hpp"

// Forward declaration
namespace Wdg2
{
	class GW_Scrollbar;
}

namespace Wdg2
{


/// @brief Button for the GW_Scrollbar
///
class GW_Scrollbar_Button :
	public ::Wdg2::GW_Pixmaps
{
	// Public methods
	public:

	GW_Scrollbar_Button (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Scrollbar_Button ( );

	::Wdg2::GW_Scrollbar *
	scrollbar ( ) const;

	bool
	is_forward ( ) const;


	// Protected methods
	protected:

	void
	update_pxm_idx ( );

	bool
	setup_pxm_request (
		unsigned int idx_n,
		::dpe2::Key_Values & kvals_n );


	void
	mousePressEvent (
		QGraphicsSceneMouseEvent * event_n );

	void
	mouseReleaseEvent (
		QGraphicsSceneMouseEvent * event_n );

	void
	wheelEvent (
		QGraphicsSceneWheelEvent * event_n );

};


/// @brief GW_Scrollbar
///
class GW_Scrollbar :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Scrollbar (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Scrollbar ( );


	const QSize &
	size ( ) const;

	void
	set_size (
		const QSize & size_n );


	Qt::Orientation
	orientation ( ) const;

	void
	set_orientation (
		Qt::Orientation orientation_n );


	/// @brief Highest available integer value
	///
	unsigned int
	int_span ( ) const;

	void
	set_int_span (
		unsigned int span_n );


	/// @brief The current integer value
	///
	unsigned int
	int_value ( ) const;

	void
	set_int_value (
		unsigned int value_n );


	/// @brief Gets called when the value changed
	///
	const ::Context_Callback &
	val_change_callback ( ) const;

	void
	set_val_change_callback (
		const ::Context_Callback & cb_n );


	/// @brief Slider widget callback mapper
	void
	read_slider_value ( );

	static
	void
	read_slider_value_cb (
		void * context_n );


	/// @brief Slider move speed in units / sec
	float
	anim_speed ( ) const;

	void
	set_anim_speed (
		float speed_n );

	/// @brief Animation callback
	static
	void
	move_animation_cb (
		void * context_n,
		unsigned int msec_n );

	void
	move_animation (
		unsigned int msec_n );


	void
	begin_move (
		bool forward_n );

	void
	end_move (
		bool forward_n );



	// Private methods
	private:

	void
	animation_start ( );

	void
	animation_stop ( );

	void
	animation_update ( );

	void
	update_geometries ( );


	// Private attributes
	private:

	QSize _size;
	Qt::Orientation _orientation;

	::Wdg2::Slider_Value_Map _value_map;
	unsigned int _int_span;
	unsigned int _int_value;
	::Context_Callback _val_change_cb;

	float _anim_speed; // in units / sec
	bool _anim_running;
	unsigned char _anim_forward;
	unsigned char _anim_backward;
	int _anim_micro_step;
	unsigned int _anim_timer_id;

	::Wdg2::GW_Scrollbar_Button _btn_low;
	::Wdg2::GW_Scrollbar_Button _btn_high;
	::Wdg2::GW_Slider _slider;
};

inline
Qt::Orientation
GW_Scrollbar::orientation ( ) const
{
	return _orientation;
}

inline
const QSize &
GW_Scrollbar::size ( ) const
{
	return _size;
}

inline
unsigned int
GW_Scrollbar::int_span ( ) const
{
	return _int_span;
}

inline
unsigned int
GW_Scrollbar::int_value ( ) const
{
	return _int_value;
}

inline
const ::Context_Callback &
GW_Scrollbar::val_change_callback ( ) const
{
	return _val_change_cb;
}

inline
float
GW_Scrollbar::anim_speed ( ) const
{
	return _anim_speed;
}


} // End of namespace

#endif
