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

#ifndef __INC_mview_settings_view_hpp__
#define __INC_mview_settings_view_hpp__

#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QStackedLayout>
#include <QStandardItem>
#include <QModelIndex>


// Forward declaration
class Desktop_Items_Setup;
namespace Wdg {
	class Tree_View_KV;
}


namespace Views
{


/// @brief Settings_View
///
class Settings_View :
	public QWidget
{
	Q_OBJECT

	// Public methods
	public:

	Settings_View (
		QWidget * parent = 0 );

	~Settings_View ( );


	void
	set_setup (
		::Desktop_Items_Setup * setup_n );


	// Signals
	signals:

	void
	sig_change_mixer_view ( );

	void
	sig_change_input ( );

	void
	sig_change_tray_view ( );

	void
	sig_change_tray_mdev ( );

	void
	sig_change_tray_balloon ( );


	void
	sig_close ( );


	// Protected slots
	protected slots:

	void
	page_selected (
		const QModelIndex & index_n );


	void
	change_startup ( );

	void
	change_appearance ( );

	void
	change_input ( );

	void
	change_tray_view ( );

	void
	change_tray_mdev ( );

	void
	change_tray_balloon ( );


	void
	wheel_turns_changed (
		int value_n );

	void
	wheel_turns_changed (
		double value_n );


	// Protected methods
	protected:

	void
	init_page_startup ( );

	void
	init_page_appearance ( );

	void
	init_page_input ( );

	void
	init_page_sys_tray ( );

	QWidget *
	create_page_container (
		QLayout * lay_n );


	void
	update_inputs_values ( );

	void
	update_inputs_vis_state ( );


	// Private attributes
	private:

	::Desktop_Items_Setup * _dsetup;

	static const unsigned int num_pages = 4;

	// Pages
	::Wdg::Tree_View_KV * _tree_view;
	QStackedLayout * _lay_pages_stack;
	QWidget * _page_startup;
	QWidget * _page_appearance;
	QWidget * _page_input;
	QWidget * _page_sys_tray;
	QStandardItem * _page_items[num_pages];

	// Startup
	QButtonGroup * _start_btn_grp_device;
	QRadioButton * _start_btn_device[3];
	QWidget * _start_dev_user;
	QLineEdit * _start_dev_user_edit;

	// Appearance
	QCheckBox * _btn_show_sliders_labels;

	// Input
	QString _mwheel_degrees_mask;
	QDoubleSpinBox * _mwheel_turns_input;
	QSlider * _mwheel_turns_slider;

	// System tray usage
	QWidget * _tray_minimize;
	QCheckBox * _tray_btn_on_close;
	QCheckBox * _tray_btn_show_icon;

	QWidget * _tray_wdg_grp_ballon;
	QCheckBox * _tray_btn_balloon_show;
	QWidget * _tray_balloon_time;
	QSpinBox * _tray_balloon_time_input;

	QWidget * _tray_wdg_grp_device;
	QButtonGroup * _tray_btn_grp_device;
	QRadioButton * _tray_btn_device[3];

	QWidget * _tray_dev_user;
	QLineEdit * _tray_dev_user_edit;


	// Layout & Styling
	QLocale loc;
	bool _updating_values;
	unsigned int _vspace;
};


} // End of namespace


#endif
