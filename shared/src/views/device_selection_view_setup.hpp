/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_device_selection_view_setup_hpp__
#define __INC_device_selection_view_setup_hpp__

#include <QKeySequence>
#include <QList>
#include <QStringList>

// Forward declaration
namespace QSnd
{
class CTL_Address;
}

namespace Views
{

/// @brief Device_Selection_View_Setup
///
class Device_Selection_View_Setup
{
  // Public types
  public:
  typedef QList<::QSnd::CTL_Address > Selection_DB;

  // Public methods
  public:
  Device_Selection_View_Setup ();

  ~Device_Selection_View_Setup ();

  void
  selection_db_clear ();

  QStringList
  selection_db_get () const;

  void
  selection_db_set ( const QStringList & sel_db_n );

  // Public attributes
  public:
  QKeySequence kseq_toggle_vis;
  Selection_DB selection_db;
};

} // namespace Views

#endif
