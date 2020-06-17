/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_views_message_widget_hpp__
#define __INC_views_message_widget_hpp__

#include <QLabel>
#include <QWidget>

namespace Views
{

/// @brief (Error) message display widget
///
class Message_Widget : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  Message_Widget ( QWidget * parent_n = 0 );

  void
  set_title ( const QString & txt_n );

  void
  set_text ( const QString & txt_n );

  void
  set_mixer_open_fail ( const QString & device_n,
                        const QString & error_n,
                        const QString & func_n );

  void
  set_no_device ();

  // Private attributes
  private:
  QLabel _title;
  QLabel _text;

  // Masks
  QString _title_mask;
  QString _table_row_mask;

  // Default titles
  QString _title_open_mixer_fail;
  QString _title_no_device;
};

} // namespace Views

#endif
