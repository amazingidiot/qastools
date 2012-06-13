//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_pixmap_request_hpp__
#define __INC_dpe2_pixmap_request_hpp__

#include "flags.hpp"
#include "callbacks.hpp"
#include "key_values.hpp"
#include "pixmap.hpp"
#include "pixmap_handle.hpp"
#include <vector>


namespace dpe2
{


/// @brief Pixmap_Request
///
class Pixmap_Request
{
	// Public typedefs
	public:

	enum Flags {
		IS_RETURN = ( 1 << 0 )
	};


	// Public methods
	public:

	Pixmap_Request ( );

	bool
	is_return ( ) const;

	void
	set_is_return (
		bool flag_n );


	// Public attributes
	public:

	::dpe2::Pixmap_Ref2 pixmap_ref;
	::dpe2::Key_Values key_values;
	::Flags _flags;
	::Context_Callback finished_callback;
};

inline
Pixmap_Request::Pixmap_Request ( )
{
}

inline
bool
Pixmap_Request::is_return ( ) const
{
	return _flags.has_any ( ::dpe2::Pixmap_Request::IS_RETURN );
}

inline
void
Pixmap_Request::set_is_return (
	bool flag_n )
{
	return _flags.set ( ::dpe2::Pixmap_Request::IS_RETURN, flag_n );
}


} // End of namespace

#endif
