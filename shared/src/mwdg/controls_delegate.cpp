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

#include "controls_delegate.hpp"

#include "qsnd/model_keys.hpp"
#include <QApplication>
#include <QPainter>
#include <iostream>


namespace MWdg
{


Controls_Delegate::Controls_Delegate (
	QObject * parent_n ) :
QStyledItemDelegate ( parent_n )
{
	_hmargin = 4;
	_vmargin = 3;
	_vspace = 1;
}


QSize
Controls_Delegate::sizeHint (
	const QStyleOptionViewItem & option_n,
	const QModelIndex & index_n ) const
{
	QSize res ( 0, 0 );

	QStyleOptionViewItemV4 opt ( option_n );
	initStyleOption ( &opt, index_n );

    const QFontMetrics & fmet ( opt.fontMetrics );
	res.setWidth ( fmet.averageCharWidth()*12 );
	res.rheight() += 2*_vmargin;
	if ( index_n.data ( ::QSnd::MKEY_CARD_INDEX ).isValid() ) {
		// Is Card
		res.rheight() += fmet.height()*2;
		res.rheight() += _vspace;
	} else {
		// Is Control
		res.rheight() += fmet.height();
	}

	return res;
}


void
Controls_Delegate::paint (
	QPainter * painter_n,
	const QStyleOptionViewItem & option_n,
	const QModelIndex & index_n ) const
{
	Q_ASSERT ( index_n.isValid() );

	QStyleOptionViewItemV4 opt ( option_n );
	initStyleOption ( &opt, index_n );
    const QFontMetrics & fmet ( opt.fontMetrics );
	opt.text = QString();

	QStyle * style ( 0 );
	if ( opt.widget != 0 ) {
		style = opt.widget->style();
	}
	if ( style == 0 ) {
		style = QApplication::style();
	}

	QFont fnt ( opt.font );
	QColor col_fg;

	{
		// Color group from state
		QPalette::ColorGroup col_grp ( QPalette::Active );
		if ( opt.state & QStyle::State_Enabled ) {
			if ( !( opt.state & QStyle::State_Active ) ) {
				col_grp = QPalette::Inactive;
			}
		} else {
			col_grp = QPalette::Disabled;
		}

		// Adjust background brush and foreground color from state
		QPalette::ColorRole col_role ( QPalette::Text );
		if ( opt.state & QStyle::State_Selected ) {
			col_role = QPalette::HighlightedText;

			QBrush & br ( opt.backgroundBrush );
			br.setStyle ( Qt::SolidPattern );
			br.setColor ( opt.palette.color ( col_grp, QPalette::Highlight ) );
		}
		col_fg = opt.palette.color ( col_grp, col_role );
	}

	// Save and setup painter
	painter_n->save();
	painter_n->setRenderHint ( QPainter::TextAntialiasing );

	// Paint background
	{
		// Paint solid area
		painter_n->setBrush ( opt.backgroundBrush );
		painter_n->setPen ( Qt::NoPen );
		painter_n->drawRect ( opt.rect );

		// Paint frame
		if ( ( opt.state & QStyle::State_Selected ) ) {
			painter_n->setBrush ( Qt::NoBrush );
			{
				QPen pen;
				{
					QColor pcol ( col_fg );
					if ( ( opt.state & QStyle::State_Active ) ) {
						pcol.setAlpha ( 110 );
					} else {
						pcol.setAlpha ( 0 );
					}
					pen.setColor ( pcol );
				}
				pen.setWidth ( 1 );
				pen.setStyle ( Qt::DotLine );
				painter_n->setPen ( pen );
			}
			QRect re_frame ( opt.rect );
			re_frame.adjust ( 0, 0, -1, -1 );
			painter_n->drawRect ( re_frame );
		}
	}

	if ( index_n.data ( ::QSnd::MKEY_CARD_INDEX ).isValid() ) {
		// Is card

		QRect re_t0;
		QRect re_t1;
		QRect re_t2;
		{
			int hspace ( fmet.averageCharWidth() * 3 / 4 );
			re_t0.setWidth ( fmet.width ( "9:" ) );
			re_t0.setHeight ( fmet.height() );
			re_t0.moveLeft ( opt.rect.left() + _hmargin );
			re_t0.moveTop ( opt.rect.top() + _vmargin );

			re_t1.setWidth ( opt.rect.width() - re_t0.width() - hspace - 2*_hmargin );
			re_t1.setHeight ( fmet.height() );
			re_t1.moveLeft ( re_t0.left() + re_t0.width() + hspace );
			re_t1.moveTop ( re_t0.top() );

			re_t2.setWidth ( re_t1.width() );
			re_t2.setHeight ( re_t1.height() );
			re_t2.moveLeft ( re_t1.left() );
			re_t2.moveTop ( re_t1.top() + re_t1.height() + _vspace );
		}

		// Paint foreground
		painter_n->setPen ( QPen ( col_fg ) );
		painter_n->setBrush ( Qt::NoBrush );
		painter_n->setFont ( fnt );

		// Index string
		QString txt;
		txt.setNum ( index_n.data ( ::QSnd::MKEY_CARD_INDEX ).toInt() );
		txt.append ( ":" );
		painter_n->drawText ( re_t0, Qt::AlignRight | Qt::AlignVCenter, txt );

		// Name string
		txt = index_n.data ( ::QSnd::MKEY_CARD_NAME ).toString();
		txt = fmet.elidedText ( txt, opt.textElideMode, re_t1.width() );
		painter_n->drawText ( re_t1, Qt::AlignLeft | Qt::AlignVCenter, txt );

		// Bottom string
		fnt.setItalic ( true );
		painter_n->setFont ( fnt );

		col_fg.setAlpha ( 160 );
		painter_n->setPen ( QPen ( col_fg ) );

		txt = index_n.data ( ::QSnd::MKEY_CARD_MIXER_NAME ).toString();
		txt = fmet.elidedText ( txt, opt.textElideMode, re_t2.width() );
		painter_n->drawText ( re_t2, Qt::AlignLeft | Qt::AlignVCenter, txt );

	} else {
		// Is control

		QRect re_t0;
		{
			re_t0.setWidth ( opt.rect.width() - 2*_hmargin );
			re_t0.setHeight ( fmet.height() );
			re_t0.moveLeft ( opt.rect.left() + _hmargin );
			re_t0.moveTop ( opt.rect.top() + _vmargin );
		}

		// Paint text
		painter_n->setPen ( QPen ( col_fg ) );
		painter_n->setBrush ( Qt::NoBrush );
		painter_n->setFont ( fnt );

		// Name string
		QRect brect;
		{
			QString txt ( index_n.data ( Qt::DisplayRole ).toString() );
			txt = fmet.elidedText ( txt, opt.textElideMode, re_t0.width() );
			painter_n->drawText ( re_t0, Qt::AlignLeft | Qt::AlignVCenter, txt, &brect );
		}

		{
			const QStringList & arg_lst (
				index_n.data ( ::QSnd::MKEY_L10N_ARGS ).toStringList() );
			if ( arg_lst.size() > 0 ) {
				col_fg.setAlpha ( 160 );
				painter_n->setPen ( QPen ( col_fg ) );
				fnt.setItalic ( true );
				painter_n->setFont ( fnt );

				QString txt ( " : " );
				txt.append ( arg_lst.join ( "," ) );
				re_t0.setLeft ( brect.left() + brect.width() );
				txt = fmet.elidedText ( txt, opt.textElideMode, re_t0.width() );
				painter_n->drawText ( re_t0, Qt::AlignLeft | Qt::AlignVCenter, txt );
			}
		}

	}

	// Restore painter
	painter_n->restore();
}


} // End of namespace
