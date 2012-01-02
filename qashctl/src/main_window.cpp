//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "main_window.hpp"

#include "qastools_config.hpp"
#include "qsnd/alsa_config_watcher.hpp"
#include "wdg/ds_switch_painter_circle.hpp"
#include "wdg/ds_switch_painter_svg.hpp"
#include "wdg/ds_slider_painter_bevelled.hpp"
#include "mwdg/mixer_style.hpp"
#include "views/info_view.hpp"
#include "views/dev_select_view.hpp"
#include "views/view_utility.hpp"

#include <QSettings>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QEvent>

#include <iostream>


Main_Window::Main_Window ( )
{
	setWindowTitle ( PROGRAM_TITLE );
	setObjectName ( PROGRAM_TITLE );
	setContextMenuPolicy ( Qt::NoContextMenu );

	// Strings and icons
	_str_fscreen_enable = tr ( "&Fullscreen mode" );
	_str_fscreen_disable = tr ( "Exit &fullscreen mode" );
	_icon_fscreen_enable = QIcon::fromTheme ( "view-fullscreen" );
	_icon_fscreen_disable = QIcon::fromTheme ( "view-restore" );

	// File watcher
	{
		::QSnd::ALSA_Config_Watcher * watcher (
			new ::QSnd::ALSA_Config_Watcher ( this ) );

		connect ( watcher, SIGNAL ( sig_change() ),
			this, SLOT ( refresh() ) );
	}

	// Slider painter bevelled
	{
		::Wdg::Painter::DS_Slider_Painter_Bevelled * pnt (
			new ::Wdg::Painter::DS_Slider_Painter_Bevelled );
		pnt->set_wdg_style_db ( &_wdg_style_db );
		_image_alloc.install_painter ( pnt );
	}

	// Switch painter circle
	{
		::Wdg::Painter::DS_Switch_Painter_Circle * pnt (
			new ::Wdg::Painter::DS_Switch_Painter_Circle );
		pnt->set_group_variant ( ::Wdg::DS_CIRCLE );
		pnt->set_wdg_style_db ( &_wdg_style_db );
		_image_alloc.install_painter ( pnt );
	}
	// Switch painter SVG
	{
		QScopedPointer < ::Wdg::Painter::DS_Switch_Painter_SVG > pnt (
			new ::Wdg::Painter::DS_Switch_Painter_SVG );
		pnt->set_group_variant ( ::Wdg::DS_SVG_JOINED );
		pnt->set_wdg_style_db ( &_wdg_style_db );
		pnt->set_base_dir ( INSTALL_DIR_WIDGETS_GRAPHICS );
		pnt->set_file_prefix_bg ( "sw_joined_bg_" );
		pnt->set_file_prefix_handle ( "sw_joined_handle_" );
		if ( pnt->ready() ) {
			_image_alloc.install_painter ( pnt.take() );
		}
	}

	// Init widget style database
	{
		::MWdg::Mixer_Style mstyle;
		::MWdg::Mixer_Style::Style_Type stype;

		stype = ::MWdg::Mixer_Style::PLAYBACK ;
		_wdg_style_db.palettes.insert (
			stype, mstyle.style_palette ( stype ) );

		stype = ::MWdg::Mixer_Style::CAPTURE;
		_wdg_style_db.palettes.insert (
			stype, mstyle.style_palette ( stype ) );
	}

	// Init setup
	{
		_setup.hctl.wdg_style_db = &_wdg_style_db;
		_setup.hctl.image_alloc = &_image_alloc;
	}

	// Widgets and docks
	init_docks();
	init_menu_bar();

	_mixer_wdg.reset ( new ::Views::Mixer_HCTL );
	setCentralWidget ( _mixer_wdg.data() );

	update_fullscreen_action();
}


