//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_mwdg_inputs_setup_hpp__
#define __INC_mwdg_inputs_setup_hpp__

#include <QKeySequence>
#include <QString>


namespace MWdg
{


class Inputs_Setup
{
	// Public methods
	public:

	Inputs_Setup ( );

	void
	update_translation ( );


	// Public attributes
	public:

	unsigned int wheel_degrees;

	QKeySequence ks_toggle_joined;
	QKeySequence ks_level_channels;
	QKeySequence ks_mute_volumes;

	QKeySequence ks_toggle_vis_stream[2];

	QString ts_split_channels;
	QString ts_join_channels;
	QString ts_level_channels;
};


} // End of namespace

#endif
