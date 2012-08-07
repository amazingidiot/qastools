//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_mv_mixer_simple_hpp__
#define __INC_mv_mixer_simple_hpp__

#include <QSplitter>
#include <QPushButton>
#include <QPointer>

#include "mwdg/mixer_simple_setup.hpp"
#include "views/view_base.hpp"

#include "qsnd2/controls_watcher.hpp"
#include "qsnd2/alsa_smi.hpp"
#include "wdg2/sliders_pad.hpp"


// Forward declaration
namespace Views {
	class Mixer_Simple_Setup;
}


namespace Views
{


class Mixer_Simple :
	public ::Views::View_Base
{
	Q_OBJECT

	// Public methods
	public:

	Mixer_Simple (
		QWidget * parent_n = 0 );

	~Mixer_Simple ( );


	// Mixer device and view setup

	void
	set_mdev_setup (
		const ::MWdg::Mixer_Device_Setup * setup_n );

	void
	set_inputs_setup (
		const ::MWdg::Inputs_Setup * setup_n );

	void
	set_view_setup (
		::Views::View_Base_Setup * setup_n );


	// Public slots
	public slots:

	void
	show_playback (
		bool flag_n );

	void
	show_capture (
		bool flag_n );

	void
	toggle_show_playback ( );

	void
	toggle_show_capture ( );


	// Protected methods
	protected:

	void
	keyPressEvent (
		QKeyEvent * event_n );


	// Private methods
	private:

	void
	clear_view ( );

	void
	setup_view ( );


	// Private attributes
	private:

	::Views::Mixer_Simple_Setup * _view_setup;
	::MWdg::Mixer_Simple_Setup _smixer_setup;

	// widgets
	QWidget _wdg_areas;
	QSplitter _mixer_split;
	::Wdg2::Scene_Database * _scene_db;
	::Wdg2::Sliders_Pad * _sliders_pad2;

	unsigned int _status_group_idx;
	unsigned int _status_column_idx;

	::QSnd2::ASMI_Controls * _snd_controls;
	::QSnd2::Controls_Watcher _snd_ctl_watcher;

	// Strings and Icons
	QString _act_stream_text[2];
	QString _act_stream_ttip[2];
	QIcon _act_stream_icon[2];
};


} // End of namespace

#endif
