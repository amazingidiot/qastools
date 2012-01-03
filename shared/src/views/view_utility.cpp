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

#include "view_utility.hpp"

#include "qastools_config.hpp"
#include <QDesktopWidget>
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFileInfo>
#include <QLocale>
#include <QLabel>


namespace Views
{


void
load_translators (
	QApplication * app_n )
{
	// Application translators setup
	{
		QTranslator * trans_qt ( new QTranslator ( app_n ) );
		QTranslator * trans_default ( new QTranslator ( app_n ) );
		QTranslator * trans_local ( new QTranslator ( app_n ) );
		{
			QString l10n_db ( "qt_" );
			l10n_db.append ( QLocale::system().name() );
			trans_qt->load ( l10n_db,
				QLibraryInfo::location ( QLibraryInfo::TranslationsPath ) );
		}
		{
			QString l10n_db ( L10N_PREFIX );
			l10n_db.append ( "default" );
			trans_default->load ( l10n_db, INSTALL_DIR_L10N );
		}
		{
			QString l10n_db ( L10N_PREFIX );
			l10n_db.append ( QLocale::system().name() );
			trans_local->load ( l10n_db, INSTALL_DIR_L10N );
		}
		app_n->installTranslator ( trans_qt );
		app_n->installTranslator ( trans_default );
		app_n->installTranslator ( trans_local );
	}
}


void
load_application_icon (
	QApplication * app_n,
	const QString & fallback_n )
{
	QIcon icon;
	{
		// Try application icon
		QString icon_path ( INSTALL_DIR_ICONS_SVG );
		icon_path += "/";
		icon_path += PROGRAM_NAME;
		icon_path += ".svg";

		QFileInfo finfo ( icon_path );
		if ( finfo.exists() && finfo.isReadable() ) {
			icon = QIcon ( icon_path );
		}
	}
	if ( icon.isNull() ) {
		// Try system theme icon
		if ( QIcon::hasThemeIcon ( fallback_n  ) ) {
			icon = QIcon::fromTheme ( fallback_n  );
		}
	}
	if ( !icon.isNull() ) {
		app_n->setWindowIcon ( icon );
	}
}


void
resize_to_default (
	QMainWindow * mwin_n )
{

	// Adjust startup size
	{
		const QSize default_size ( 800, 450 );
		const QRect ravail ( QApplication::desktop()->availableGeometry() );

		if ( ravail.isValid() ) {

			const unsigned int aspect[2] = { 16, 9 };
			unsigned int rel_width[2];
			if ( ravail.width() > 1024 ) {
				// Larger screens
				rel_width[0] = 2;
				rel_width[1] = 3;
			} else {
				// Small screens - occupy more relative space
				rel_width[0] = 3;
				rel_width[1] = 4;
			}

			QRect wrect;
			wrect.setWidth ( ( ravail.width() * rel_width[0] ) / rel_width[1] );
			wrect.setHeight ( ( wrect.width() * aspect[1] ) / aspect[0] );
			if ( wrect.height() > ravail.height() ) {
				wrect.setHeight ( ravail.height() );
			}
			wrect.moveTop ( ( ravail.height() - wrect.height() ) / 2 );
			wrect.moveLeft ( ( ravail.width() - wrect.width() ) / 2 );
			mwin_n->resize ( wrect.size() );
			mwin_n->move ( wrect.topLeft() );
		} else {
			mwin_n->resize ( default_size );
		}
	}
}


} // End of namespace
