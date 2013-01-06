//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_gw_scroll_area_hpp__
#define __INC_wdg2_gw_scroll_area_hpp__

#include "gw_widget.hpp"
#include "gw_scroll_bar.hpp"
#include <QScopedPointer>

namespace Wdg2
{


/// @brief GW_Scroll_Area
///
class GW_Scroll_Area :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Scroll_Area (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Scroll_Area ( );


	void
	set_size (
		const QSize & size_n );


	::Wdg2::GW_Widget *
	widget ( ) const;

	void
	set_widget (
		::Wdg2::GW_Widget * widget_n );

	::Wdg2::GW_Widget *
	take_widget ( );


	Qt::Orientation
	scroll_orientation ( ) const;

	void
	set_scroll_orientation (
		Qt::Orientation orientation_n );


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

	/// @return If 0 scrollbar will be disabled, if greater than 0
	///         it is taken as the widget length in the scroll orientation.
	/// @param size_off_n viewport size with scrollbar disabled
	/// @param size_on_n viewport size with scrollbar enabled
	virtual
	unsigned int
	viewport_resize (
		QSize size_off_n,
		QSize size_on_n );


	// Private methods
	private:

	void
	update_geometries ( );

	void
	update_widget_position ( );


	// Private attributes
	private:

	Qt::Orientation _scroll_orientation;
	int _panels_shift;
	int _panels_shift_max;
	::Wdg2::GW_Widget _viewport;
	QScopedPointer < ::Wdg2::GW_Scroll_Bar > _scrollbar;
	::Wdg2::GW_Widget * _widget;
};

inline
Qt::Orientation
GW_Scroll_Area::scroll_orientation ( ) const
{
	return _scroll_orientation;
}

inline
int
GW_Scroll_Area::panels_shift ( )
{
	return _panels_shift;
}

inline
::Wdg2::GW_Widget *
GW_Scroll_Area::widget ( ) const
{
	return _widget;
}



} // End of namespace

#endif
