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

#include "views/mixer_simple.hpp"
#include "views/dev_select_view.hpp"

#include <QMenuBar>
#include <QSplitter>
#include <QLabel>
#include <QIcon>
#include <QFile>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <iostream>


Main_Window::Main_Window (
	QWidget * parent_n,
	Qt::WindowFlags flags_n ) :
QMainWindow ( parent_n, flags_n ),
_mixer_setup ( 0 ),
_mixer_simple ( 0 ),
_dev_select ( 0 ),
_dev_select_dock ( 0 )
{
	setWindowTitle ( PROGRAM_TITLE );
	setObjectName ( PROGRAM_TITLE );

	_str_fscreen_enable = tr ( "&Fullscreen mode" );
	_str_fscreen_disable = tr ( "Exit &fullscreen mode" );
	_icon_fscreen_enable = QIcon::fromTheme ( "view-fullscreen" );
	_icon_fscreen_disable = QIcon::fromTheme ( "view-restore" );

	// Init menus
	init_menus();
	init_docks();

	_mixer_simple = new ::Views::Mixer_Simple();

	setCentralWidget ( _mixer_simple );
	update_fullscreen_action();
}


Main_Window::~Main_Window ( )
{
}


void
Main_Window::init_menus ( )
{
	// Action: Exit / Quit
	QAction * act_quit = new QAction ( tr ( "&Quit" ), this );
	act_quit->setShortcut ( QKeySequence ( QKeySequence::Quit ) );
	act_quit->setIcon ( QIcon::fromTheme ( "application-exit" ) );

	// Action: Settings
	QAction * act_settings = new QAction ( tr ( "&Settings" ), this );
	act_settings->setShortcut ( QKeySequence ( tr ( "Ctrl+s" ) ) );
	act_settings->setIcon ( QIcon::fromTheme ( "preferences-desktop" ) );

	// Action: Refresh
	QAction * act_refresh = new QAction ( tr ( "&Refresh" ), this );
	act_refresh->setShortcut ( QKeySequence ( QKeySequence::Refresh ) );
	act_refresh->setIcon ( QIcon::fromTheme ( "view-refresh" ) );

	// Action: Device selection
	_act_show_dev_select = new QAction ( this );
	_act_show_dev_select->setText ( tr ( "Show &device selection" ) );
	_act_show_dev_select->setCheckable ( true );

	// Action: Fullscreen
	_act_fullscreen = new QAction ( this );
	_act_fullscreen->setShortcut ( QKeySequence ( Qt::Key_F11 ) );
	_act_fullscreen->setCheckable ( true );

	// Action: Info
	QAction * act_info = new QAction ( tr ( "&Info" ), this );
	act_info->setShortcut ( QKeySequence ( QKeySequence::HelpContents ) );
	act_info->setIcon ( QIcon::fromTheme ( "help-about" ) );


	// Menus
	{
		QMenu * cmenu = menuBar()->addMenu ( tr ( "&File" ) );
		cmenu->addAction ( act_settings );
		cmenu->addSeparator();
		cmenu->addAction ( act_quit );
	}
	{
		QMenu * cmenu = menuBar()->addMenu ( tr ( "&View" ) );
		cmenu->addAction ( _act_show_dev_select );
		cmenu->addAction ( _act_fullscreen );
		cmenu->addAction ( act_refresh );
	}
	{
		QMenu * cmenu = menuBar()->addMenu ( tr ( "&Help" ) );
		cmenu->addAction ( act_info );
	}


	// Connect actions

    connect ( act_quit, SIGNAL ( triggered() ),
		this, SIGNAL ( sig_quit() ) );


	// View actions

	connect ( _act_show_dev_select, SIGNAL ( toggled ( bool ) ),
		this, SLOT ( show_device_selection ( bool ) ) );

    connect ( act_refresh, SIGNAL ( triggered() ),
		this, SLOT ( reload_mixer_device() ) );

    connect ( _act_fullscreen, SIGNAL ( toggled ( bool ) ),
		this, SLOT ( set_fullscreen ( bool ) ) );


	// Info action

    connect ( act_settings, SIGNAL ( triggered() ),
		this, SIGNAL ( sig_show_settings() ) );

    connect ( act_info, SIGNAL ( triggered() ),
		this, SIGNAL ( sig_show_info() ) );
}


