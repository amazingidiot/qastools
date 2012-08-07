//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_theme_hpp__
#define __INC_wdg2_theme_hpp__

#include <QList>
#include <QPalette>

// Forward declaration
namespace Wdg2
{
	class Theme_Painter;
}

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

	::Wdg2::Theme_Painter *
	painter (
		unsigned int idx_n );

	/// @brief Adds a painter including ownership to the Theme
	///
	void
	add_painter (
		::Wdg2::Theme_Painter * painter_n );

	void
	take_painter (
		::Wdg2::Theme_Painter * painter_n );


	const QPalette &
	qpalette ( ) const;

	void
	set_qpalette (
		const QPalette & palette_n );


	// Private attributes
	private:

	typedef QList < ::Wdg2::Theme_Painter * > Painter_List;

	Painter_List _painters;
	QPalette _qpalette;
};


inline
unsigned int
Theme::num_painters ( ) const
{
	return _painters.size();
}

inline
::Wdg2::Theme_Painter *
Theme::painter (
	unsigned int idx_n )
{
	return _painters[idx_n];
}

inline
const QPalette &
Theme::qpalette ( ) const
{
	return _qpalette;
}


} // End of namespace

#endif
