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

#include "tray_mixer_icon.hpp"

#include "qastools_config.hpp"
#include <QIcon>
#include <QAction>
#include <QEvent>
#include <QWheelEvent>
#include <iostream>


Tray_Mixer_Icon::Tray_Mixer_Icon (
	QObject * parent_n ) :
QSystemTrayIcon ( parent_n )
{
	connect (
		this, SIGNAL ( activated ( QSystemTrayIcon::ActivationReason ) ),
		this, SLOT ( activation ( QSystemTrayIcon::ActivationReason ) ) );

	// Context menu
	{
		QAction * act_show ( new QAction ( this ) );
		act_show->setText ( tr ( "&Show mixer" ) );
		act_show->setShortcut ( QKeySequence ( tr ( "Ctrl+s" ) ) );
		act_show->setIcon ( QIcon::fromTheme ( "view-fullscreen" ) );

		QAction * act_quit ( new QAction ( this ) );
		//: %1 will be replaced with the program title
		act_quit->setText ( tr ( "&Close %1" ).arg ( PROGRAM_TITLE ) );
		act_quit->setShortcut ( QKeySequence ( QKeySequence::Quit ) );
		act_quit->setIcon ( QIcon::fromTheme ( "application-exit" ) );

		connect ( act_show, SIGNAL ( triggered() ),
			this, SIGNAL ( sig_activated() ) );

		connect ( act_quit, SIGNAL ( triggered() ),
			this, SIGNAL ( sig_quit() ) );

		_cmenu.addAction ( act_show );
		_cmenu.addAction ( act_quit );

		setContextMenu ( &_cmenu );
	}
}


void
Tray_Mixer_Icon::activation (
	QSystemTrayIcon::ActivationReason reason_n )
{
	//::std::cout << "Tray_Mixer::activation " << reason_n << "\n";

	if ( reason_n == QSystemTrayIcon::Context ) {
		return;
	} else if ( reason_n == QSystemTrayIcon::MiddleClick  ) {
		emit sig_middle_click();
	} else {
		emit sig_activated();
	}
}


bool
Tray_Mixer_Icon::event (
	QEvent * event_n )
{
	bool res ( QSystemTrayIcon::event ( event_n ) );

	if ( event_n->type() == QEvent::Wheel ) {
		QWheelEvent * wev ( static_cast < QWheelEvent * > ( event_n ) );
		emit sig_wheel_delta ( wev->delta() );
		res = true;
	} else if ( event_n->type() == QEvent::ToolTip ) {
		emit sig_hover();
	}

	return res;
}

