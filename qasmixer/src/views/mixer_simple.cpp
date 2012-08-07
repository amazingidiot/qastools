//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "mixer_simple.hpp"

#include "qastools_config.hpp"
#include "mwdg/inputs_setup.hpp"
#include "mwdg/mixer_style.hpp"
#include "mwdg/mixer_device_setup.hpp"
#include "views/mixer_simple_setup.hpp"
#include "views/message_widget.hpp"
#include "wdg2/theme_sbevel.hpp"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QWidgetAction>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QKeyEvent>

#include <iostream>


namespace Views
{


Mixer_Simple::Mixer_Simple (
	QWidget * parent_n ) :
::Views::View_Base ( parent_n ),
_view_setup ( 0 ),
_status_group_idx ( ~0 ),
_status_column_idx ( ~0 )
{
	// Strings and icons
	_act_stream_text[0] = tr ( "Show playback" );
	_act_stream_text[1] = tr ( "Show capture" );
	_act_stream_ttip[0] = tr ( "Show playback elements" );
	_act_stream_ttip[1] = tr ( "Show capture elements" );

	{
		const char * fd_app[3] = {
			"show-playback",
			"show-capture"
		};
		const char * fd_def[3] = {
			"media-playback-start",
			"media-record"
		};

		QString icon_path_base ( INSTALL_DIR_APP_ICONS );
		icon_path_base.append ( "/" );
		for ( unsigned int ii=0; ii < 2; ++ii ) {
			QIcon icon;
			{
				QString icon_path ( icon_path_base );
				icon_path.append ( fd_app[ii] );
				icon_path.append ( ".svg" );
				QFileInfo finfo ( icon_path );
				if ( finfo.exists() && finfo.isReadable() ) {
					icon = QIcon ( icon_path );
				} else {
					icon = QIcon::fromTheme ( fd_def[ii] );
				}
			}
			_act_stream_icon[ii] = icon;
		}
	}

	// Mixer object 2
	{
		_snd_controls = new ::QSnd2::ASMI_Controls;
	}

	// Scene database
	{
		_scene_db = new ::Wdg2::Scene_Database;
		_scene_db->install_theme ( new ::Wdg2::Theme_SBevel );
	}

	// Sliders pad 2
	{
		_sliders_pad2 = new ::Wdg2::Sliders_Pad ( _scene_db );
	}

	// Vertical splitter
	{
		_mixer_split.setOrientation ( Qt::Vertical );
		_mixer_split.addWidget ( _sliders_pad2 );

		QVBoxLayout * lay_vbox ( new QVBoxLayout );
		lay_vbox->setContentsMargins ( 0, 0, 0, 0 );
		lay_vbox->addWidget ( &_mixer_split );
		_wdg_areas.setLayout ( lay_vbox );

		lay_stack()->addWidget ( &_wdg_areas );
	}
}


Mixer_Simple::~Mixer_Simple ( )
{
	set_mdev_setup ( 0 );
	set_view_setup ( 0 );
}


void
Mixer_Simple::set_mdev_setup (
	const ::MWdg::Mixer_Device_Setup * setup_n )
{
	if ( mdev_setup() != 0 ) {
		clear_view();

		_snd_ctl_watcher.set_controls ( 0 );
		_sliders_pad2->set_snd_controls ( 0 );
		_snd_controls->close();
	}

	::Views::View_Base::set_mdev_setup ( setup_n );

	if ( mdev_setup() != 0 ) {
		_snd_controls->open ( mdev_setup()->ctl_addr );
		_snd_ctl_watcher.set_controls ( _snd_controls );
		_sliders_pad2->set_snd_controls ( _snd_controls );
		setup_view();
	}
}


void
Mixer_Simple::set_inputs_setup (
	const ::MWdg::Inputs_Setup * setup_n )
{
	::Views::View_Base::set_inputs_setup ( setup_n );
}


void
Mixer_Simple::set_view_setup (
	::Views::View_Base_Setup * setup_n )
{
	clear_view();
	_view_setup = dynamic_cast < ::Views::Mixer_Simple_Setup * > ( setup_n );
	setup_view();
}


void
Mixer_Simple::clear_view ( )
{
	if ( ( mdev_setup() == 0 ) || ( _view_setup == 0 ) ) {
		return;
	}
}


void
Mixer_Simple::setup_view ( )
{
	if ( ( mdev_setup() == 0 ) || ( _view_setup == 0 ) ) {
		return;
	}

	// Fix show stream flags on demand
	if ( ! ( _view_setup->show_stream[0] || _view_setup->show_stream[1] ) ) {
		_view_setup->show_stream[0] = true;
	}

	// The stream dirs shown are temporarily overridden for
	// devices with only one stream direction type
	bool show_stream[2];
	show_stream[0] = _view_setup->show_stream[0];
	show_stream[1] = _view_setup->show_stream[1];

	_smixer_setup.show_stream[0] = show_stream[0];
	_smixer_setup.show_stream[1] = show_stream[1];
	_smixer_setup.show_slider_value_labels = _view_setup->show_slider_value_labels;
	_smixer_setup.wdg_style_db = _view_setup->wdg_style_db;
	_smixer_setup.image_alloc = _view_setup->image_alloc;

	unsigned int lay_stack_idx ( 0 );
	if ( mdev_setup()->ctl_addr.isEmpty() ) {
		message_wdg()->set_no_device();
	} else {
		if ( _snd_controls->is_open() ) {
			lay_stack_idx = 1;
		} else {
			message_wdg()->set_mixer_open_fail (
				mdev_setup()->ctl_addr,
				_snd_controls->err_message(),
				_snd_controls->err_func() );
		}
	}

	lay_stack()->setCurrentIndex ( lay_stack_idx );
}


void
Mixer_Simple::show_playback (
	bool flag_n )
{
	if ( _view_setup == 0 ) {
		return;
	}
	if ( flag_n != _view_setup->show_stream[0] ) {
		clear_view();

		_view_setup->show_stream[0] = flag_n;
		if ( !_view_setup->show_stream[0] &&
			!_view_setup->show_stream[1] )
		{
			_view_setup->show_stream[1] = true;
		}

		setup_view();
		setFocus();
	}
}


void
Mixer_Simple::show_capture (
	bool flag_n )
{
	if ( _view_setup == 0 ) {
		return;
	}
	if ( flag_n != _view_setup->show_stream[1] ) {
		clear_view();

		_view_setup->show_stream[1] = flag_n;
		if ( !_view_setup->show_stream[0] &&
			!_view_setup->show_stream[1] )
		{
			_view_setup->show_stream[0] = true;
		}

		setup_view();
		setFocus();
	}
}


void
Mixer_Simple::toggle_show_playback ( )
{
	if ( _view_setup != 0 ) {
		show_playback ( !_view_setup->show_stream[0] );
	}
}

void
Mixer_Simple::toggle_show_capture ( )
{
	if ( _view_setup != 0 ) {
		show_capture ( !_view_setup->show_stream[1] );
	}
}

void
Mixer_Simple::keyPressEvent (
	QKeyEvent * event_n )
{
	bool handled ( false );
	if ( inputs_setup() != 0 ) {
		handled = true;
		const QKeySequence kseq ( event_n->key() );
		if ( kseq == inputs_setup()->ks_toggle_vis_stream[0] ) {
			toggle_show_playback();
		} else if ( kseq == inputs_setup()->ks_toggle_vis_stream[1] ) {
			toggle_show_capture();
		} else {
			handled = false;
		}
	}
	if ( !handled ) {
		::Views::View_Base::keyPressEvent ( event_n );
	}
}


} // End of namespace
