/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_switches_pad_widgets_hpp__
#define __INC_switches_pad_widgets_hpp__

#include <QLabel>
#include <QObject>
#include <QScopedPointer>

namespace Wdg
{

/// @brief Switches_Pad_Widgets
///
class Switches_Pad_Widgets
{

  // Public methods
  public:
  Switches_Pad_Widgets ();

  ~Switches_Pad_Widgets ();

  void
  clear_widgets ();

  // Input widget

  void
  set_input_wdg ( QWidget * wdg_n );

  QWidget *
  input_wdg ();

  // Label widget

  QLabel *
  label_wdg ();

  void
  set_label_wdg ( QLabel * wdg_n );

  // Private attributes
  private:
  QScopedPointer< QLabel > _label;
  QScopedPointer< QWidget > _input;
};

inline QWidget *
Switches_Pad_Widgets::input_wdg ()
{
  return _input.data ();
}

inline QLabel *
Switches_Pad_Widgets::label_wdg ()
{
  return _label.data ();
}

} // namespace Wdg

#endif
