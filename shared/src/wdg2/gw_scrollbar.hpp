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
#include "graphical_widget.hpp"
#include "slider_value_map.hpp"
#include "gw_slider.hpp"
#include <QGraphicsItem>

namespace Wdg2
{


/// @brief Button for the GW_Scrollbar
///
class GW_Scrollbar_Button :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Scrollbar_Button (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );


	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	void
	set_size (
		const QSize & size_n );

	Qt::Orientation
	orientation ( ) const;


	// Private attributes
	private:

	QSize _size;
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


	void
	read_slider_value ( );

	static
	void
	read_slider_value_cb (
		void * context_n );


	// Private methods
	private:

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


} // End of namespace

#endif
