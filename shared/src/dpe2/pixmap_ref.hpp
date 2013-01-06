//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_dpe2_pixmap_handle_hpp__
#define __INC_dpe2_pixmap_handle_hpp__

#include "key_values.hpp"
#include "pixmap.hpp"
#include <QTime>

// Forward declaration
namespace dpe2
{
	class Painter;
	class Pixmap_Paint_Waiter;
	class Pixmap_IRef0;
	class Pixmap_IRef1;
	class Pixmap_Ref;
}

namespace dpe2
{

/// @brief Pixmap_IRef0
///
class Pixmap_IRef0
{
	// Public methods
	public:

	Pixmap_IRef0 (
		::dpe2::Painter * painter_n );


	::dpe2::Pixmap &
	pixmap ( );

	const ::dpe2::Pixmap &
	pixmap ( ) const;

	::dpe2::Painter *
	painter ( ) const;


	unsigned int
	num_iref1 ( ) const;

	::dpe2::Pixmap_IRef1 &
	iref1 (
		unsigned int idx_n );

	::dpe2::Pixmap_IRef1 *
	create_iref1 (
		const ::dpe2::Key_Values & kvals_n );

	void
	destroy_iref1 (
		::dpe2::Pixmap_IRef1 * iref1_n );


	::dpe2::Pixmap_Paint_Waiter *
	waiter ( ) const;

	void
	set_waiter (
		::dpe2::Pixmap_Paint_Waiter * waiter_n );


	// Private attributes
	private:

	typedef ::std::vector < ::dpe2::Pixmap_IRef1 * > IRef1_List;

	::dpe2::Pixmap _pixmap;
	::dpe2::Painter * _painter;
	IRef1_List _iref1s;
	::dpe2::Pixmap_Paint_Waiter * _waiter;
};

inline
::dpe2::Pixmap &
Pixmap_IRef0::pixmap ( )
{
	return _pixmap;
}

inline
const ::dpe2::Pixmap &
Pixmap_IRef0::pixmap ( ) const
{
	return _pixmap;
}

inline
::dpe2::Painter *
Pixmap_IRef0::painter ( ) const
{
	return _painter;
}

inline
unsigned int
Pixmap_IRef0::num_iref1 ( ) const
{
	return _iref1s.size();
}

inline
::dpe2::Pixmap_IRef1 &
Pixmap_IRef0::iref1 (
	unsigned int idx_n )
{
	return *_iref1s[idx_n];
}

inline
::dpe2::Pixmap_Paint_Waiter *
Pixmap_IRef0::waiter ( ) const
{
	return _waiter;
}

inline
void
Pixmap_IRef0::set_waiter (
	::dpe2::Pixmap_Paint_Waiter * waiter_n )
{
	_waiter = waiter_n;
}


/// @brief Pixmap_IRef1
///
class Pixmap_IRef1
{
	// Public methods
	public:

	Pixmap_IRef1 (
		::dpe2::Pixmap_IRef0 * iref0_n,
		const ::dpe2::Key_Values & kvals_n );


	const ::dpe2::Key_Values &
	key_values ( ) const;

	void
	set_key_values (
		const ::dpe2::Key_Values & vset_n );


	::dpe2::Pixmap_IRef0 *
	iref0 ( ) const;

	::dpe2::Pixmap &
	pixmap ( ) const;


	unsigned int
	num_refs ( ) const;

	void
	ref_one ( );

	void
	deref_one ( );


	// Private attributes
	private:

	::dpe2::Key_Values _key_values;
	::dpe2::Pixmap_IRef0 * _iref0;

	unsigned int _num_refs;
	QTime remove_time;
};

inline
Pixmap_IRef1::Pixmap_IRef1 (
	::dpe2::Pixmap_IRef0 * iref0_n,
	const ::dpe2::Key_Values & kvals_n ) :
_key_values ( kvals_n ),
_iref0 ( iref0_n ),
_num_refs ( 0 )
{
}

inline
const ::dpe2::Key_Values &
Pixmap_IRef1::key_values ( ) const
{
	return _key_values;
}

inline
void
Pixmap_IRef1::set_key_values (
	const ::dpe2::Key_Values & vset_n )
{
	_key_values = vset_n;
}

inline
::dpe2::Pixmap_IRef0 *
Pixmap_IRef1::iref0 ( ) const
{
	return _iref0;
}

inline
::dpe2::Pixmap &
Pixmap_IRef1::pixmap ( ) const
{
	return iref0()->pixmap();
}

inline
unsigned int
Pixmap_IRef1::num_refs ( ) const
{
	return _num_refs;
}

inline
void
Pixmap_IRef1::ref_one ( )
{
	_num_refs += 1;
}

inline
void
Pixmap_IRef1::deref_one ( )
{
	_num_refs -= 1;
}


/// @brief Pixmap_Ref
///
class Pixmap_Ref
{
	// Public methods
	public:

	Pixmap_Ref ( );

	Pixmap_Ref (
		::dpe2::Pixmap_IRef1 * ref1_n );

	void
	clear ( );

	void
	swap (
		::dpe2::Pixmap_Ref & ref_n );

	bool
	is_valid ( ) const;

	void
	set_iref1 (
		::dpe2::Pixmap_IRef1 * ref1_n );

	::dpe2::Pixmap *
	pixmap ( ) const;

	::dpe2::Pixmap_IRef1 *
	iref1 ( ) const;


	// Private attributes
	private:

	::dpe2::Pixmap * _pixmap;
	::dpe2::Pixmap_IRef1 * _iref1;
};

inline
Pixmap_Ref::Pixmap_Ref ( ) :
_pixmap ( 0 ),
_iref1 ( 0 )
{
}

inline
bool
Pixmap_Ref::is_valid ( ) const
{
	return ( _pixmap != 0 );
}

inline
void
Pixmap_Ref::swap (
	::dpe2::Pixmap_Ref & ref_n )
{
	::std::swap ( _iref1, ref_n._iref1 );
	::std::swap ( _pixmap, ref_n._pixmap );
}

inline
::dpe2::Pixmap *
Pixmap_Ref::pixmap ( ) const
{
	return _pixmap;
}

inline
::dpe2::Pixmap_IRef1 *
Pixmap_Ref::iref1 ( ) const
{
	return _iref1;
}


} // End of namespace

#endif
