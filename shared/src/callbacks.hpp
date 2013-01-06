//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_callbacks_hpp__
#define __INC_callbacks_hpp__


/// @brief Anonymous context callback class
///
struct Context_Callback
{
	typedef void (*Func_t)( void * context_n );

	Context_Callback (
		void * context_n = 0,
		Func_t func_n = 0 );

	void
	reset ( );

	void *
	context ( ) const;

	void
	set_context (
		void * context_n );


	Func_t
	func ( ) const;

	void
	set_func (
		Func_t func_n );


	void
	set (
		void * context_n,
		Func_t func_n);


	bool
	is_valid ( ) const;

	void
	call ( ) const;

	void
	call_if_valid ( ) const;


	// Private attributes
	private:

	void * _context;
	Func_t _func;
};

inline
Context_Callback::Context_Callback (
	void * context_n,
	Func_t func_n ) :
_context ( context_n ),
_func ( func_n )
{
}

inline
void
Context_Callback::reset ( )
{
	_context = 0;
	_func = 0;
}

inline
void *
Context_Callback::context ( ) const
{
	return _context;
}

inline
void
Context_Callback::set_context (
	void * context_n )
{
	_context = context_n;
}

inline
::Context_Callback::Func_t
Context_Callback::func ( ) const
{
	return _func;
}

inline
void
Context_Callback::set_func (
	Func_t func_n )
{
	_func = func_n;
}

inline
void
Context_Callback::set (
	void * context_n,
	Func_t func_n )
{
	_context = context_n;
	_func = func_n;
}

inline
bool
Context_Callback::is_valid ( ) const
{
	return ( ( _func != 0 ) && ( _context != 0 ) );
}

inline
void
Context_Callback::call ( ) const
{
	_func ( _context );
}

inline
void
Context_Callback::call_if_valid ( ) const
{
	if ( is_valid() ) {
		call();
	}
}



/// @brief Anonymous context callback class with unsigned int argument
///
struct Context_Callback_UInt
{
	typedef void (*Func_t)( void * context_n, unsigned int val_n );

	Context_Callback_UInt (
		void * context_n = 0,
		Func_t func_n = 0,
		unsigned int val_n = 0 );

	void
	reset ( );

	void *
	context ( ) const;

	void
	set_context (
		void * context_n );

	Func_t
	func ( ) const;

	void
	set_func (
		Func_t func_n );

	unsigned int
	value ( ) const;

	void
	set_value (
		unsigned int value_n );

	void
	set (
		void * context_n,
		Func_t func_n );

	void
	set (
		void * context_n,
		Func_t func_n,
		unsigned int value_n );

	bool
	is_valid ( ) const;

	void
	call ( ) const;

	void
	call_if_valid ( ) const;


	// Private attributes
	private:

	void * _context;
	Func_t _func;
	unsigned int _value;
};

inline
Context_Callback_UInt::Context_Callback_UInt (
	void * context_n,
	Func_t func_n,
	unsigned int val_n ) :
_context ( context_n ),
_func ( func_n ),
_value ( val_n )
{
}

inline
void
Context_Callback_UInt::reset ( )
{
	_context = 0;
	_func = 0;
	_value = 0;
}

inline
void *
Context_Callback_UInt::context ( ) const
{
	return _context;
}

inline
void
Context_Callback_UInt::set_context (
	void * context_n )
{
	_context = context_n;
}

inline
::Context_Callback_UInt::Func_t
Context_Callback_UInt::func ( ) const
{
	return _func;
}

inline
void
Context_Callback_UInt::set_func (
	Func_t func_n )
{
	_func = func_n;
}

inline
unsigned int
Context_Callback_UInt::value ( ) const
{
	return _value;
}

inline
void
Context_Callback_UInt::set_value (
	unsigned int value_n )
{
	_value = value_n;
}

inline
void
Context_Callback_UInt::set (
	void * context_n,
	Func_t func_n )
{
	_context = context_n;
	_func = func_n;
}

inline
void
Context_Callback_UInt::set (
	void * context_n,
	Func_t func_n,
	unsigned int value_n )
{
	_context = context_n;
	_func = func_n;
	_value = value_n;
}

inline
bool
Context_Callback_UInt::is_valid ( ) const
{
	return ( ( _context != 0 ) && ( _func != 0 ) );
}

inline
void
Context_Callback_UInt::call ( ) const
{
	_func ( _context, _value );
}

inline
void
Context_Callback_UInt::call_if_valid ( ) const
{
	if ( is_valid() ) {
		call();
	}
}

#endif
