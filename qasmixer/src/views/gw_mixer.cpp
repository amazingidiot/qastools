//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "gw_mixer.hpp"

#include "qastools_config.hpp"
#include <iostream>
#include <QCoreApplication>
#include <QKeyEvent>

namespace Views
{

GW_Mixer::GW_Mixer (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_mdev_setup ( 0 )
{
	_gw_mixer_pad.reset ( new ::Wdg2::GW_Mixer_Pad ( scene_db(), this ) );
	_snd_controls.reset ( new ::QSnd2::ASMI_Controls );
}

GW_Mixer::~GW_Mixer ( )
{
}

void
GW_Mixer::set_size (
	const QSize & size_n )
{
	if ( size() != size_n ) {
		::Wdg2::GW_Widget::set_size ( size_n );
		_gw_mixer_pad->set_size ( size() );
	}
}

void
GW_Mixer::set_mdev_setup (
	const ::MWdg::Mixer_Device_Setup * setup_n )
{
	if ( _mdev_setup != 0 ) {
		_gw_mixer_pad->set_snd_controls ( 0 );
		_snd_ctl_watcher.set_controls ( 0 );
		_snd_controls->close();
	}

	_mdev_setup = setup_n;

	if ( _mdev_setup != 0 ) {
		_snd_controls->open ( _mdev_setup->ctl_addr );
		_snd_ctl_watcher.set_controls ( _snd_controls.data() );
		_gw_mixer_pad->set_snd_controls ( _snd_controls.data() );
	}
}


} // End of namespace
