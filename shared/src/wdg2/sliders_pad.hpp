//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_sliders_pad_hpp__
#define __INC_wdg2_sliders_pad_hpp__

#include <QWidget>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "qsnd2/controls.hpp"


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
		QWidget * parent_n = 0 );

	~Sliders_Pad ( );


	::QSnd2::Controls *
	snd_controls ( ) const;

	void
	set_snd_controls (
		::QSnd2::Controls * controls_n );


	// Public slots
	public slots:

	// Protected methods
	protected:


	// Protected slots
	protected slots:

	// Private methods
	private:

	void
	destroy_scene_items ( );

	void
	build_scene_items ( );


	// Private attributes
	private:

	::QSnd2::Controls * _snd_controls;
	QGraphicsScene _scene;
};


inline
::QSnd2::Controls *
Sliders_Pad::snd_controls ( ) const
{
	return _snd_controls;
}


} // End of namespace

#endif
