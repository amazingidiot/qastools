/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_sliders_pad_header_hpp__
#define __INC_sliders_pad_header_hpp__

#include "sliders_pad_header_data.hpp"
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

namespace Wdg
{

// Forward declaration
class Sliders_Pad_Data;
class Sliders_Pad_Data_Group;
class Sliders_Pad_Data_Column;
class Sliders_Pad_Style;

///
/// @brief Sliders_Pad_Header
///
class Sliders_Pad_Header : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  Sliders_Pad_Header ( Sliders_Pad_Data * sp_data_n,
                       Sliders_Pad_Style * sp_style_n,
                       QWidget * parent_n = 0 );

  ~Sliders_Pad_Header ();

  QSize
  minimumSizeHint () const;

  QSize
  sizeHint () const;

  // Header variables

  const Sliders_Pad_Header_Data &
  hd_data () const;

  Sliders_Pad_Header_Data &
  hd_data ();

  Sliders_Pad_Data *
  sp_data () const;

  Sliders_Pad_Style *
  sp_style () const;

  // Decoration graphics

  /// @brief Sets the index of the label/group with the focus
  void
  set_focus_idx ( unsigned int group_idx_n, unsigned int column_idx_n );

  unsigned int
  label_str_length_px_max ( const QString & str_n ) const;

  // Public slots
  public slots:

  void
  set_label_text ( unsigned int lbl_idx_n, const QString & txt_n );

  // Signals
  signals:

  void
  sig_label_selected ( unsigned int group_idx_n, unsigned int column_idx_n );

  // Protected methods
  protected:
  void
  enterEvent ( QEvent * event_n );

  void
  leaveEvent ( QEvent * event_n );

  void
  mousePressEvent ( QMouseEvent * event_n );

  void
  mouseMoveEvent ( QMouseEvent * event_n );

  void
  changeEvent ( QEvent * event_n );

  void
  paintEvent ( QPaintEvent * event_n );

  void
  paint_label_rects ( QPainter & pnt_n );

  void
  paint_label_decos ( QPainter & pnt_n );

  void
  paint_label_texts ( QPainter & pnt_n );

  void
  paint_label_text ( QPainter & pnt_n,
                     Qt::Alignment txt_align_n,
                     unsigned int lbl_idx_n,
                     unsigned int state_idx_n );

  const Sliders_Pad_Header_Label *
  find_label ( const QPoint & pos_n );

  void
  elided_label_text ( Sliders_Pad_Header_Label & lbl_n );

  void
  update_elided_texts ();

  void
  update_painter_states ();

  // Private attributes
  private:
  Sliders_Pad_Header_Data _data;
  Sliders_Pad_Data * _sp_data;
  Sliders_Pad_Style * _sp_style;

  unsigned int _focus_idx;
  unsigned int _weak_focus_idx;
  const unsigned int _invalid_idx;

  // 0 - default
  // 1 - focus only
  // 2 - weak focus only
  // 3 - focus and weak focus
  QBrush _lbl_rect_brush[ 3 ];
  QPen _lbl_txt_pen[ 4 ];
  QFont _lbl_txt_font[ 4 ];
};

inline const Sliders_Pad_Header_Data &
Sliders_Pad_Header::hd_data () const
{
  return _data;
}

inline Sliders_Pad_Header_Data &
Sliders_Pad_Header::hd_data ()
{
  return _data;
}

inline Sliders_Pad_Data *
Sliders_Pad_Header::sp_data () const
{
  return _sp_data;
}

inline Sliders_Pad_Style *
Sliders_Pad_Header::sp_style () const
{
  return _sp_style;
}

} // namespace Wdg

#endif
