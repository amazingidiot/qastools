/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "alsa_config_watcher.hpp"
#include <QDir>

namespace QSnd
{

ALSA_Config_Watcher::ALSA_Config_Watcher ( ::QObject * parent_n )
: QObject ( parent_n )
{

  _cfg_system_dir = "/etc";
  _cfg_system_file = _cfg_system_dir + "/asound.conf";
  _cfg_home_dir = QDir::homePath ();
  _cfg_home_file = _cfg_home_dir + "/.asoundrc";

  // File system watcher looks for changes in ALSA cfg files
  _fwatch.addPath ( _cfg_system_dir );
  if ( QFile::exists ( _cfg_system_file ) ) {
    _fwatch.addPath ( _cfg_system_file );
  }
  _fwatch.addPath ( _cfg_home_dir );
  if ( QFile::exists ( _cfg_home_file ) ) {
    _fwatch.addPath ( _cfg_home_file );
  }

  connect ( &_fwatch,
            SIGNAL ( fileChanged ( const QString & ) ),
            this,
            SLOT ( change_in_file ( const QString & ) ) );

  connect ( &_fwatch,
            SIGNAL ( directoryChanged ( const QString & ) ),
            this,
            SLOT ( change_in_dir ( const QString & ) ) );
}

void
ALSA_Config_Watcher::change_in_file ( const QString & fl_n )
{
  //::std::cout << "Change in file " << fl_n.toLocal8Bit().data() << "\n";
  if ( ( fl_n == _cfg_system_file ) || ( fl_n == _cfg_home_file ) ) {
    emit sig_change ();
  }
}

void
ALSA_Config_Watcher::change_in_dir ( const QString & fl_n )
{
  if ( fl_n == _cfg_system_dir ) {
    if ( !_fwatch.files ().contains ( _cfg_system_file ) ) {
      if ( QFile::exists ( _cfg_system_file ) ) {
        //::std::cout << "Change in dir " <<
        //_cfg_system_file.toLocal8Bit().data() << "\n";
        _fwatch.addPath ( _cfg_system_file );
        emit sig_change ();
      }
    }
  }
  if ( fl_n == _cfg_home_dir ) {
    if ( !_fwatch.files ().contains ( _cfg_home_file ) ) {
      if ( QFile::exists ( _cfg_home_file ) ) {
        //::std::cout << "Change in dir " << _cfg_home_file.toLocal8Bit().data()
        //<< "\n";
        _fwatch.addPath ( _cfg_home_file );
        emit sig_change ();
      }
    }
  }
}

} // namespace QSnd
