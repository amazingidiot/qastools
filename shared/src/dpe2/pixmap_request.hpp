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
#include "key_values.hpp"
#include "pixmap.hpp"
#include "pixmap_ref.hpp"


namespace dpe2
{

class Pixmap_Request;
typedef void (*Pixmap_Request_CBFunk)( void * contex_n, ::dpe2::Pixmap_Request * request_n );

enum Request_States {
	RS_FREE          = 0,
	RS_PROCESSING    = ( 1 << 0 ),
	RS_NEEDS_UPDATE  = ( 1 << 1 )
};


/// @brief Pixmap_Request
///
class Pixmap_Request
{
	// Public methods
	public:

	Pixmap_Request ( );

	void
	call_back ( );


	// Public attributes
	public:

	::dpe2::Pixmap_Ref pxm_ref;
	::dpe2::Key_Values kvals;
	unsigned int key; // free to change general purpose identification key
	::Flags state;
	void * cb_context;
	Pixmap_Request_CBFunk cb_func;
};

inline
Pixmap_Request::Pixmap_Request ( ) :
key ( 0 ),
state ( RS_FREE ),
cb_context ( 0 ),
cb_func ( 0 )
{
}

inline
void
Pixmap_Request::call_back ( )
{
	if ( cb_func != 0 ) {
		cb_func ( cb_context, this );
	}
}


} // End of namespace

#endif
