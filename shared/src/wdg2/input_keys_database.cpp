//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "input_keys_database.hpp"
#include <cassert>

namespace Wdg2
{


inline
Input_Action::Input_Action ( ) :
key ( 0 ),
default_key ( 0 )
{
}

void
Input_Action::reset_to_default ( )
{
	key = default_key;
}



Input_Keys_Database::Input_Keys_Database ( ) :
_next_action_id ( 1 )
{
}

Input_Keys_Database::~Input_Keys_Database ( )
{
}

unsigned int
Input_Keys_Database::register_action ( )
{
	const unsigned int act_key ( _next_action_id );
	++_next_action_id;

	_actions.insert (
		_actions.end(),
		Action_Map::value_type ( act_key, ::Wdg2::Input_Action() ) );
	return act_key;
}

void
Input_Keys_Database::unregister_action (
	unsigned int action_id_n )
{
	_actions.erase ( action_id_n );
}

const ::Wdg2::Input_Action *
Input_Keys_Database::action (
	unsigned int action_id_n ) const
{
	const ::Wdg2::Input_Action * res ( 0 );
	Action_Map::const_iterator it ( _actions.find ( action_id_n ) );
	if ( it != _actions.end() ) {
		res = &it->second;
	}
	return res;
}

void
Input_Keys_Database::set_action (
	unsigned int action_id_n,
	const ::Wdg2::Input_Action & action_n )
{
	Action_Map::iterator it ( _actions.find ( action_id_n ) );
	if ( it != _actions.end() ) {
		it->second = action_n;
	}
}

bool
Input_Keys_Database::action_key_match (
	unsigned int action_id_n,
	unsigned int key_n ) const
{
	bool res ( false );
	const ::Wdg2::Input_Action * act ( action ( action_id_n ) );
	if ( act != 0 ) {
		res = ( act->key == key_n );
	}
	return res;
}


} // End of namespace
