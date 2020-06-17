/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_views_info_dialog_hpp__
#define __INC_views_info_dialog_hpp__

#include "views/multi_page_dialog.hpp"

namespace Views
{

/// @brief Info_Dialog
///
class Info_Dialog : public ::Views::Multi_Page_Dialog
{
  Q_OBJECT

  // Public methods
  public:
  Info_Dialog ( QWidget * parent_n = 0, Qt::WindowFlags flags_n = 0 );

  // Protected methods
  protected:
  bool
  read_utf8_file ( const QString & filename_n, QString & txt_n ) const;

  // Private attributes
  private:
};

} // namespace Views

#endif
