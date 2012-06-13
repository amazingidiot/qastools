//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_painter_hpp__
#define __INC_dpe2_painter_hpp__

#include "pixmap_handle.hpp"
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

	bool
	process_request (
		::dpe2::Pixmap_Request & request_n );

	bool
	return_request (
		::dpe2::Pixmap_Request & request_n );

	/// @brief Must be implemented in a thread safe fashion
	///
	virtual
	void
	paint (
		QPainter & painter_n,
		const ::dpe2::Key_Values & vals_n ) = 0;


	// Protected methods
	protected:

	/// @brief Must be implemented in a thread safe fashion
	///
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

	::dpe2::Pixmap_Ref1 *
	find_match (
		const ::dpe2::Key_Values & vset1_n ) const;


	// Private attributes
	private:

	QList < ::dpe2::Pixmap_Ref1 * > _refs1;
	QMutex _mutex;
};


/// @brief Painter_Simple
///
class Painter_Simple :
	public ::dpe2::Painter
{
	// Public methods
	public:

	void
	paint (
		QPainter & painter_n,
		const ::dpe2::Key_Values & vals_n );


	// Protected methods
	protected:

	bool
	is_responsible (
		const ::dpe2::Key_Values & vset_n );

	QColor
	random_color ( ) const;
};


} // End of namespace

#endif
