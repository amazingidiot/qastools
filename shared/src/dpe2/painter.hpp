//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_painter_hpp__
#define __INC_dpe2_painter_hpp__

#include "pixmap_ref.hpp"
#include "pixmap_request.hpp"
#include <QPainter>
#include <QMutex>

namespace dpe2
{


/// @brief Painter
///
class Painter
{
	// Public methods
	public:

	Painter ( );

	virtual
	~Painter ( );

	QMutex &
	mutex ( );

	::dpe2::Pixmap_IRef0 *
	iref0_create ( );

	void
	iref0_destroy (
		::dpe2::Pixmap_IRef0 * iref0_n );

	void
	iref1_deref (
		::dpe2::Pixmap_IRef1 * iref1_n );

	::dpe2::Pixmap_IRef1 *
	find_match (
		const ::dpe2::Key_Values & vset1_n );

	/// @brief Signalizes this painter feels responsible
	///
	/// Must be implemented in a thread safe fashion
	virtual
	bool
	is_responsible (
		const ::dpe2::Key_Values & vset_n ) = 0;

	/// @brief Used in find_match. Can be reimplemented.
	///
	virtual
	bool
	key_values_match (
		const ::dpe2::Key_Values & vset1_n,
		const ::dpe2::Key_Values & vset2_n ) const;

	/// @brief Must be implemented in a thread safe fashion
	///
	virtual
	void
	paint (
		QPainter & painter_n,
		const ::dpe2::Key_Values & vals_n ) = 0;

	void
	paint_pixmap (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n );


	// Private attributes
	private:

	typedef ::std::vector < ::dpe2::Pixmap_IRef0 * > IRef0_List;

	IRef0_List _iref0s;
	QMutex _mutex;
};


inline
QMutex &
Painter::mutex ( )
{
	return _mutex;
}


} // End of namespace

#endif
