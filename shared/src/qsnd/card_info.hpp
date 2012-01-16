//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_qsnd_card_info_hpp__
#define __INC_qsnd_qsnd_card_info_hpp__

#include <QString>
#include "qsnd/alsa.hpp"


namespace QSnd
{


/// @brief Card_Info
///
class Card_Info
{
	// Public methods
	public:

	Card_Info ( );

	Card_Info (
		const int hw_idx_n );

	Card_Info (
		const QString & dev_str_n );


	void
	clear ( );

	bool
	is_clear ( ) const;


	int
	acquire_info (
		const int hw_idx_n );

	int
	acquire_info (
		const QString & dev_str_n );

	int
	acquire_info (
		snd_hctl_t * snd_hctl_n );


	int
	card_index ( ) const;

	const QString &
	card_id ( ) const;

	const QString &
	card_driver ( ) const;

	const QString &
	card_name ( ) const;

	const QString &
	card_long_name ( ) const;

	const QString &
	card_mixer_name ( ) const;

	const QString &
	card_components ( ) const;


	// Private attributes
	private:

	int _card_index;
	QString _strings[6];
};


inline
int
Card_Info::card_index ( ) const
{
	return _card_index;
}


inline
const QString &
Card_Info::card_id ( ) const
{
	return _strings[0];
}


inline
const QString &
Card_Info::card_driver ( ) const
{
	return _strings[1];
}


inline
const QString &
Card_Info::card_name ( ) const
{
	return _strings[2];
}


inline
const QString &
Card_Info::card_long_name ( ) const
{
	return _strings[3];
}


inline
const QString &
Card_Info::card_mixer_name ( ) const
{
	return _strings[4];
}


inline
const QString &
Card_Info::card_components ( ) const
{
	return _strings[5];
}


} // End of namespace


#endif
