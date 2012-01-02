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

#include "settings_view.hpp"

#include "qastools_config.hpp"
#include "desktop_items_setup.hpp"
#include "wdg/scroll_area_vertical.hpp"
#include "views/view_utility.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QSplitter>
#include <QGroupBox>
#include <QStandardItemModel>

#include <iostream>


namespace Views
{


Settings_View::Settings_View (
	QWidget * parent ) :
QWidget ( parent ),
_dsetup ( 0 ),
_updating_values ( false )
{
	setWindowTitle (
		QString ( "%1 - %2" )
		.arg ( PROGRAM_TITLE )
		.arg ( tr ( "Settings" ) ) );

	_vspace = qMax ( 0, fontMetrics().height() * 2 / 3 );

	_page_selection = new QListView;

	// Page selection model for the tree view
	{
		QStandardItemModel * pages_model ( new QStandardItemModel ( this ) );

		QString pnames[num_pages];
		pnames[0] = tr ( "Startup" );
		pnames[1] = tr ( "Appearance" );
		pnames[2] = tr ( "Input" );
		pnames[3] = tr ( "System tray" );

		for ( unsigned int ii=0; ii < num_pages; ++ii ) {
			QStandardItem * sitem ( new QStandardItem );
			sitem->setText ( pnames[ii] );
			sitem->setToolTip ( pnames[ii] );
			sitem->setSelectable ( true );
			sitem->setEditable ( false );
			_page_items[ii] = sitem;
		}

		for ( unsigned int ii=0; ii < num_pages; ++ii ) {
			pages_model->appendRow ( _page_items[ii] );
		}

		_page_selection->setModel ( pages_model );
	}

	// QueuedConnection to paint update the tree view before heavy painting new widgets
	connect ( _page_selection->selectionModel(),
		SIGNAL ( currentChanged ( const QModelIndex &, const QModelIndex & ) ),
		this, SLOT ( page_changed ( const QModelIndex &, const QModelIndex & ) ),
		Qt::QueuedConnection );


	init_page_startup();
	init_page_appearance();
	init_page_input();
	init_page_sys_tray();


	// Close button layout
	QHBoxLayout * lay_close ( new QHBoxLayout );
	lay_close->setContentsMargins ( 0, 0, 0, 0 );
	{
		QPushButton * btn_close ( new QPushButton ( tr ( "&Close" ) ) );

		if ( QIcon::hasThemeIcon ( "window-close" ) ) {
			btn_close->setIcon ( QIcon::fromTheme ( "window-close" ) );
		}

		connect ( btn_close, SIGNAL ( clicked() ),
			this, SIGNAL ( sig_close() ) );

		//lay_close->addStretch ( 1 );
		lay_close->addWidget ( btn_close );
		lay_close->addStretch ( 1 );
	}


	// Page navigation widget
	QWidget * navi_wdg ( new QWidget );
	{
		QVBoxLayout * lay_vbox ( new QVBoxLayout );
		lay_vbox->setContentsMargins ( 0, 0, 0, 0 );
		lay_vbox->addWidget ( _page_selection, 1 );
		lay_vbox->addLayout ( lay_close, 0 );

		navi_wdg->setLayout ( lay_vbox );
	}

	// Setup pages
	QWidget * pages_wdg ( new QWidget );
	{
		QHBoxLayout * lay_pages ( new QHBoxLayout );
		lay_pages->setContentsMargins ( 0, 0, 0, 0 );
		pages_wdg->setLayout ( lay_pages );

		_lay_pages_stack = new QStackedLayout;
		lay_pages->addSpacing ( fontMetrics().averageCharWidth() * 3 / 2 );
		lay_pages->addLayout ( _lay_pages_stack );

		_lay_pages_stack->setContentsMargins ( 0, 0, 0, 0 );
		_lay_pages_stack->addWidget ( _page_startup );
		_lay_pages_stack->addWidget ( _page_appearance );
		_lay_pages_stack->addWidget ( _page_input );
		_lay_pages_stack->addWidget ( _page_sys_tray );
	}

	// Main splitter and layout
	{
		QWidget * wdg_title ( dialog_title_widget ( tr ( "Settings" ) ) );

		QSplitter * hsplit ( new QSplitter );
		hsplit->setChildrenCollapsible ( false );
		hsplit->addWidget ( navi_wdg );
		hsplit->addWidget ( pages_wdg );
		hsplit->setStretchFactor ( 0, 2 );
		hsplit->setStretchFactor ( 1, 5 );

		QVBoxLayout * lay_vbox ( new QVBoxLayout );
		lay_vbox->addWidget ( wdg_title, 0 );
		lay_vbox->addWidget ( hsplit, 1 );
		setLayout ( lay_vbox );
	}

	// Update states
	update_inputs_vis_state();
}


Settings_View::~Settings_View ( )
{
	//::std::cout << "~Settings_View\n";
}


void
Settings_View::init_page_startup ( )
{
	QGroupBox * box_device ( new QGroupBox );
	box_device->setTitle ( tr ( "Startup mixer device" ) );
	{
		QVBoxLayout * lay_dev_sel ( new QVBoxLayout );

		_start_btn_device[0] = new QRadioButton ( tr ( "Default" ) );
		_start_btn_device[1] = new QRadioButton ( tr ( "From last session" ) );
		_start_btn_device[2] = new QRadioButton ( tr ( "User defined" ) );

		_start_btn_grp_device = new QButtonGroup ( this );
		_start_btn_grp_device->addButton ( _start_btn_device[0], 0 );
		_start_btn_grp_device->addButton ( _start_btn_device[1], 1 );
		_start_btn_grp_device->addButton ( _start_btn_device[2], 2 );


		_start_dev_user = new QWidget;
		_start_dev_user_edit = new QLineEdit;
		QLabel * lbl_user ( new QLabel ( tr ( "User device:" ) ) );
		QLabel * lbl_example ( new QLabel ( tr ( "e.g. hw:0" ) ) );

		connect ( _start_btn_grp_device, SIGNAL ( buttonClicked ( int ) ),
			this, SLOT ( change_startup() ) );

		connect ( _start_dev_user_edit, SIGNAL ( editingFinished() ),
			this, SLOT ( change_startup() ) );

		QHBoxLayout * lay_dev_user ( new QHBoxLayout );
		lay_dev_user->setContentsMargins ( 0, 0, 0, 0 );
		lay_dev_user->addWidget ( lbl_user );
		lay_dev_user->addWidget ( _start_dev_user_edit );
		lay_dev_user->addWidget ( lbl_example );
		lay_dev_user->addStretch ( 1 );
		_start_dev_user->setLayout ( lay_dev_user );

		lay_dev_sel->addWidget ( _start_btn_device[0] );
		lay_dev_sel->addWidget ( _start_btn_device[1] );
		lay_dev_sel->addWidget ( _start_btn_device[2] );
		lay_dev_sel->addWidget ( _start_dev_user );
		lay_dev_sel->addStretch ( 1 );

		box_device->setLayout ( lay_dev_sel );
	}




	// Layout
	QVBoxLayout * lay_wdg ( new QVBoxLayout );
	lay_wdg->setContentsMargins ( 0, 0, 0, 0 );
	lay_wdg->addWidget ( box_device );
	lay_wdg->addStretch ( 1 );

	_page_startup = create_page_container ( lay_wdg );
}


void
Settings_View::init_page_appearance ( )
{
	QGroupBox * box_smixer ( new QGroupBox );
	box_smixer->setTitle ( tr ( "Simple mixer view" ) );
	{
		_btn_show_sliders_labels =
			new QCheckBox ( tr ( "Show slider value labels" ) );

		connect ( _btn_show_sliders_labels, SIGNAL ( toggled ( bool ) ),
			this, SLOT ( change_appearance() ) );

		QVBoxLayout * lay_smixer ( new QVBoxLayout );
		lay_smixer->addWidget ( _btn_show_sliders_labels );
		box_smixer->setLayout ( lay_smixer );
	}

	// Layout
	QVBoxLayout * lay_wdg ( new QVBoxLayout );
	lay_wdg->setContentsMargins ( 0, 0, 0, 0 );
	lay_wdg->addWidget ( box_smixer );
	lay_wdg->addStretch ( 1 );

	_page_appearance = create_page_container ( lay_wdg );
}


void
Settings_View::init_page_input ( )
{
	QGroupBox * box_mwheel ( new QGroupBox );
	box_mwheel->setTitle ( tr ( "Mouse wheel" ) );
	{
		QLabel * lbl_info ( new QLabel (
			tr ( "Number of turns for a slider change from 0% to 100%" ) ) );

		const unsigned int range[2] = { 1, 150 };

		_mwheel_turns_input = new QDoubleSpinBox;
		_mwheel_turns_input->setRange ( range[0] / 10.0, range[1] / 10.0 );
		_mwheel_turns_input->setSingleStep ( 0.1 );
		_mwheel_turns_input->setDecimals ( 1 );
		//_mwheel_turns_input->setButtonSymbols ( QAbstractSpinBox::NoButtons );

		_mwheel_turns_slider = new QSlider ( Qt::Horizontal );
		_mwheel_turns_slider->setRange ( range[0], range[1] );
		_mwheel_turns_slider->setSingleStep ( 1 );
		_mwheel_turns_slider->setPageStep ( 10 );

		connect ( _mwheel_turns_slider, SIGNAL ( valueChanged ( int ) ),
			this, SLOT ( wheel_turns_changed ( int ) ) );

		connect ( _mwheel_turns_input, SIGNAL ( valueChanged ( double ) ),
			this, SLOT ( wheel_turns_changed ( double ) ) );

		connect ( _mwheel_turns_input, SIGNAL ( valueChanged ( double ) ),
			this, SLOT ( change_input() ) );


		// Layout
		QHBoxLayout * lay_input ( new QHBoxLayout );
		lay_input->setContentsMargins ( 0, 0, 0, 0 );
		lay_input->addWidget ( _mwheel_turns_input );
		lay_input->addWidget ( _mwheel_turns_slider );

		QVBoxLayout * lay_mwheel ( new QVBoxLayout );
		lay_mwheel->addWidget ( lbl_info );
		lay_mwheel->addLayout ( lay_input );
		box_mwheel->setLayout ( lay_mwheel );
	}

	// Layout
	QVBoxLayout * lay_wdg ( new QVBoxLayout );
	lay_wdg->setContentsMargins ( 0, 0, 0, 0 );
	lay_wdg->addWidget ( box_mwheel );
	lay_wdg->addStretch ( 1 );

	_page_input = create_page_container ( lay_wdg );
}


void
Settings_View::init_page_sys_tray ( )
{
	_tray_btn_show_icon = new QCheckBox ( tr ( "Show tray icon" ) );
	_tray_btn_on_close = new QCheckBox ( tr ( "Close to tray" ) );

	connect ( _tray_btn_show_icon, SIGNAL ( toggled ( bool ) ),
		this, SLOT ( change_tray_view() ) );

	connect ( _tray_btn_on_close, SIGNAL ( toggled ( bool ) ),
		this, SLOT ( change_tray_view() ) );


	QGroupBox * box_icon ( new QGroupBox );
	box_icon->setTitle ( tr ( "System tray usage" ) );
	{
		QVBoxLayout * lay_gbox ( new QVBoxLayout );
		lay_gbox->addWidget ( _tray_btn_show_icon );
		lay_gbox->addWidget ( _tray_btn_on_close );
		box_icon->setLayout ( lay_gbox );
	}


	QGroupBox * box_balloon ( new QGroupBox );
	box_balloon->setTitle ( tr ( "Notification balloon" ) );
	{
		_tray_btn_balloon_show = new QCheckBox;
		_tray_btn_balloon_show->setText ( tr ( "Show balloon on a volume change" ) );

		connect ( _tray_btn_balloon_show, SIGNAL ( toggled ( bool ) ),
			this, SLOT ( change_tray_balloon() ) );

		QVBoxLayout * lay_balloon_time ( new QVBoxLayout );
		lay_balloon_time->setContentsMargins ( 0, 0, 0, 0 );
		{
			QString str_lt ( tr ( "Balloon lifetime" ) );
			QLabel * lbl_lifetime ( new QLabel ( str_lt + ":" ) );
			//: ms - abbreviation for milliseconds
			QLabel * lbl_ms ( new QLabel ( tr ( "ms" ) ) );

			const int range[2] = { 100, 10000 };

			_tray_balloon_time_input = new QSpinBox;
			_tray_balloon_time_input->setRange ( range[0], range[1] );
			_tray_balloon_time_input->setSingleStep ( 100 );
			_tray_balloon_time_input->setToolTip ( str_lt );

			connect ( _tray_balloon_time_input, SIGNAL ( valueChanged ( int ) ),
				this, SLOT ( change_tray_balloon() ) );

			QHBoxLayout * lay_input ( new QHBoxLayout );
			lay_input->setContentsMargins ( 0, 0, 0, 0 );
			lay_input->addWidget ( lbl_lifetime );
			lay_input->addWidget ( _tray_balloon_time_input );
			lay_input->addWidget ( lbl_ms );
			lay_input->addStretch ( 1 );

			lay_balloon_time->addLayout ( lay_input );
		}
		_tray_balloon_time = new QWidget;
		_tray_balloon_time->setLayout ( lay_balloon_time );

		QVBoxLayout * lay_balloon ( new QVBoxLayout );
		lay_balloon->addWidget ( _tray_btn_balloon_show );
		lay_balloon->addWidget ( _tray_balloon_time );
		box_balloon->setLayout ( lay_balloon );
	}


	QGroupBox * box_device ( new QGroupBox );
	box_device->setTitle ( tr ( "Mini mixer device" ) );
	{
		_tray_btn_device[0] = new QRadioButton ( tr ( "Default" ) );
		_tray_btn_device[1] = new QRadioButton ( tr ( "Current (same as in main mixer window)" ) );
		_tray_btn_device[2] = new QRadioButton ( tr ( "User defined" ) );

		_tray_btn_grp_device = new QButtonGroup ( this );
		_tray_btn_grp_device->addButton ( _tray_btn_device[0], 0 );
		_tray_btn_grp_device->addButton ( _tray_btn_device[1], 1 );
		_tray_btn_grp_device->addButton ( _tray_btn_device[2], 2 );


		_tray_dev_user = new QWidget;
		_tray_dev_user_edit = new QLineEdit;
		QLabel * lbl_user ( new QLabel ( tr ( "User device:" ) ) );
		QLabel * lbl_example ( new QLabel ( tr ( "e.g. hw:0" ) ) );

		connect ( _tray_btn_grp_device, SIGNAL ( buttonClicked ( int ) ),
			this, SLOT ( change_tray_mdev() ) );

		connect ( _tray_dev_user_edit, SIGNAL ( editingFinished() ),
			this, SLOT ( change_tray_mdev() ) );

		QHBoxLayout * lay_dev_user ( new QHBoxLayout );
		lay_dev_user->setContentsMargins ( 0, 0, 0, 0 );
		lay_dev_user->addWidget ( lbl_user );
		lay_dev_user->addWidget ( _tray_dev_user_edit );
		lay_dev_user->addWidget ( lbl_example );
		lay_dev_user->addStretch ( 1 );
		_tray_dev_user->setLayout ( lay_dev_user );

		QVBoxLayout * lay_dev_sel ( new QVBoxLayout );
		lay_dev_sel->addWidget ( _tray_btn_device[0] );
		lay_dev_sel->addWidget ( _tray_btn_device[1] );
		lay_dev_sel->addWidget ( _tray_btn_device[2] );
		lay_dev_sel->addWidget ( _tray_dev_user );
		lay_dev_sel->addStretch ( 1 );

		box_device->setLayout ( lay_dev_sel );
	}


	_tray_wdg_grp_ballon = box_balloon;
	_tray_wdg_grp_device = box_device;

	// Layout
	QVBoxLayout * lay_wdg ( new QVBoxLayout );
	lay_wdg->setContentsMargins ( 0, 0, 0, 0 );
	lay_wdg->addWidget ( box_icon );
	lay_wdg->addSpacing ( _vspace );
	lay_wdg->addWidget ( box_balloon );
	lay_wdg->addSpacing ( _vspace );
	lay_wdg->addWidget ( box_device );
	lay_wdg->addStretch ( 1 );

	_page_sys_tray = create_page_container ( lay_wdg );
}


QWidget *
Settings_View::create_page_container (
	QLayout * lay_n )
{
	QWidget * wdg ( new QWidget );
	wdg->setLayout ( lay_n );

	::Wdg::Scroll_Area_Vertical * vscroll (
		new ::Wdg::Scroll_Area_Vertical );
	vscroll->setFrameStyle ( QFrame::NoFrame );
	vscroll->set_widget ( wdg );
	return vscroll;
}


void
Settings_View::set_setup (
	::Desktop_Items_Setup * setup_n )
{
	_dsetup = setup_n;
	if ( _dsetup != 0 ) {
		update_inputs_values();

		if ( _dsetup->settings_view.page >= num_pages ) {
			_dsetup->settings_view.page = 0;
		}
		_page_selection->setCurrentIndex (
			_page_items[_dsetup->settings_view.page]->index() );
	}
}


void
Settings_View::update_inputs_values ( )
{
	if ( _dsetup == 0 ) {
		return;
	}

	_updating_values = true;

	// Startup
	{
		unsigned int idx ( _dsetup->start_device_mode );
		if ( idx > ::Desktop_Items_Setup::MIXER_DEV_LAST ) {
			idx = 0;
		}
		_start_btn_device[idx]->setChecked ( true );
	}
	_start_dev_user_edit->setText ( _dsetup->start_user_device );

	// Appearance
	_btn_show_sliders_labels->setChecked (
		_dsetup->main_window.mixer_simple.show_slider_value_labels );

	// Input
	{
		const int value ( _dsetup->main_window.inputs.wheel_degrees );
		_mwheel_turns_slider->setValue ( value / 36 );
		_mwheel_turns_input->setValue ( value / 360.0 );
	}

	// System tray
	_tray_btn_on_close->setChecked ( _dsetup->tray_on_close );
	_tray_btn_show_icon->setChecked ( _dsetup->tray_show_icon );

	_tray_btn_balloon_show->setChecked ( _dsetup->tray_view.show_balloon );
	_tray_balloon_time_input->setValue ( _dsetup->tray_view.balloon_lifetime );

	{
		unsigned int idx ( _dsetup->tray_mdev.device_mode );
		if ( idx > ::Tray_Mixer_MDev_Setup::MIXER_DEV_LAST ) {
			idx = 0;
		}
		_tray_btn_device[idx]->setChecked ( true );
	}
	_tray_dev_user_edit->setText ( _dsetup->tray_mdev.user_device );

	update_inputs_vis_state();

	_updating_values = false;
}


void
Settings_View::update_inputs_vis_state ( )
{
	const bool has_setup ( _dsetup != 0 );
	_page_appearance->setEnabled ( has_setup );
	_page_input->setEnabled ( has_setup );
	_page_sys_tray->setEnabled ( has_setup );

	if ( !has_setup ) {
		return;
	}

	_start_dev_user->setEnabled (
		( _dsetup->start_device_mode == ::Desktop_Items_Setup::MIXER_DEV_USER ) );

	{
		bool flag ( _dsetup->tray_on_close || _dsetup->tray_show_icon );
		_tray_wdg_grp_ballon->setEnabled ( flag );
		_tray_wdg_grp_device->setEnabled ( flag );
	}

	_tray_balloon_time->setEnabled (
		_dsetup->tray_view.show_balloon );

	_tray_dev_user->setEnabled (
		( _dsetup->tray_mdev.device_mode == ::Tray_Mixer_MDev_Setup::MIXER_DEV_USER ) );
}


void
Settings_View::page_changed (
	const QModelIndex & cur_n,
	const QModelIndex & )
{
	page_selected ( cur_n );
}


void
Settings_View::page_selected (
	const QModelIndex & index_n )
{
	for ( unsigned int ii=0; ii < num_pages; ++ii ) {
		if ( _page_items[ii]->index() == index_n ) {
			_lay_pages_stack->setCurrentIndex ( ii );
			break;
		}
	}

	if ( _dsetup != 0 ) {
		_dsetup->settings_view.page = _lay_pages_stack->currentIndex();
	}
}


void
Settings_View::wheel_turns_changed (
	int value_n )
{
	_mwheel_turns_input->setValue ( value_n / 10.0 );
}


void
Settings_View::wheel_turns_changed (
	double value_n )
{
	_mwheel_turns_slider->setValue ( value_n * 10.0 );
}


void
Settings_View::change_startup ( )
{
	if ( ( _dsetup == 0 ) || _updating_values ) {
		return;
	}

	bool changed ( false );
	int idx = _start_btn_grp_device->checkedId();
	if ( ( idx >= 0 ) && ( idx <= ::Desktop_Items_Setup::MIXER_DEV_LAST ) ) {
		if ( idx != (int)_dsetup->start_device_mode ) {
			_dsetup->start_device_mode = idx;
			changed = true;
		}
	}

	if ( _start_dev_user_edit->text() != _dsetup->start_user_device ) {
		_dsetup->start_user_device = _start_dev_user_edit->text();
		changed = true;
	}

	if ( changed ) {
		update_inputs_vis_state();
	}
}


void
Settings_View::change_appearance ( )
{
	if ( ( _dsetup == 0 ) || _updating_values ) {
		return;
	}

	bool changed ( false );
	{
		bool & sflag (
			_dsetup->main_window.mixer_simple.show_slider_value_labels );
		if ( _btn_show_sliders_labels->isChecked() != sflag ) {
			sflag = _btn_show_sliders_labels->isChecked();
			changed = true;
		}
	}
	if ( changed ) {
		//::std::cout << "sig_change_mixer_view()\n";
		emit sig_change_mixer_view();
	}
}


void
Settings_View::change_input ( )
{
	if ( ( _dsetup == 0 ) || _updating_values ) {
		return;
	}

	bool changed ( false );

	int value = _mwheel_turns_input->value() * 360.0;
	if ( value >= 0 ) {
		if ( _dsetup->main_window.inputs.wheel_degrees != (unsigned int)value ) {
			_dsetup->main_window.inputs.wheel_degrees = (unsigned int)value;
			changed = true;
		}
	}

	if ( changed ) {
		//::std::cout << "sig_change_input()\n";
		emit sig_change_input();
	}
}


void
Settings_View::change_tray_view  ( )
{
	if ( ( _dsetup == 0 ) || _updating_values ) {
		return;
	}

	bool changed ( false );

	if ( _tray_btn_on_close->isChecked() != _dsetup->tray_on_close ) {
		_dsetup->tray_on_close = _tray_btn_on_close->isChecked();
		changed = true;
	}

	if ( _tray_btn_show_icon->isChecked() != _dsetup->tray_show_icon ) {
		_dsetup->tray_show_icon = _tray_btn_show_icon->isChecked();
		changed = true;
	}

	if ( changed ) {
		//::std::cout << "sig_change_tray_view()\n";
		update_inputs_vis_state();
		emit sig_change_tray_view();
	}
}


void
Settings_View::change_tray_balloon  ( )
{
	if ( ( _dsetup == 0 ) || _updating_values ) {
		return;
	}

	bool changed ( false );

	if ( _tray_btn_balloon_show->isChecked() != _dsetup->tray_view.show_balloon ) {
		_dsetup->tray_view.show_balloon = _tray_btn_balloon_show->isChecked();
		changed = true;
	}

	if ( _tray_balloon_time_input->value() >= 0 ) {
		const unsigned int lifetime ( _tray_balloon_time_input->value() );
		if ( lifetime != _dsetup->tray_view.balloon_lifetime ) {
			_dsetup->tray_view.balloon_lifetime = lifetime;
			changed = true;
		}
	}

	if ( changed ) {
		//::std::cout << "sig_change_tray_balloon()\n";
		update_inputs_vis_state();
		emit sig_change_tray_balloon();
	}
}


void
Settings_View::change_tray_mdev ( )
{
	if ( ( _dsetup == 0 ) || _updating_values ) {
		return;
	}

	bool changed ( false );

	int idx = _tray_btn_grp_device->checkedId();
	if ( ( idx >= 0 ) && ( idx <= ::Tray_Mixer_MDev_Setup::MIXER_DEV_LAST ) ) {
		if ( idx != (int)_dsetup->tray_mdev.device_mode ) {
			_dsetup->tray_mdev.device_mode = idx;
			changed = true;
		}
	}

	if ( _tray_dev_user_edit->text() != _dsetup->tray_mdev.user_device ) {
		_dsetup->tray_mdev.user_device = _tray_dev_user_edit->text();
		changed = true;
	}

	if ( changed ) {
		//::std::cout << "sig_change_tray_mdev()\n";
		update_inputs_vis_state();
		emit sig_change_tray_mdev();
	}
}


} // End of namespace
