//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_graphics_view_hpp__
#define __INC_wdg2_graphics_view_hpp__

#include "scene_database.hpp"
#include <QGraphicsScene>
#include <QGraphicsView>

// Forward declarations
namespace Wdg2 {
	class GW_Widget;
}

namespace Wdg2
{

/// @brief Graphics_View
///
class Graphics_View :
	public QGraphicsView
{
	// Public methods
	public:

	Graphics_View (
		::Wdg2::Scene_Database * scene_db_n,
		QWidget * parent_n = 0 );

	~Graphics_View ( );


	::Wdg2::Scene_Database *
	scene_db ( ) const;


	::Wdg2::GW_Widget *
	widget ( ) const;

	void
	set_widget (
		::Wdg2::GW_Widget * widget_n );


	bool
	opengl_enabled ( ) const;

	void
	enable_opengl (
		bool flag_n );


	bool
	event (
		QEvent * event_n );

	static
	void
	pixmaps_finished_cb (
		void * context_n );


	// Protected methods
	protected:

	void
	resizeEvent (
		QResizeEvent * event_n );


	// Private methods
	private:

	void
	update_geometries ( );


	// Private attributes
	private:

	QGraphicsScene _scene;
	QScopedPointer < ::Wdg2::GW_Widget > _widget;
	::Wdg2::Scene_Database * _scene_db;

	int _etype_deliver_pixmaps;
	bool _opengl_enabled;
};

inline
::Wdg2::GW_Widget *
Graphics_View::widget ( ) const
{
	return _widget.data();
}

inline
::Wdg2::Scene_Database *
Graphics_View::scene_db ( ) const
{
	return _scene_db;
}


} // End of namespace

#endif
