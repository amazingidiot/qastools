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

#include "card_info.hpp"

#include <iostream>


namespace QSnd
{


Card_Info::Card_Info ( ) :
_card_index ( -1 )
{
}


Card_Info::Card_Info (
	const int hw_idx_n ) :
_card_index ( -1 )
{
	acquire_info ( hw_idx_n );
}


Card_Info::Card_Info (
	const QString & dev_str_n ) :
_card_index ( -1 )
{
	acquire_info ( dev_str_n );
}


void
Card_Info::clear ( )
{
	_card_index = -1;
	for ( int ii=0; ii < 6; ++ii ) {
		_strings[ii].clear();
	}
}


bool
Card_Info::is_clear ( ) const
{
	return ( _card_index < 0 );
}


int
Card_Info::acquire_info (
	const int hw_idx_n )
{
	QString dev_str ( "hw:%1" );
	dev_str = dev_str.arg ( hw_idx_n );

	return acquire_info ( dev_str );
}


int
Card_Info::acquire_info (
	const QString & dev_str_n )
{
	// Open control handle
	int err;
	snd_hctl_t * snd_hctl;
	err = snd_hctl_open (
		&snd_hctl,
		dev_str_n.toLocal8Bit().constData(),
		SND_CTL_NONBLOCK );

	if ( err >= 0 ) {
		err = acquire_info ( snd_hctl );
		snd_hctl_close ( snd_hctl );
	}

	return err;
}



int
Card_Info::acquire_info (
	snd_hctl_t * snd_hctl_n )
{
	int err;
	snd_ctl_t * snd_card_ctl ( 0 );
	snd_ctl_card_info_t * snd_card_info ( 0 );

	snd_ctl_card_info_alloca ( &snd_card_info );
	snd_card_ctl = snd_hctl_ctl ( snd_hctl_n );

	// Get card information
	err = snd_ctl_card_info ( snd_card_ctl, snd_card_info );
	if ( err >= 0 ) {
		_card_index = snd_ctl_card_info_get_card ( snd_card_info );
		_strings[0] = snd_ctl_card_info_get_id ( snd_card_info );
		_strings[1] = snd_ctl_card_info_get_driver ( snd_card_info );
		_strings[2] = snd_ctl_card_info_get_name ( snd_card_info );
		_strings[3] = snd_ctl_card_info_get_longname ( snd_card_info );
		_strings[4] = snd_ctl_card_info_get_mixername ( snd_card_info );
		_strings[5] = snd_ctl_card_info_get_components ( snd_card_info );
	} else {
		clear();
	}

	return err;
}


} // End of namespace
