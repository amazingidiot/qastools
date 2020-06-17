/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_controls_database_hpp__
#define __INC_qsnd_controls_database_hpp__

#include "qsnd/ctl_format.hpp"
#include <QList>
#include <QObject>

namespace QSnd
{

/// @brief Controls_Database
///
class Controls_Database : public QObject
{
  Q_OBJECT;

  // Public methods
  public:
  Controls_Database ();

  ~Controls_Database ();

  // Control plugins

  unsigned int
  num_controls () const;

  const ::QSnd::CTL_Format &
  control_format ( unsigned int index_n ) const;

  const ::QSnd::CTL_Format *
  find_control_format ( const QString & ctl_name_n ) const;

  // Signals
  signals:

  void
  sig_change_coming ();

  void
  sig_change_done ();

  void
  sig_reload_required ();

  // Public slots
  public slots:

  void
  clear ();

  void
  clear_silent ();

  void
  reload ();

  void
  reload_silent ();

  // Protected methods
  protected:
  void
  load_plugins ();

  // Private attributes
  private:
  QList<::QSnd::CTL_Format > _ctl_formats;
};

inline unsigned int
Controls_Database::num_controls () const
{
  return _ctl_formats.size ();
}

inline const ::QSnd::CTL_Format &
Controls_Database::control_format ( unsigned int index_n ) const
{
  return _ctl_formats[ index_n ];
}

} // namespace QSnd

#endif
