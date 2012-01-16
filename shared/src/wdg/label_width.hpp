//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//


#ifndef __INC_label_width_hpp__
#define __INC_label_width_hpp__

#include <QString>
#include <QLabel>


namespace Wdg
{


class Label_Width :
	public QLabel
{
	// Public methods
	public:

	Label_Width (
		QWidget * parent = 0 );


	const QString &
	min_text ( ) const;

	void
	set_min_text (
		const QString txt_n );


	QSize
	minimumSizeHint ( ) const;

	QSize
	sizeHint ( ) const;


	// Private attributes
	private:

	QString _min_text;
};


inline
const QString &
Label_Width::min_text ( ) const
{
	return _min_text;
}


} // End of namespace

#endif