void
Main_Window::init_docks ( )
{
	_dev_select_dock = new QDockWidget ( this );
	_dev_select_dock->hide(); // Start hidden
	_dev_select_dock->setTitleBarWidget ( new QWidget ( _dev_select_dock ) );
	_dev_select_dock->setFeatures ( QDockWidget::DockWidgetMovable );
	_dev_select_dock->setObjectName ( "device_dock" );
	_dev_select_dock->setAllowedAreas (
		Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
	_dev_select_dock->installEventFilter ( this );

	// QueuedConnection to update the GUI before loading the mixer
	_dev_select = new ::Views::Dev_Select_View;
	connect ( _dev_select, SIGNAL ( sig_control_changed() ),
		this, SLOT ( select_ctl_from_side_iface() ), Qt::QueuedConnection );

	connect ( _dev_select, SIGNAL ( sig_close() ),
		this, SLOT ( toggle_device_selection() ) );

	_dev_select_dock->setWidget ( _dev_select );
	addDockWidget ( Qt::RightDockWidgetArea, _dev_select_dock );
}


void
Main_Window::init_menu_bar ( )
{
	// Action: Quit
	QAction * act_quit ( new QAction ( tr ( "&Quit" ), this ) );
	act_quit->setShortcut ( QKeySequence ( QKeySequence::Quit ) );
	act_quit->setIcon ( QIcon::fromTheme ( "application-exit" ) );

	// Action: Device selection
	_act_show_dev_select = new QAction ( this );
	_act_show_dev_select->setText ( tr ( "Show &device selection" ) );
	_act_show_dev_select->setCheckable ( true );
	_act_show_dev_select->setShortcut ( _setup.dev_select.kseq_toggle_vis );

	// Action: Fullscreen
	_act_fullscreen = new QAction ( this );
	_act_fullscreen->setShortcut ( QKeySequence ( Qt::Key_F11 ) );
	_act_fullscreen->setCheckable ( true );

	// Action: Refresh
	QAction * act_refresh ( new QAction ( tr ( "&Refresh" ), this ) );
	act_refresh->setShortcut ( QKeySequence ( QKeySequence::Refresh ) );
	act_refresh->setIcon ( QIcon::fromTheme ( "view-refresh" ) );

	// Action: Info
	QAction * act_info = new QAction ( tr ( "&Info" ), this );
	act_info->setShortcut ( QKeySequence ( QKeySequence::HelpContents ) );
	act_info->setIcon ( QIcon::fromTheme ( "help-about" ) );

	// Menu: File
	{
		QMenu * cmenu ( menuBar()->addMenu ( tr ( "&File" ) ) );
		cmenu->addAction ( act_quit );
	}

	// Menu: View
	{
		QMenu * cmenu = menuBar()->addMenu ( tr ( "&View" ) );
		cmenu->addAction ( _act_show_dev_select );
		cmenu->addAction ( _act_fullscreen );
		cmenu->addSeparator();
		cmenu->addAction ( act_refresh );
	}

	// Menu: Help
	{
		QMenu * menu = menuBar()->addMenu ( tr ( "&Help" ) );
		menu->addAction ( act_info );
	}


	// Connect actions

    connect ( act_quit, SIGNAL ( triggered() ),
		this, SLOT ( close() ) );

	connect ( _act_show_dev_select, SIGNAL ( toggled ( bool ) ),
		this, SLOT ( show_device_selection ( bool ) ) );

    connect ( _act_fullscreen, SIGNAL ( toggled ( bool ) ),
		this, SLOT ( set_fullscreen ( bool ) ) );

    connect ( act_refresh, SIGNAL ( triggered() ),
		this, SLOT ( refresh() ) );

    connect ( act_info, SIGNAL ( triggered() ),
		this, SLOT ( show_info_dialog() ) );
}


void
Main_Window::restore_state ( )
{
	QSettings settings;

	{
		const QByteArray & ba = settings.value (
			"main_window_state", QByteArray() ).toByteArray();
		restoreState ( ba );
	}
	{
		const QByteArray & ba = settings.value (
			"main_window_geometry", QByteArray() ).toByteArray();
		if ( !restoreGeometry ( ba ) ) {
			::Views::resize_to_default ( this );
		}
		update_fullscreen_action();
	}

	_setup.show_dev_select = settings.value ( "show_device_selection",
		_setup.show_dev_select ).toBool();

	_setup.mixer_dev.ctl_addr = settings.value ( "current_device",
		_setup.mixer_dev.ctl_addr ).toString();

	_setup.inputs.wheel_degrees = settings.value ( "wheel_degrees",
		_setup.inputs.wheel_degrees ).toUInt();

	// CTL mixer
	{
		settings.beginGroup ( "element_mixer" );

		_setup.hctl.iface_name = settings.value ( "iface_name",
			_setup.hctl.iface_name ).toString();

		_setup.hctl.elem_grp_name = settings.value ( "elem_grp_name",
			_setup.hctl.elem_grp_name ).toString();

		_setup.hctl.elem_grp_index = settings.value ( "elem_grp_index",
			_setup.hctl.elem_grp_index ).toUInt();

		settings.endGroup();
	}

	// Device selection
	{
		settings.beginGroup ( "device_selection" );

		_setup.dev_select.sel_db_set ( settings.value ( "selection_db",
			_setup.dev_select.sel_db_get() ).toStringList() );

		settings.endGroup();
	}

	// Sanitize values

	if ( _setup.mixer_dev.ctl_addr.isEmpty() ) {
		_setup.mixer_dev.ctl_addr = "default";
	}

	if ( _setup.inputs.wheel_degrees == 0 ) {
		_setup.inputs.wheel_degrees = 720;
	}

	_setup.inputs.update_translation();

	// Setup widgets
	show_device_selection ( _setup.show_dev_select );
	_dev_select->set_view_setup ( &_setup.dev_select );
	_dev_select->silent_select_ctl ( _setup.mixer_dev.ctl_addr );
	_mixer_wdg->set_inputs_setup ( &_setup.inputs );
	_mixer_wdg->set_mdev_setup ( &_setup.mixer_dev );
	_mixer_wdg->set_view_setup ( &_setup.hctl );
}


void
Main_Window::save_state ( )
{
	QSettings settings;

	settings.setValue ( "main_window_state", saveState() );
	settings.setValue ( "main_window_geometry", saveGeometry() );

	settings.setValue ( "show_device_selection", _setup.show_dev_select );
	settings.setValue ( "current_device", _setup.mixer_dev.ctl_addr );
	settings.setValue ( "wheel_degrees", _setup.inputs.wheel_degrees );

	// CTL mixer
	{
		settings.beginGroup ( "element_mixer" );
		settings.setValue ( "iface_name", _setup.hctl.iface_name );
		settings.setValue ( "elem_grp_name", _setup.hctl.elem_grp_name );
		settings.setValue ( "elem_grp_index", _setup.hctl.elem_grp_index );
		settings.endGroup();
	}

	// Device selection
	{
		settings.beginGroup ( "device_selection" );
		settings.setValue ( "selection_db", _setup.dev_select.sel_db_get() );
		settings.endGroup();
	}
}


void
Main_Window::refresh ( )
{
	//::std::cout << "Refresh" << "\n";
	_mixer_wdg->set_mdev_setup ( 0 );
	_mixer_wdg->set_mdev_setup ( &_setup.mixer_dev );
}



void
Main_Window::select_snd_ctl (
	const QString & ctl_n )
{
	if ( ctl_n != _setup.mixer_dev.ctl_addr ) {
		_mixer_wdg->set_mdev_setup ( 0 );
		_setup.mixer_dev.ctl_addr = ctl_n;
		_mixer_wdg->set_mdev_setup ( &_setup.mixer_dev );
	}
}


void
Main_Window::select_ctl_from_side_iface ( )
{
	select_snd_ctl ( _dev_select->selected_ctl().addr_str() );
}


void
Main_Window::set_fullscreen (
	bool flag_n )
{
	if ( flag_n != isFullScreen() ) {
		if ( flag_n ) {
			showFullScreen();
		} else {
			showNormal();
		}
		update_fullscreen_action();
	}
}


void
Main_Window::show_device_selection (
	bool flag_n )
{
	if ( _setup.show_dev_select != flag_n ) {
		_setup.show_dev_select = flag_n;
	}
	_act_show_dev_select->setChecked ( flag_n );
	_dev_select_dock->setVisible ( flag_n );
}


void
Main_Window::toggle_device_selection ( )
{
	_act_show_dev_select->setChecked ( !_act_show_dev_select->isChecked() );
}


void
Main_Window::update_fullscreen_action ( )
{
	QString * txt;
	QIcon * icon;
	bool checked;

	if ( isFullScreen() ) {
		txt = &_str_fscreen_disable;
		icon = &_icon_fscreen_disable;
		checked = true;
	} else {
		txt = &_str_fscreen_enable;
		icon = &_icon_fscreen_enable;
		checked = false;
	}

	_act_fullscreen->setText ( *txt );
	_act_fullscreen->setIcon ( *icon );
	_act_fullscreen->setChecked ( checked );
}


void
Main_Window::show_info_dialog ( )
{
	if ( _info_dialog == 0 ) {
		QDialog * dlg ( new QDialog ( this ) );
		dlg->setAttribute ( Qt::WA_DeleteOnClose );
		{
			::Views::Info_View * view ( new ( ::Views::Info_View ) );
			connect ( view, SIGNAL ( sig_close() ),
				dlg, SLOT ( close() ) );

			QVBoxLayout * lay ( new QVBoxLayout );
			lay->setContentsMargins ( 0, 0, 0, 0 );
			lay->addWidget ( view );
			dlg->setLayout ( lay );
		}
		_info_dialog = dlg;
	}

	_info_dialog->show();
}


void
Main_Window::changeEvent (
	QEvent * event_n )
{
	QMainWindow::changeEvent ( event_n );
	if ( event_n->type() == QEvent::WindowStateChange ) {
		update_fullscreen_action();
	}
}


void
Main_Window::closeEvent (
	QCloseEvent * event_n )
{
	save_state();
	QMainWindow::closeEvent ( event_n );
}


bool
Main_Window::eventFilter (
	QObject * obj_n,
	QEvent * event_n )
{
	bool filtered ( false );
	if ( obj_n == _dev_select_dock ) {
		if ( event_n->type() == QEvent::Close ) {
			_act_show_dev_select->setChecked ( false );
		}
	}
	return filtered;
}

