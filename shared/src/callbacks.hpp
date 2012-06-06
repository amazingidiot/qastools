//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_callbacks_hpp__
#define __INC_callbacks_hpp__

#include <QList>
#include <QString>


/// @brief Context plus function callback reference class
///
struct Context_Callback
{
	typedef void (*Func)( void * context_n );

	Context_Callback (
		void * context_n = 0,
		Func func_n = 0 );

	bool
	valid ( ) const;

	void
	call ( ) const;

	void
	call_if_valid ( ) const;

	void * context;
	Func func;
};

inline
Context_Callback::Context_Callback (
	void * context_n,
	Func func_n ) :
context ( context_n ),
func ( func_n )
{
}

inline
bool
Context_Callback::valid ( ) const
{
	return ( ( context != 0 ) && ( func != 0 ) );
}

inline
void
Context_Callback::call ( ) const
{
	func ( context );
}

inline
void
Context_Callback::call_if_valid ( ) const
{
	if ( valid() ) {
		call();
	}
}


#endif
