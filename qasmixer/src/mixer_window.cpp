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

#include "mixer_window.hpp"

#include <QMenuBar>
#include <QSplitter>
#include <QLabel>
#include <QIcon>
#include <QFile>
#include <QVBoxLayout>
#include <QCloseEvent>

#include "mview/mv_mixer_simple.hpp"
#include "mview/mv_mixer_ctl.hpp"
#include "mview/mv_info.hpp"
#include "mview/dev_select_view.hpp"
#include "mview/switcher.hpp"

#include <iostream>


Mixer_Window::Mixer_Window (
	QWidget * parent_n,
	Qt::WindowFlags flags_n ) :
QMainWindow ( parent_n, flags_n ),
_mixer_setup ( 0 ),
_switcher ( 0 ),
_dev_select ( 0 ),
_dev_select_dock ( 0 )
{
	setWindowTitle ( PROGRAM_TITLE );
	setObjectName ( PROGRAM_TITLE );

	_str_fscreen_enable = tr ( "&Fullscreen mode" );
	_str_fscreen_disable = tr ( "Exit &fullscreen mode" );
	_icon_fscreen_enable = QIcon::fromTheme ( "view-fullscreen" );
	_icon_fscreen_disable = QIcon::fromTheme ( "view-restore" );

	_switcher = new ::MView::Switcher();

    connect ( _switcher, SIGNAL ( sig_view_type_changed() ),
		this, SLOT ( update_view_type_actions() ) );

	// Init menus
	init_menus();
	init_docks();

	setCentralWidget ( _switcher );
	update_fullscreen_action();
}


Mixer_Window::~Mixer_Window ( )
{
	_switcher->set_mixer_setup ( 0 );
	delete _switcher;
	delete _dev_select;
	delete _dev_select_dock;
}


void
Mixer_Window::init_menus ( )
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

	// Action: View type selection
	_act_show_vtype_select = new QAction ( this );
	_act_show_vtype_select->setText ( tr ( "Show &view type selection" ) );
	_act_show_vtype_select->setCheckable ( true );


	_act_grp_menu_vtype = new QActionGroup ( this );
	{
		QAction * acts[_switcher->num_view_types()];
		for ( unsigned int vii=0; vii < _switcher->num_view_types(); ++vii ) {
			acts[vii] = new QAction ( _act_grp_menu_vtype );
			acts[vii]->setText ( _switcher->view_type_name ( vii ) );
			acts[vii]->setCheckable ( true );
			acts[vii]->setData ( vii );
			acts[vii]->setToolTip ( _switcher->view_type_name ( vii ) );
		}

		acts[0]->setShortcut ( QKeySequence ( "1" ) );
		acts[1]->setShortcut ( QKeySequence ( "2" ) );
		acts[2]->setShortcut ( QKeySequence ( "3" ) );
	}


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
		cmenu->addAction ( _act_show_vtype_select );
		cmenu->addSeparator();
		cmenu->addActions ( _act_grp_menu_vtype->actions() );
		cmenu->addSeparator();
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

	connect ( _act_show_vtype_select, SIGNAL ( toggled ( bool ) ),
		_switcher, SLOT ( show_vtype_select ( bool ) ) );

	connect ( _switcher, SIGNAL ( sig_show_vtype_select ( bool ) ),
		_act_show_vtype_select, SLOT ( setChecked ( bool ) ) );


	connect ( _act_grp_menu_vtype, SIGNAL ( triggered ( QAction * ) ),
		this, SLOT ( view_type_action_triggered ( QAction * ) ) );


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
Mixer_Window::init_docks ( )
{
	_dev_select = new ::MView::Dev_Select_View;

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
Mixer_Window::set_mixer_setup (
	Mixer_Window_Setup * setup_n )
{
	if ( _mixer_setup != 0 ) {
		_switcher->set_mixer_setup ( 0 );
		_dev_select->set_view_setup ( 0 );
	}

	_mixer_setup = setup_n;

	if ( _mixer_setup != 0 ) {
		_act_show_dev_select->setShortcut ( _mixer_setup->kseq_dev_select );
		_act_show_dev_select->setChecked ( _mixer_setup->show_dev_select );

		_act_show_vtype_select->setShortcut ( _mixer_setup->kseq_vtype_select );
		_act_show_vtype_select->setChecked ( _mixer_setup->switcher.show_vtype_select );

		// Pass setup tree to child classes
		_switcher->set_mixer_setup ( &_mixer_setup->switcher );
		_dev_select->set_view_setup ( &_mixer_setup->dev_select );
		_dev_select->silent_select_ctl (
			_mixer_setup->switcher.mixer_dev.ctl_addr );
	}
}


void
Mixer_Window::update_fullscreen_action ( )
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
Mixer_Window::select_ctl (
	const QString & ctl_n )
{
	_dev_select->silent_select_ctl ( ctl_n );
	_switcher->select_snd_ctl ( ctl_n );

	emit sig_control_changed();
}


void
Mixer_Window::select_ctl_from_side_iface ( )
{
	_switcher->select_snd_ctl ( _dev_select->selected_ctl().addr_str() );

	emit sig_control_changed();
}


void
Mixer_Window::reload_mixer_device ( )
{
	//::std::cout << "Mixer_Window::reload_mixer_device" << "\n";
	_dev_select->reload_database();
	_switcher->reload_mdev_setup();
}


void
Mixer_Window::reload_mixer_inputs ( )
{
	//::std::cout << "Mixer_Window::reload_mixer_inputs" << "\n";
	_switcher->reload_inputs_setup();
}


void
Mixer_Window::reload_mixer_view ( )
{
	//::std::cout << "Mixer_Window::reload_mixer_view" << "\n";
	_switcher->reload_view_setup();
}


void
Mixer_Window::set_fullscreen (
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
Mixer_Window::show_device_selection (
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
Mixer_Window::toggle_device_selection ( )
{
	_act_show_dev_select->setChecked ( !_act_show_dev_select->isChecked() );
}


void
Mixer_Window::save_state ( )
{
	if ( _mixer_setup != 0 ) {
		_mixer_setup->window_state = saveState();
		_mixer_setup->window_geometry = saveGeometry();
	}
}


void
Mixer_Window::view_type_action_triggered (
	QAction * act_n )
{
	if ( act_n != 0 ) {
		const QVariant & vdata ( act_n->data() );
		if ( vdata.isValid() ) {
			if ( vdata.canConvert ( QVariant::UInt ) ) {
				_switcher->select_view_type ( vdata.toUInt() );
			}
		}
	}
}


void
Mixer_Window::update_view_type_actions ( )
{
	if ( _mixer_setup == 0 ) {
		return;
	}

	const int vtype ( _mixer_setup->switcher.view_type );
	QList < QAction * > acts ( _act_grp_menu_vtype->actions() );
	if ( vtype < acts.size() ) {
		acts[vtype]->setChecked ( true );
	}
}


void
Mixer_Window::changeEvent (
	QEvent * event_n )
{
	QMainWindow::changeEvent ( event_n );
	if ( event_n->type() == QEvent::WindowStateChange ) {
		update_fullscreen_action();
	}
}


void
Mixer_Window::keyPressEvent (
	QKeyEvent * event_n )
{
	QMainWindow::keyPressEvent ( event_n );
	if ( _mixer_setup != 0 ) {
		const QKeySequence kseq ( event_n->key() );
		if ( kseq == _mixer_setup->kseq_dev_select ) {
			toggle_device_selection();
		} else if ( kseq == _mixer_setup->kseq_vtype_select ) {
			_switcher->toggle_vtype_select();
		}
	}
}


bool
Mixer_Window::eventFilter (
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

