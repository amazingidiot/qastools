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

#include <QMainWindow>
#include <QDialog>
#include <QPointer>

#include "qsnd/alsa_config_model.hpp"
#include "views/alsa_config_view.hpp"


/// @brief Main_Window
///
class Main_Window :
	public QMainWindow
{
	Q_OBJECT

	// Public methods
	public:

	Main_Window ( );

	QSize
	sizeHint ( ) const;


	void
	restore_state ( );

	void
	save_state ( );


	// Public slots
	private slots:

	void
	refresh ( );


	// Protected methods
	protected:

	void
	closeEvent (
		QCloseEvent * event_n );


	// Private slots
	private slots:

	void
	show_info_dialog ( );


	// Private methods
	private:

	void
	init_menu_bar ( );


	// Private attributes
	private:

	::QSnd::Alsa_Config_Model _alsa_cfg_model;
	::Views::Alsa_Config_View _alsa_cfg_view;

	QPointer < QDialog > _info_dialog;
};


#endif
