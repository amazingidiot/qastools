/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_alsa_config_watcher_hpp__
#define __INC_qsnd_alsa_config_watcher_hpp__

#include <QFileSystemWatcher>
#include <QObject>
#include <QString>

namespace QSnd
{

/// @brief Main_Window
///
class ALSA_Config_Watcher : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  ALSA_Config_Watcher ( ::QObject * parent_n = 0 );

  // Signals
  signals:

  void
  sig_change ();

  // Private slots
  private slots:

  void
  change_in_file ( const QString & fl_n );

  void
  change_in_dir ( const QString & fl_n );

  // Private attributes
  private:
  QFileSystemWatcher _fwatch;
  QString _cfg_system_dir;
  QString _cfg_system_file;
  QString _cfg_home_dir;
  QString _cfg_home_file;
};

} // namespace QSnd

#endif
