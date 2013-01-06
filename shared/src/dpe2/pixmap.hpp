//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_dpe2_pixmap_hpp__
#define __INC_dpe2_pixmap_hpp__

#include <QPixmap>
#include <QScopedPointer>

namespace dpe2
{


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
	reset_qpixmap (
		QPixmap * pixmap_n = 0 );

	void
	set_size (
		unsigned int width_n,
		unsigned int height_n );

	unsigned int
	width ( ) const;

	unsigned int
	height ( ) const;


	void
	convert_to_qpixmap ( );

	QPixmap *
	qpixmap ( ) const;

	QImage &
	qimage ( );


	// Private attributes
	private:

	QPixmap * _qpixmap;
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
Pixmap::qpixmap ( ) const
{
	return _qpixmap;
}

inline
QImage &
Pixmap::qimage ( )
{
	return _qimage;
}


} // End of namespace

#endif
