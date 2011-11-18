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

#include "config.hpp"
#include "qsnd/alsa_config_watcher.hpp"
#include "views/info_view.hpp"

#include <QVBoxLayout>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <iostream>


Main_Window::Main_Window ( )
{
	setWindowTitle ( PROGRAM_TITLE );
	setObjectName ( PROGRAM_TITLE );

	init_menu_bar();

	{
		::QSnd::ALSA_Config_Watcher * watcher (
			new ::QSnd::ALSA_Config_Watcher ( this ) );

		connect ( watcher, SIGNAL ( sig_change() ),
			this, SLOT ( refresh() ) );
	}


	_alsa_cfg_view.set_model ( &_alsa_cfg_model );

	setCentralWidget ( &_alsa_cfg_view );
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
Main_Window::init_menu_bar ( )
{
	// Action: Quit
	QAction * act_quit ( new QAction ( tr ( "&Quit" ), this ) );
	act_quit->setShortcut ( QKeySequence ( QKeySequence::Quit ) );
	act_quit->setIcon ( QIcon::fromTheme ( "application-exit" ) );

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
		QMenu * menu ( menuBar()->addMenu ( tr ( "&File" ) ) );
		menu->addAction ( act_refresh );
		menu->addAction ( act_quit );
	}

	// Menu: Help
	{
		QMenu * cmenu = menuBar()->addMenu ( tr ( "&Help" ) );
		cmenu->addAction ( act_info );
	}


	// Connect actions

    connect ( act_quit, SIGNAL ( triggered() ),
		this, SLOT ( close() ) );

    connect ( act_refresh, SIGNAL ( triggered() ),
		this, SLOT ( refresh() ) );

    connect ( act_info, SIGNAL ( triggered() ),
		this, SLOT ( show_info_dialog() ) );
}


void
Main_Window::refresh ( )
{
	//::std::cout << "Refresh" << "\n";
	_alsa_cfg_view.reload_config();
}
