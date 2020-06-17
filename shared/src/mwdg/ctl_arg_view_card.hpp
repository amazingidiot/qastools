/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mwdg_ctl_arg_view_card_hpp__
#define __INC_mwdg_ctl_arg_view_card_hpp__

#include "mwdg/ctl_arg_view.hpp"
#include <QModelIndex>

// Forward declaration
namespace MWdg
{
class Controls_View;
}

namespace MWdg
{

/// @brief CTL_Arg_View_Card
///
class CTL_Arg_View_Card : public ::MWdg::CTL_Arg_View
{
  Q_OBJECT

  // Public methods
  public:
  CTL_Arg_View_Card ( QWidget * parent_n = 0 );

  void
  set_arg_string ( const QString & str_n );

  // Protected methods
  protected:
  void
  cards_model_changed ();

  // Protected methods
  protected slots:

  void
  read_view_selection ();

  // Private attributes
  private:
  ::MWdg::Controls_View * _ctl_view;
};

} // namespace MWdg

#endif
