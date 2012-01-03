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

#include "info_dialog.hpp"

#include "qastools_config.hpp"
#include "wdg/text_browser.hpp"

#include <QVBoxLayout>
#include <QLocale>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QPushButton>

namespace Views
{


Info_Dialog::Info_Dialog (
	QWidget * parent_n,
	Qt::WindowFlags flags_n ) :
::Views::Multi_Page_Dialog ( parent_n, flags_n )
{
	// Window title
	{
		QString txt ( "%1 - %2" );
		txt = txt.arg ( PROGRAM_TITLE );
		txt = txt.arg ( tr ( "Info" ) );
		setWindowTitle ( txt );
	}

	// Title label
	{
		QString txt ( "%1 - %2" );
		txt = txt.arg ( PACKAGE_TITLE );
		txt = txt.arg ( PACKAGE_VERSION );
		set_title_str ( txt );
	}

	::Wdg::Text_Browser * _txt_info ( 0 );
	::Wdg::Text_Browser * _txt_people ( 0 );
	::Wdg::Text_Browser * _txt_license ( 0 );

	const QString hmask ( "<h3>%1</h3>\n" );

	// Description text
	{
		const QString amask ( "<a href=\"%1\" title=\"%2\">%1</a>" );
		QString txt;
		txt += hmask.arg ( tr ( "About" ) );

		txt += "<p>";
		{
			QString val ( tr ( "%1 is a collection of desktop applications for the Linux sound system %2." ) );
			val = val.arg ( PACKAGE_TITLE );
			val = val.arg ( "<a href=\"http://www.alsa-project.org\" title=\"The ALSA project\">ALSA</a>" );
			txt += val;
		}
		txt += "</p>";

		// Internet
		txt += hmask.arg ( tr ( "Internet" ) );

		txt += "<p>";
		txt += amask.arg ( "http://xwmw.org/qastools", tr ( "Home page" ) );
		txt += "</p>";

		txt += "<p>";
		txt += amask.arg ( "http://sourceforge.net/projects/qastools/", tr ( "Project page" ) );
		txt += "</p>";


		_txt_info = new ::Wdg::Text_Browser;
		_txt_info->setAlignment ( Qt::AlignLeft | Qt::AlignTop );
		_txt_info->setWordWrapMode ( QTextOption::NoWrap );
		_txt_info->setOpenLinks ( true );
		_txt_info->setOpenExternalLinks ( true );
		_txt_info->setHtml ( txt );
	}


	// People text
	{
		const QString pemask ( "%1 &lt;<a href=\"mailto:%2\">%2</a>&gt;" );
		QString trmask;
		QString pdivmask;
		{
			const QString pdiv ( "<div style=\"margin-bottom: 4px;\">%1</div>" );
			trmask.append ( pemask );
			trmask.append ( " [%3]" );
			trmask = pdiv.arg ( trmask );
			pdivmask = pdiv.arg ( pemask );
		}


		QString txt;

		// Developers
		txt += hmask.arg ( tr ( "Developers" ) );
		txt += pdivmask.arg ( "Sebastian Holtermann", "sebholt@xwmw.org" );

		// Contributors
		txt += hmask.arg ( tr ( "Contributors" ) );
		txt += pdivmask.arg ( "Jose Lencioni", "elcorreodelcoco@gmail.com" );
		txt += pdivmask.arg ( "Ivan Sorokin", "vanyacpp@gmail.com" );

		// Translators
		txt += hmask.arg ( tr ( "Translators" ) );
		txt += trmask.arg ( "Sebastian Holtermann", "sebholt@xwmw.org", "de" );
		txt += trmask.arg ( "Jose Lencioni", "elcorreodelcoco@gmail.com", "es" );
		txt += trmask.arg ( "Ivan Sorokin", "vanyacpp@gmail.com", "ru" );
		txt += trmask.arg ( "Pavel Fric", "pavelfric@seznam.cz", "cs" );

		_txt_people = new ::Wdg::Text_Browser;
		_txt_people->setAlignment ( Qt::AlignLeft | Qt::AlignTop );
		_txt_people->setWordWrapMode ( QTextOption::NoWrap );
		_txt_people->setOpenLinks ( true );
		_txt_people->setOpenExternalLinks ( true );
		_txt_people->setHtml ( txt );
	}


	// License text
	{
		QString txt;
		{
			const unsigned int num_search ( 2 );
			QString fl_abs[num_search];
			fl_abs[0] = INSTALL_DIR_DATA;
			fl_abs[0] += "/COPYING";
			fl_abs[1] = "/usr/share/common-licenses/GPL-3";

			bool found ( false );
			for ( unsigned int ii=0; ii < num_search; ++ii ) {
				if ( read_utf8_file ( fl_abs[ii], txt ) ) {
					found = true;
					break;
				}
			}

			if ( found ) {
				_txt_license = new ::Wdg::Text_Browser;
			} else {
				txt = tr ( "The license file %1 is not available." );
				txt = txt.arg ( fl_abs[0] );
			}
		}

		if ( _txt_license != 0 ) {
			//_txt_license->setFrameStyle ( QFrame::NoFrame );
			_txt_license->setAlignment ( Qt::AlignLeft | Qt::AlignTop );
			_txt_license->setWordWrapMode ( QTextOption::NoWrap );
			_txt_license->setOpenLinks ( true );
			_txt_license->setOpenExternalLinks ( true );
			{
				QFont fnt ( _txt_license->document()->defaultFont() );
				fnt.setFamily ( "courier" );
				fnt.setFixedPitch ( true );
				_txt_license->document()->setDefaultFont ( fnt );
			}
			_txt_license->setPlainText ( txt );
		}
	}

	add_page ( tr ( "Information" ), _txt_info );
	add_page ( tr ( "People" ), _txt_people );
	add_page ( tr ( "License" ), _txt_license );

	set_current_page_idx ( 0 );
}


bool
Info_Dialog::read_utf8_file (
	const QString & filename_n,
	QString & txt_n ) const
{
	bool res ( false );

	QFile file ( filename_n );
	file.open ( QIODevice::ReadOnly );
	if ( file.isOpen() ) {
		QByteArray ba ( file.readAll() );
		txt_n = QString::fromUtf8 ( ba.data(), ba.size() );
		file.close();
		res = true;
	}

	return res;
}


} // End of namespace
