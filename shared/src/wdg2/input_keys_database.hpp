//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_input_keys_database_hpp__
#define __INC_wdg2_input_keys_database_hpp__

#include <map>

namespace Wdg2
{


struct Input_Action
{
	Input_Action ( );

	void
	reset_to_default ( );

	unsigned int key;
	unsigned int default_key;
};


/// @brief Input_Keys_Database
///
class Input_Keys_Database
{
	// Public typedefs
	public:

	typedef ::std::map < unsigned int, ::Wdg2::Input_Action > Action_Map;


	// Public methods
	public:

	Input_Keys_Database ( );

	~Input_Keys_Database ( );


	/// @brief Registers/creates a new input type
	///
	/// @return A new action_id
	unsigned int
	register_action ( );

	void
	unregister_action (
		unsigned int action_id_n );


	const ::Wdg2::Input_Action *
	action (
		unsigned int action_id_n ) const;

	void
	set_action (
		unsigned int action_id_n,
		const ::Wdg2::Input_Action & input_n );


	bool
	action_key_match (
		unsigned int action_id_n,
		unsigned int key_n ) const;



	// Private attributes
	private:

	Action_Map _actions;
	::Wdg2::Input_Action _action_default;
	unsigned int _next_action_id;
};


} // End of namespace

#endif
