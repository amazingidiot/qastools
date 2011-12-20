//
// C++ Interface:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INC_dpe_image_hpp__
#define __INC_dpe_image_hpp__

#include <QPixmap>
#include <QScopedPointer>


namespace dpe
{


/// @brief Image
///
/// An image set that can be shared by multiple users.
/// Right after creation the user count is always 0.
///
class Image
{
	// Public methods
	public:

	Image ( );

	~Image ( );

	void
	clear ( );

	void
	clear_data ( );

	unsigned int
	byte_count ( ) const;

	QPixmap *
	convert_to_pixmap ( );

	QPixmap *
	pixmap ( ) const;


	// Public attributes
	public:

	QScopedPointer < QPixmap > pixmap_sptr;

	unsigned int width;
	unsigned int height;
	unsigned int stride;
	unsigned char * data;
};


inline
QPixmap *
Image::pixmap ( ) const
{
	return pixmap_sptr.data();
}


} // End of namespace


#endif
