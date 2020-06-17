/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_view_view_base_hpp__
#define __INC_view_view_base_hpp__

#include <QStackedLayout>
#include <QWidget>

// Forward declaration
namespace MWdg
{
class Mixer_Device_Setup;
class Inputs_Setup;
} // namespace MWdg
namespace Views
{
class Message_Widget;
class View_Base_Setup;
} // namespace Views

namespace Views
{

class View_Base : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  View_Base ( QWidget * parent_n = 0 );

  virtual ~View_Base ();

  /// The stacked layout holds the actual view widget and a
  /// (most times) hidden message widget that can be used for
  /// displaying error or info texts.
  QStackedLayout *
  lay_stack () const;

  /// @brief The message widget is mostly used for error messages
  ///
  Message_Widget *
  message_wdg ();

  // Mixer device setup

  /// @brief Mixer device setup
  ///
  /// @return The setup
  const ::MWdg::Mixer_Device_Setup *
  mdev_setup ();

  virtual void
  set_mdev_setup ( const ::MWdg::Mixer_Device_Setup * setup_n );

  // Input setup (keyboard/mouse)

  /// @brief Inputs (keyboard/mouse) setup
  ///
  /// @return The setup
  const ::MWdg::Inputs_Setup *
  inputs_setup ();

  virtual void
  set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n );

  // View setup (colors, element selection, etc.)

  virtual void
  set_view_setup ( ::Views::View_Base_Setup * setup_n ) = 0;

  // Signals
  signals:

  void
  sig_mdev_reload_request ();

  // Private attributes
  private:
  const ::MWdg::Mixer_Device_Setup * _mdev_setup;
  const ::MWdg::Inputs_Setup * _inputs_setup;
  ::Views::Message_Widget * _message_wdg;
  QStackedLayout * _lay_stack;
};

inline const ::MWdg::Mixer_Device_Setup *
View_Base::mdev_setup ()
{
  return _mdev_setup;
}

inline const ::MWdg::Inputs_Setup *
View_Base::inputs_setup ()
{
  return _inputs_setup;
}

inline QStackedLayout *
View_Base::lay_stack () const
{
  return _lay_stack;
}

inline Message_Widget *
View_Base::message_wdg ()
{
  return _message_wdg;
}

} // namespace Views

#endif
