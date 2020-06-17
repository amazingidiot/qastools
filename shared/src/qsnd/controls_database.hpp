/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_controls_database_hpp__
#define __INC_qsnd_controls_database_hpp__

#include "qsnd/ctl_format.hpp"
#include <QObject>
#include <memory>
#include <vector>

namespace QSnd
{

/// @brief Controls_Database
///
class Controls_Database : public QObject
{
  Q_OBJECT;

  // -- Types
  public:
  using Const_Handle = std::shared_ptr< const ::QSnd::CTL_Format >;

  // -- Construction

  Controls_Database ();

  ~Controls_Database ();

  // -- Control plugins

  std::size_t
  num_controls () const
  {
    return _controls.size ();
  }

  const std::vector< Const_Handle > &
  controls () const
  {
    return _controls;
  }

  const Const_Handle &
  control_format ( std::size_t index_n ) const
  {
    return _controls[ index_n ];
  }

  Const_Handle
  find_control_format ( const QString & ctl_name_n ) const;

  // Signals
  signals:

  void
  sig_reload_begin ();

  void
  sig_reload_end ();

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

  protected:
  // -- Utility

  void
  load_plugins ();

  private:
  // -- Attributes
  std::vector< Const_Handle > _controls;
};

} // namespace QSnd

#endif
