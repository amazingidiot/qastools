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


	void
	paint_handle (
		::dpe2::Pixmap_Handle * handle_n );

	virtual
	void
	paint (
		QPainter & painter_n,
		const ::dpe2::Values_Set & vals_n ) = 0;


	// Protected methods
	protected:

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
		const ::dpe2::Values_Set & vals_n );

	QColor
	random_color ( ) const;
};


} // End of namespace

#endif
