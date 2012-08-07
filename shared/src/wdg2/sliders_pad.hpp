//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_sliders_pad_hpp__
#define __INC_wdg2_sliders_pad_hpp__

#include "scene_database.hpp"
#include "gw_sliders_pad.hpp"
#include <QGraphicsScene>
#include <QGraphicsView>

namespace Wdg2
{


/// @brief Sliders_Pad
///
class Sliders_Pad :
	public QGraphicsView
{
	Q_OBJECT

	// Public methods
	public:

	Sliders_Pad (
		::Wdg2::Scene_Database * scene_db_n,
		QWidget * parent_n = 0 );

	~Sliders_Pad ( );


	::QSnd2::Controls *
	snd_controls ( ) const;

	void
	set_snd_controls (
		::QSnd2::Controls * controls_n );


	bool
	opengl_enabled ( ) const;

	void
	enable_opengl (
		bool flag_n );

	::Wdg2::Scene_Database *
	scene_db ( ) const;


	bool
	event (
		QEvent * event_n );

	static
	void
	notify_pixmaps_finished (
		void * context_n );


	// Public slots
	public slots:


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

	::Wdg2::Scene_Database * _scene_db;
	::Wdg2::GW_Sliders_Pad _gw_sliders_pad;
	QGraphicsScene _scene;

	int _etype_deliver_pixmaps;
	bool _opengl_enabled;
};


inline
::QSnd2::Controls *
Sliders_Pad::snd_controls ( ) const
{
	return _gw_sliders_pad.snd_controls();
}

inline
::Wdg2::Scene_Database *
Sliders_Pad::scene_db ( ) const
{
	return _scene_db;
}


} // End of namespace

#endif
