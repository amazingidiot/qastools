//
// C++ Interface:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INC_mixer_window_hpp__
#define __INC_mixer_window_hpp__

#include "config.hpp"
#include "mixer_window_setup.hpp"

#include <QAction>
#include <QActionGroup>
#include <QMainWindow>
#include <QDockWidget>

// Forward declaration
namespace Views {
	class Mixer_Simple;
	class Dev_Select_View;
}


class Mixer_Window :
	public QMainWindow
{
	Q_OBJECT

	// Public methods
	public:

	Mixer_Window (
		QWidget * parent_n = 0,
		Qt::WindowFlags flags_n = 0 );

	~Mixer_Window ( );


	void
	set_mixer_setup (
		Mixer_Window_Setup * setup_n );

	void
	select_ctl (
		const QString & ctl_n );

	bool
	eventFilter (
		QObject * obj_n,
		QEvent * event_n );


	// Signals
	signals:

	void
	sig_show_settings ( );

	void
	sig_show_info ( );

	void
	sig_control_changed ( );


	void
	sig_quit ( );


	// Public slots
	public slots:

	// Snd control selection

	void
	select_ctl_from_side_iface ( );

	void
	reload_mixer_device ( );

	void
	reload_mixer_inputs ( );

	void
	reload_mixer_view ( );


	/// @brief Sets/unsets fullscreen mode
	///
	void
	set_fullscreen (
		bool flag_n );


	// Device selection

	void
	show_device_selection (
		bool flag_n );

	void
	toggle_device_selection ( );


	/// @brief Save state to the setup tree
	///
	void
	save_state ( );


	// Protected methods
	protected:

	void
	update_fullscreen_action ( );


	// Event handlers

	void
	changeEvent (
		QEvent * event_n );

	void
	keyPressEvent (
		QKeyEvent * event_n );


	void
	init_menus ( );

	void
	init_docks ( );


	// Private attributes
	private:

	Mixer_Window_Setup * _mixer_setup;

	// Base widgets
	::Views::Mixer_Simple * _mixer_simple;
	::Views::Dev_Select_View * _dev_select;
	QDockWidget * _dev_select_dock;

	// Menubar
	QMenu * _menu_mixer;
	QAction * _act_show_dev_select;
	QAction * _act_fullscreen;

	// Strings and icons
	QString _str_fscreen_enable;
	QString _str_fscreen_disable;
	QIcon _icon_fscreen_enable;
	QIcon _icon_fscreen_disable;
};


#endif
