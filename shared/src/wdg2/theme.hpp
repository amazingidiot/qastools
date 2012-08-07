//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_theme_hpp__
#define __INC_wdg2_theme_hpp__

#include "dpe2/painter.hpp"
#include <QList>

namespace Wdg2
{


/// @brief Theme
///
class Theme
{
	// Public methods
	public:

	Theme ( );

	virtual
	~Theme ( );


	unsigned int
	num_painters ( ) const;

	::dpe2::Painter *
	painter (
		unsigned int idx_n );


	/// @brief Adds a painter including ownership to the Theme
	///
	void
	add_painter (
		::dpe2::Painter * painter_n );

	void
	take_painter (
		::dpe2::Painter * painter_n );


	// Private attributes
	private:

	typedef QList < ::dpe2::Painter * > Painter_List;

	Painter_List _painters;
};


inline
unsigned int
Theme::num_painters ( ) const
{
	return _painters.size();
}

inline
::dpe2::Painter *
Theme::painter (
	unsigned int idx_n )
{
	return _painters[idx_n];
}


} // End of namespace

#endif
