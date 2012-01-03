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

#ifndef __INC_mixer_switches_proxy_group_hpp__
#define __INC_mixer_switches_proxy_group_hpp__

#include <QObject>
#include <QString>
#include <QList>
#include <QTimer>

#include "wdg/pad_proxies_group.hpp"
#include "mixer_switches_proxy_enum.hpp"
#include "mixer_switches_proxy_switch.hpp"
#include "mixer_separation_info.hpp"


namespace MWdg
{


/// @brief Mixer_Switches_Proxies_Group
///
class Mixer_Switches_Proxies_Group :
	public ::Wdg::Pad_Proxies_Group
{
	Q_OBJECT


	// Public methods
	public:

	Mixer_Switches_Proxies_Group (
		QObject * parent_n = 0 );

	~Mixer_Switches_Proxies_Group ( );


	// Mixer_Simple_Elem

	::QSnd::Mixer_Simple_Elem *
	mixer_simple_elem ( ) const;

	virtual
	void
	set_mixer_simple_elem (
		::QSnd::Mixer_Simple_Elem * selem_n );


	// Snd direction

	unsigned char
	snd_dir ( ) const;

	void
	set_snd_dir (
		unsigned char dir_n );


	// Is joined flag

	bool
	is_joined ( ) const;

	void
	set_is_joined (
		bool flag_n );


	// Is visible flag

	bool
	is_visible ( ) const;

	void
	set_is_visible (
		bool flag_n );


	// Notify value change

	bool
	notify_value_change ( ) const;

	void
	set_notify_value_change (
		bool flag_n );


	// Separation info

	bool
	can_be_separated ( ) const;

	bool
	should_be_separated ( ) const;

	bool
	needs_separation ( ) const;


	bool
	separation_request ( ) const;

	void
	set_separation_request (
		bool flag_n );


	// Public slots
	public slots:

	void
	update_mixer_values ( );


	// Protected slots
	protected slots:

	void
	timer_separation_check ( );


	// Private attributes
	private:

	::QSnd::Mixer_Simple_Elem * _mixer_simple_elem;
	unsigned char _snd_dir;
	bool _is_joined;
	bool _is_visible;
	bool _notify_value_change;
	bool _separation_request;
	QTimer _separation_timer;
};


inline
::QSnd::Mixer_Simple_Elem *
Mixer_Switches_Proxies_Group::mixer_simple_elem ( ) const
{
	return _mixer_simple_elem;
}


inline
unsigned char
Mixer_Switches_Proxies_Group::snd_dir ( ) const
{
	return _snd_dir;
}


inline
bool
Mixer_Switches_Proxies_Group::is_joined ( ) const
{
	return _is_joined;
}


inline
bool
Mixer_Switches_Proxies_Group::is_visible ( ) const
{
	return _is_visible;
}


inline
bool
Mixer_Switches_Proxies_Group::notify_value_change ( ) const
{
	return _notify_value_change;
}


inline
bool
Mixer_Switches_Proxies_Group::separation_request ( ) const
{
	return _separation_request;
}


} // End of namespace


#endif
