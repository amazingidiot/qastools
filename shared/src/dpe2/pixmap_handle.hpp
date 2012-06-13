//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_pixmap_handle_hpp__
#define __INC_dpe2_pixmap_handle_hpp__

#include "key_values.hpp"
#include <QTime>
#include <QScopedPointer>
#include <QAtomicInt>

// Forward declaration
namespace dpe2
{
	class Pixmap;
	class Painter;

	class Pixmap_Ref0;
	class Pixmap_Ref1;
	class Pixmap_Ref2;
}

namespace dpe2
{


/// @brief Pixmap_Handle
///
class Pixmap_Handle
{
	// Public methods
	public:

	Pixmap_Handle (
		::dpe2::Painter * painter_n = 0 );

	~Pixmap_Handle ( );


	// Public attributes
	public:

	QScopedPointer < ::dpe2::Pixmap > pixmap;
	::dpe2::Key_Values id_values;
	::dpe2::Painter * painter;

	unsigned int num_users;
	QTime remove_time;
};


/// @brief Pixmap_Ref0
///
class Pixmap_Ref0
{
	// Public methods
	public:

	Pixmap_Ref0 ( );

	::dpe2::Pixmap *
	pixmap ( ) const;

	unsigned int
	num_refs ( ) const;

	::dpe2::Pixmap_Ref1 *
	ref1 (
		unsigned int idx_n ) const;

	void
	append_ref (
		::dpe2::Pixmap_Ref1 * ref_n );

	void
	remove_ref (
		::dpe2::Pixmap_Ref1 * ref_n );


	// Private attributes
	private:

	QScopedPointer < ::dpe2::Pixmap > _pixmap;
	QList < ::dpe2::Pixmap_Ref1 * > _refs;
};

inline
::dpe2::Pixmap *
Pixmap_Ref0::pixmap ( ) const
{
	return _pixmap.data();
}

inline
unsigned int
Pixmap_Ref0::num_refs ( ) const
{
	return _refs.size();
}

inline
::dpe2::Pixmap_Ref1 *
Pixmap_Ref0::ref1 (
	unsigned int idx_n ) const
{
	return _refs[idx_n];
}

inline
void
Pixmap_Ref0::append_ref (
	::dpe2::Pixmap_Ref1 * ref_n )
{
	_refs.append ( ref_n );
}

inline
void
Pixmap_Ref0::remove_ref (
	::dpe2::Pixmap_Ref1 * ref_n )
{
	_refs.removeOne ( ref_n );
}


/// @brief Pixmap_Ref1
///
class Pixmap_Ref1
{
	// Public methods
	public:

	Pixmap_Ref1 (
		::dpe2::Pixmap_Ref0 * ref0_n );

	const ::dpe2::Key_Values &
	key_values ( ) const;

	void
	set_key_values (
		const ::dpe2::Key_Values & vset_n );


	::dpe2::Pixmap_Ref0 *
	ref0 ( ) const;

	::dpe2::Pixmap *
	pixmap ( ) const;


	QAtomicInt &
	num_ref2 ( );

	const QAtomicInt &
	num_ref2 ( ) const;


	// Private attributes
	private:

	::dpe2::Key_Values _key_values;
	::dpe2::Pixmap_Ref0 * _ref0;
	QAtomicInt _num_ref2;
};

inline
Pixmap_Ref1::Pixmap_Ref1 (
	::dpe2::Pixmap_Ref0 * ref0_n ) :
_ref0 ( ref0_n ),
_num_ref2 ( 0 )
{
}

inline
const ::dpe2::Key_Values &
Pixmap_Ref1::key_values ( ) const
{
	return _key_values;
}

inline
void
Pixmap_Ref1::set_key_values (
	const ::dpe2::Key_Values & vset_n )
{
	_key_values = vset_n;
}

inline
::dpe2::Pixmap_Ref0 *
Pixmap_Ref1::ref0 ( ) const
{
	return _ref0;
}

inline
::dpe2::Pixmap *
Pixmap_Ref1::pixmap ( ) const
{
	return ref0()->pixmap();
}

inline
QAtomicInt &
Pixmap_Ref1::num_ref2 ( )
{
	return _num_ref2;
}

inline
const QAtomicInt &
Pixmap_Ref1::num_ref2 ( ) const
{
	return _num_ref2;
}


/// @brief Pixmap_Ref2
///
class Pixmap_Ref2
{
	// Public methods
	public:

	Pixmap_Ref2 ( );

	Pixmap_Ref2 (
		::dpe2::Pixmap_Ref1 * ref1_n );

	void
	clear ( );

	void
	set_ref1 (
		::dpe2::Pixmap_Ref1 * ref1_n );

	::dpe2::Pixmap *
	pixmap ( ) const;

	::dpe2::Pixmap_Ref1 *
	ref1 ( ) const;


	// Private attributes
	private:

	::dpe2::Pixmap * _pixmap;
	::dpe2::Pixmap_Ref1 * _ref1;
};

inline
Pixmap_Ref2::Pixmap_Ref2 ( ) :
_pixmap ( 0 ),
_ref1 ( 0 )
{
}

inline
::dpe2::Pixmap *
Pixmap_Ref2::pixmap ( ) const
{
	return _pixmap;
}

inline
::dpe2::Pixmap_Ref1 *
Pixmap_Ref2::ref1 ( ) const
{
	return _ref1;
}


} // End of namespace

#endif
