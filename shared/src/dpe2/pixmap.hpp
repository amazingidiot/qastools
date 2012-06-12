//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_pixmap_hpp__
#define __INC_dpe2_pixmap_hpp__

#include <QPixmap>
#include <QScopedPointer>


namespace dpe2
{


enum Pixmap_Keys {
	PMK_WIDTH,
	PMK_HEIGHT
};


/// @brief A pixmap
///
class Pixmap
{
	// Public methods
	public:

	Pixmap ( );

	~Pixmap ( );

	void
	clear ( );

	void
	set_size (
		unsigned int width_n,
		unsigned int height_n );

	unsigned int
	width ( ) const;

	unsigned int
	height ( ) const;


	QPixmap *
	convert_to_pixmap ( );

	QPixmap *
	pixmap ( ) const;

	QImage &
	qimage ( );


	// Private attributes
	private:

	QScopedPointer < QPixmap > _pixmap;
	QImage _qimage;

	unsigned int _width;
	unsigned int _height;
};


inline
unsigned int
Pixmap::width ( ) const
{
	return _width;
}

inline
unsigned int
Pixmap::height ( ) const
{
	return _height;
}

inline
QPixmap *
Pixmap::pixmap ( ) const
{
	return _pixmap.data();
}

inline
QImage &
Pixmap::qimage ( )
{
	return _qimage;
}


} // End of namespace

#endif
