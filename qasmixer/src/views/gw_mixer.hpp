//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_views_gw_mixer_hpp__
#define __INC_views_gw_mixer_hpp__

#include "qsnd2/controls_watcher.hpp"
#include "qsnd2/alsa_smi.hpp"
#include "wdg2/gw_widget.hpp"
#include "wdg2/gw_mixer_pad.hpp"
#include "mwdg/mixer_device_setup.hpp"
#include <QScopedPointer>

namespace Views
{


class GW_Mixer :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Mixer (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Mixer ( );

	void
	set_size (
		const QSize & size_n );

	void
	set_mdev_setup (
		const ::MWdg::Mixer_Device_Setup * setup_n );


	// Protected methods
	protected:


	// Private methods
	private:


	// Private attributes
	private:

	const ::MWdg::Mixer_Device_Setup * _mdev_setup;
	QScopedPointer < ::Wdg2::GW_Mixer_Pad > _gw_mixer_pad;
	QScopedPointer < ::QSnd2::ASMI_Controls > _snd_controls;
	::QSnd2::Controls_Watcher _snd_ctl_watcher;
};


} // End of namespace

#endif
