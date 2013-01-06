//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_gw_scroll_bar_hpp__
#define __INC_wdg2_gw_scroll_bar_hpp__

#include "flags.hpp"
#include "callbacks.hpp"
#include "gw_widget.hpp"
#include "gw_pixmaps.hpp"
#include "slider_value_map.hpp"
#include "gw_slider.hpp"

// Forward declaration
namespace Wdg2
{
	class GW_Scroll_Bar;
}

namespace Wdg2
{


/// @brief Button for the GW_Scroll_Bar
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

	::Wdg2::GW_Scroll_Bar *
	scrollbar ( ) const;

	bool
	is_forward ( ) const;


	// Protected methods
	protected:

	void
	update_pxm_idx ( );


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


/// @brief GW_Scrollbar_Rail
///
class GW_Scrollbar_Rail :
	public ::Wdg2::GW_Slider_Rail
{
	// Public methods
	public:

	GW_Scrollbar_Rail (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );
};


/// @brief GW_Scrollbar_Handle
///
class GW_Scrollbar_Handle :
	public ::Wdg2::GW_Slider_Handle
{
	// Public methods
	public:

	GW_Scrollbar_Handle (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );
};


/// @brief GW_Scroll_Bar
///
class GW_Scroll_Bar :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Scroll_Bar (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Scroll_Bar ( );


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
GW_Scroll_Bar::orientation ( ) const
{
	return _orientation;
}

inline
unsigned int
GW_Scroll_Bar::int_span ( ) const
{
	return _int_span;
}

inline
unsigned int
GW_Scroll_Bar::int_value ( ) const
{
	return _int_value;
}

inline
const ::Context_Callback &
GW_Scroll_Bar::val_change_callback ( ) const
{
	return _val_change_cb;
}

inline
float
GW_Scroll_Bar::anim_speed ( ) const
{
	return _anim_speed;
}


} // End of namespace

#endif
