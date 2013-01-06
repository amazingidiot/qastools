//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_flags_hpp__
#define __INC_flags_hpp__


/// @brief A set of binary flags with set/unset/test interface
///
class Flags
{
	// Public methods
	public:

	Flags (
		unsigned int flags_n = 0 );

	unsigned int
	flags ( ) const;

	void
	set_flags (
		unsigned int flags_n );

	void
	clear ( );

	bool
	has_any (
		unsigned int flags_n ) const;

	bool
	has_all (
		unsigned int flags_n ) const;

	void
	set (
		unsigned int flags_n );

	void
	unset (
		unsigned int flags_n );

	void
	set (
		unsigned int flags_n,
		bool on_n );


	bool
	operator== (
		const ::Flags & flags_n ) const;

	bool
	operator!= (
		const ::Flags & flags_n ) const;

	bool
	operator== (
		unsigned int val_n ) const;

	bool
	operator!= (
		unsigned int val_n ) const;


	// Private attributes
	private:

	unsigned int _flags;
};

inline
Flags::Flags (
	unsigned int flags_n ) :
_flags ( flags_n )
{
}

inline
unsigned int
Flags::flags ( ) const
{
	return _flags;
}

inline
void
Flags::set_flags (
	unsigned int flags_n )
{
	_flags = flags_n;
}

inline
void
Flags::clear ( )
{
	set_flags ( 0 );
}

inline
bool
Flags::has_any (
	unsigned int flags_n ) const
{
	return ( ( _flags & flags_n ) != 0 );
}

inline
bool
Flags::has_all (
	unsigned int flags_n ) const
{
	return ( ( _flags & flags_n ) == flags_n );
}

inline
void
Flags::set (
	unsigned int flags_n )
{
	_flags |= flags_n;
}

inline
void
Flags::unset (
	unsigned int flags_n )
{
	_flags &= ~flags_n;
}

inline
void
Flags::set (
	unsigned int flags_n,
	bool on_n )
{
	if ( on_n ) {
		set ( flags_n );
	} else {
		unset ( flags_n );
	}
}

inline
bool
Flags::operator== (
	const ::Flags & flags_n ) const
{
	return ( flags() == flags_n.flags() );
}

inline
bool
Flags::operator!= (
	const ::Flags & flags_n ) const
{
	return ( flags() != flags_n.flags() );
}

inline
bool
Flags::operator== (
	unsigned int val_n ) const
{
	return ( _flags == val_n );
}

inline
bool
Flags::operator!= (
	unsigned int val_n ) const
{
	return ( _flags != val_n );
}


#endif