void
Main_Window::init_docks ( )
{
	_dev_select = new ::Views::Dev_Select_View;

	// QueuedConnection to update the GUI before loading the mixer
	connect ( _dev_select, SIGNAL ( sig_control_changed() ),
		this, SLOT ( select_ctl_from_side_iface() ), Qt::QueuedConnection );

	connect ( _dev_select, SIGNAL ( sig_close() ),
		this, SLOT ( toggle_device_selection() ) );

	QString dock_title ( tr ( "Mixer device" ) );
	_dev_select_dock = new QDockWidget ( this );
	_dev_select_dock->hide(); // Start hidden
	//_dev_select_dock->setWindowTitle ( dock_title );
	_dev_select_dock->setTitleBarWidget ( new QWidget ( _dev_select_dock ) );
	_dev_select_dock->setFeatures ( QDockWidget::DockWidgetMovable );
	_dev_select_dock->setObjectName ( "device_dock" );
	_dev_select_dock->setWidget ( _dev_select );
	_dev_select_dock->setAllowedAreas (
		Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
	_dev_select_dock->installEventFilter ( this );

	addDockWidget ( Qt::RightDockWidgetArea, _dev_select_dock );
}


void
Main_Window::set_mixer_setup (
	Main_Window_Setup * setup_n )
{
	if ( _mixer_setup != 0 ) {
		_mixer_simple->set_mdev_setup ( 0 );
		_mixer_simple->set_inputs_setup ( 0 );
		_mixer_simple->set_view_setup ( 0 );
		_dev_select->set_view_setup ( 0 );
	}

	_mixer_setup = setup_n;

	if ( _mixer_setup != 0 ) {
		// Actions
		_act_show_dev_select->setShortcut ( _mixer_setup->dev_select.kseq_toggle_vis );
		_act_show_dev_select->setChecked ( _mixer_setup->show_dev_select );

		// Pass setup tree to child classes
		_dev_select->set_view_setup ( &_mixer_setup->dev_select );
		_dev_select->silent_select_ctl ( _mixer_setup->mixer_dev.ctl_addr );

		_mixer_simple->set_view_setup ( &_mixer_setup->mixer_simple );
		_mixer_simple->set_inputs_setup ( &_mixer_setup->inputs );
		_mixer_simple->set_mdev_setup ( &_mixer_setup->mixer_dev );
	}
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
Main_Window::select_ctl (
	const QString & ctl_n )
{
	_dev_select->silent_select_ctl ( ctl_n );

	if ( _mixer_setup != 0 ) {
		if ( ctl_n != _mixer_setup->mixer_dev.ctl_addr ) {
			// Remove
			_mixer_simple->set_mdev_setup ( 0 );
			// Change
			_mixer_setup->mixer_dev.ctl_addr = ctl_n;
			// Reinstall
			_mixer_simple->set_mdev_setup ( &_mixer_setup->mixer_dev );
		}
	}

	emit sig_control_changed();
}


void
Main_Window::select_ctl_from_side_iface ( )
{
	select_ctl ( _dev_select->selected_ctl().addr_str() );

	emit sig_control_changed();
}


void
Main_Window::reload_mixer_device ( )
{
	//::std::cout << "Main_Window::reload_mixer_device" << "\n";
	_dev_select->reload_database();

	_mixer_simple->set_mdev_setup ( 0 );
	_mixer_simple->set_mdev_setup ( &_mixer_setup->mixer_dev );
}


void
Main_Window::reload_mixer_inputs ( )
{
	//::std::cout << "Main_Window::reload_mixer_inputs" << "\n";
	_mixer_simple->set_inputs_setup ( 0 );
	_mixer_simple->set_inputs_setup ( &_mixer_setup->inputs );
}


void
Main_Window::reload_mixer_view ( )
{
	//::std::cout << "Main_Window::reload_mixer_view" << "\n";
	_mixer_simple->set_view_setup ( 0 );
	_mixer_simple->set_view_setup ( &_mixer_setup->mixer_simple );
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
	if ( _mixer_setup != 0 ) {
		if ( _mixer_setup->show_dev_select != flag_n ) {
			_mixer_setup->show_dev_select = flag_n;
		}
	}
	_dev_select_dock->setVisible ( flag_n );
}


void
Main_Window::toggle_device_selection ( )
{
	_act_show_dev_select->setChecked ( !_act_show_dev_select->isChecked() );
}


void
Main_Window::save_state ( )
{
	if ( _mixer_setup != 0 ) {
		_mixer_setup->window_state = saveState();
		_mixer_setup->window_geometry = saveGeometry();
	}
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
Main_Window::keyPressEvent (
	QKeyEvent * event_n )
{
	QMainWindow::keyPressEvent ( event_n );
	if ( _mixer_setup != 0 ) {
		const QKeySequence kseq ( event_n->key() );
		if ( kseq == _mixer_setup->dev_select.kseq_toggle_vis ) {
			toggle_device_selection();
		}
	}
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
