/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mwdg_controls_view_hpp__
#define __INC_mwdg_controls_view_hpp__

#include <QListView>

namespace MWdg
{

/// @brief Controls_View
///
class Controls_View : public QListView
{
  Q_OBJECT

  // Public methods
  public:
  Controls_View ( QWidget * parent_n = 0 );

  ~Controls_View ();

  QSize
  minimumSizeHint () const;

  QSize
  sizeHint () const;

  void
  setModel ( QAbstractItemModel * model_n );

  bool
  event ( QEvent * event_n );

  // Protected slots
  protected slots:

  void
  maximum_height_update_request ();

  // Protected methods
  protected:
  void
  changeEvent ( QEvent * event_n );

  void
  maximum_height_update ();

  // Private attributes
  private:
  int _show_rows_min;
  int _show_rows_avrg;
  int _min_chars_vis;

  bool _maximum_update_requested;
};

} // namespace MWdg

#endif
