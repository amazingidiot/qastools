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

#ifndef __INC_main_window_hpp__
#define __INC_main_window_hpp__

#include "dpe/image_allocator.hpp"
#include "wdg/ds_widget_style_db.hpp"
#include "views/mixer_hctl.hpp"
#include "main_window_setup.hpp"

#include <QMainWindow>
#include <QDialog>
#include <QFileSystemWatcher>
#include <QPointer>
#include <QSharedPointer>


// Forward declaration
namespace Views { class Mixer_HCTL; }
namespace Views { class Dev_Select_View; }


/// @brief Main_Window
///
class Main_Window :
	public QMainWindow
{
	Q_OBJECT

	// Public methods
	public:

	Main_Window ( );


	bool
	eventFilter (
		QObject * obj_n,
		QEvent * event_n );


	// Public slots
	public slots:

	void
	restore_state ( );

	void
	save_state ( );

	void
	select_snd_ctl (
		const QString & ctl_n );

	void
	refresh ( );

	void
	set_fullscreen (
		bool flag_n );


	// Device selection

	void
	show_device_selection (
		bool flag_n );

	void
	toggle_device_selection ( );


	// Protected methods
	protected:

	void
	changeEvent (
		QEvent * event_n );

	void
	closeEvent (
		QCloseEvent * event_n );


	// Private slots
	private slots:

	void
	select_ctl_from_side_iface ( );

	void
	show_info_dialog ( );


	// Private methods
	private:

	void
	init_docks ( );

	void
	init_menu_bar ( );

	void
	update_fullscreen_action ( );


	// Private attributes
	private:

	// Shared storages and settings
	::Wdg::DS_Widget_Style_Db _wdg_style_db;
	::dpe::Image_Allocator _image_alloc;

	// Widgets settings
	Main_Window_Setup _setup;

	// Widgets
	QSharedPointer < ::Views::Mixer_HCTL > _mixer_wdg;
	::Views::Dev_Select_View * _dev_select;
	QDockWidget * _dev_select_dock;
	QPointer < QDialog > _info_dialog;

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
