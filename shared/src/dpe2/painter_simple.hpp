//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
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
	// Public methods
	public:

	bool
	is_responsible (
		const ::dpe2::Key_Values & vset_n );


	// Protected methods
	protected:

	void
	paint (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n );

	QColor
	random_color ( ) const;
};


} // End of namespace

#endif
