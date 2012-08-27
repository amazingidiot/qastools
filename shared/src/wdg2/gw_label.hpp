//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_label_hpp__
#define __INC_wdg2_gw_label_hpp__

#include "gw_pixmaps.hpp"
#include <QString>
#include <QFont>

namespace Wdg2
{


/// @brief GW_Label_Ground
///
class GW_Label_Ground :
	public ::Wdg2::GW_Pixmaps
{
	// Public methods
	public:

	GW_Label_Ground (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );


	const QString &
	text ( ) const;

	void
	set_text (
		const QString & text_n );


	const QFont &
	font ( ) const;

	void
	set_font (
		const QFont & font_n );


	const QRect &
	text_brect ( ) const;

	int
	font_height ( ) const;


	QRect
	calc_brect (
		const QFontMetrics & fmet_n,
		const QString & text_n );


	// Protected methods
	protected:

	void
	update_text_brect ( );

	bool
	setup_pxm_request (
		unsigned int idx_n,
		::dpe2::Key_Values & kvals_n );


	// Private attributes
	private:

	QString _text;
	QFont _font;
	QRect _text_brect;
	int _font_height;
};

inline
const QString &
GW_Label_Ground::text ( ) const
{
	return _text;
}

inline
const QFont &
GW_Label_Ground::font ( ) const
{
	return _font;
}

inline
const QRect &
GW_Label_Ground::text_brect ( ) const
{
	return _text_brect;
}

inline
int
GW_Label_Ground::font_height ( ) const
{
	return _font_height;
}


/// @brief GW_Label
///
class GW_Label :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Label (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	const QSize &
	size ( ) const;

	void
	set_size (
		const QSize & size_n );

	QSize
	probe_size (
		const QString & text_n );


	const QString &
	text ( ) const;

	void
	set_text (
		const QString & text_n );


	const QFont &
	font ( ) const;

	void
	set_font (
		const QFont & font_n );


	Qt::Alignment
	alignment ( ) const;

	void
	set_alignment (
		Qt::Alignment align_n );



	// Protected methods
	protected:

	void
	auto_set_size ( );

	void
	update_pixmap ( );


	// Private attributes
	private:

	GW_Label_Ground _ground;
	QSize _size;
	Qt::Alignment _alignment;
};

inline
const QSize &
GW_Label::size ( ) const
{
	return _size;
}

inline
const QString &
GW_Label::text ( ) const
{
	return _ground.text();
}

inline
const QFont &
GW_Label::font ( ) const
{
	return _ground.font();
}

inline
Qt::Alignment
GW_Label::alignment ( ) const
{
	return _alignment;
}


} // End of namespace

#endif
