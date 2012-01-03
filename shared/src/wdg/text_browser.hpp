//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INC_text_browser_hpp__
#define __INC_text_browser_hpp__

#include <QTextBrowser>
#include <QTextFrame>
#include <QString>
#include <QScrollBar>


namespace Wdg
{


class Text_Browser :
	public QTextBrowser
{
	// Public methods
	public:

	Text_Browser (
		QWidget * parent = 0 );


	void
	setPlainText (
		const QString & txt_n );

	void
	setHtml (
		const QString & txt_n );

	QSize
	sizeHint ( ) const;


	// Protected methods
	protected:

	void
	update_size_hint ( );


	// Private attributes
	private:

	int _hint_width;
	int _hint_height;
};


} // Enf of namespace

#endif
