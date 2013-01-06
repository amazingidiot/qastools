//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_dpe2_pixmap_request_hpp__
#define __INC_dpe2_pixmap_request_hpp__

#include "flags.hpp"
#include "key_values.hpp"
#include "pixmap.hpp"
#include "pixmap_ref.hpp"

// Forward declarations
namespace dpe2
{
	class Pixmap_Request;
}

namespace dpe2
{

// Type definitions

enum Pixmap_Keys {
	PMK_NONE = 0,
	PMK_WIDTH,
	PMK_HEIGHT,
	PMK_USER = 4096
};

typedef void (*Pixmap_Request_CBFunk)(
	void * contex_n,
	::dpe2::Pixmap_Request * request_n );

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
