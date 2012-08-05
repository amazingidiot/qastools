//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_painter_simple_hpp__
#define __INC_dpe2_painter_simple_hpp__

#include "painter.hpp"

namespace dpe2
{


/// @brief Painter_Simple
///
class Painter_Simple :
	public ::dpe2::Painter
{
	// Protected methods
	protected:

	bool
	is_responsible (
		const ::dpe2::Key_Values & vset_n );

	void
	paint (
		QPainter & painter_n,
		const ::dpe2::Key_Values & vals_n );

	QColor
	random_color ( ) const;
};


} // End of namespace

#endif
