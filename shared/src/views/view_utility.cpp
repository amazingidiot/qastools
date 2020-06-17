/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "view_utility.hpp"
#include "qastools_config.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QLabel>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

namespace Views
{

void
load_translators ( QApplication * app_n )
{
  // Application translators setup
  {
    QTranslator * trans_qt ( new QTranslator ( app_n ) );
    QTranslator * trans_default ( new QTranslator ( app_n ) );
    QTranslator * trans_local ( new QTranslator ( app_n ) );
    {
      QString l10n_db ( "qt_" );
      l10n_db.append ( QLocale::system ().name () );
      trans_qt->load (
          l10n_db, QLibraryInfo::location ( QLibraryInfo::TranslationsPath ) );
    }
    {
      QString l10n_db ( L10N_PREFIX );
      l10n_db.append ( "default" );
      trans_default->load ( l10n_db, INSTALL_DIR_L10N );
    }
    {
      QString l10n_db ( L10N_PREFIX );
      l10n_db.append ( QLocale::system ().name () );
      trans_local->load ( l10n_db, INSTALL_DIR_L10N );
    }
    app_n->installTranslator ( trans_qt );
    app_n->installTranslator ( trans_default );
    app_n->installTranslator ( trans_local );
  }
}

void
load_application_icon ( QApplication * app_n, const QString & fallback_n )
{
  QIcon icon;
  {
    // Try application icon
    QString icon_path ( INSTALL_DIR_ICONS_SVG );
    icon_path += "/";
    icon_path += PROGRAM_NAME;
    icon_path += ".svg";

    QFileInfo finfo ( icon_path );
    if ( finfo.exists () && finfo.isReadable () ) {
      icon = QIcon ( icon_path );
    }
  }
  if ( icon.isNull () ) {
    // Try system theme icon
    if ( QIcon::hasThemeIcon ( fallback_n ) ) {
      icon = QIcon::fromTheme ( fallback_n );
    }
  }
  if ( !icon.isNull () ) {
    app_n->setWindowIcon ( icon );
  }
}

bool
win_default_geometry ( QRect & rect_n )
{
  const unsigned int aspect[ 2 ] = {16, 9};
  const QRect ravail ( QApplication::desktop ()->availableGeometry () );
  const bool res ( ravail.isValid () );
  if ( res ) {
    QSize rsize;
    {
      // Calculate window size from available screen area
      unsigned int rel_width[ 2 ];
      if ( ravail.width () > 1024 ) {
        // Larger screens
        rel_width[ 0 ] = 2;
        rel_width[ 1 ] = 3;
      } else {
        // Small screens - occupy more relative space
        rel_width[ 0 ] = 3;
        rel_width[ 1 ] = 4;
      }

      rsize.setWidth ( ( ravail.width () * rel_width[ 0 ] ) / rel_width[ 1 ] );
      rsize.setHeight ( ( rsize.width () * aspect[ 1 ] ) / aspect[ 0 ] );
      if ( rsize.width () > ravail.width () ) {
        rsize.setWidth ( ravail.width () );
      }
      if ( rsize.height () > ravail.height () ) {
        rsize.setHeight ( ravail.height () );
      }
    }

    rect_n.setSize ( rsize );
    rect_n.moveTop ( ( ravail.height () - rect_n.height () ) / 2 );
    rect_n.moveLeft ( ( ravail.width () - rect_n.width () ) / 2 );
  } else {
    const QSize default_size ( 800, 450 );
    rect_n.setSize ( default_size );
  }
  return res;
}

bool
win_default_size ( QSize & size_n )
{
  QRect rhint;
  if ( ::Views::win_default_geometry ( rhint ) ) {
    if ( size_n.width () < rhint.width () ) {
      size_n.setWidth ( rhint.width () );
    }
    if ( size_n.height () < rhint.height () ) {
      size_n.setHeight ( rhint.height () );
    }
    return true;
  }
  return false;
}

void
resize_to_default ( QMainWindow * mwin_n )
{
  QRect wrect;
  if ( win_default_geometry ( wrect ) ) {
    mwin_n->resize ( wrect.size () );
    mwin_n->move ( wrect.topLeft () );
  } else {
    mwin_n->resize ( wrect.size () );
  }
}

} // namespace Views
