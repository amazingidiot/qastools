/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_sliders_pad_header_data_hpp__
#define __INC_sliders_pad_header_data_hpp__

#include <QPointer>
#include <QWidget>
#include <vector>

namespace Wdg
{

class Sliders_Pad_Header_Label
{
  // Public methods
  public:
  Sliders_Pad_Header_Label ();

  // Public attributes
  public:
  unsigned int label_length_max;
  QTransform label_trans;
  QTransform label_trans_inv;
  QRectF label_rect;
  QRectF text_area;
  QRectF text_rect;
  QRect label_txt_bbox;

  unsigned int group_idx;
  unsigned int column_idx;
  unsigned int column_total_idx;

  QString text;
  QString text_elided;
  QString tool_tip;

  QColor col_fg;
};

///
/// @brief Sliders_Pad_Header_Data
///
class Sliders_Pad_Header_Data
{
  // Public methods
  public:
  Sliders_Pad_Header_Data ();

  bool update_elided_texts;
  bool update_decoration;
  bool upside_down;
  bool column_labels;
  bool label_sliding;

  std::vector< Sliders_Pad_Header_Label > labels;

  double angle;
  double angle_sin;
  double angle_cos;

  double center_x;
  double center_y;

  unsigned int max_str_length_px;

  unsigned int pad_left;
  unsigned int pad_right;
  unsigned int spacing_inter;
  unsigned int spacing_vertical;

  QPointer< QWidget > widget;
};

} // namespace Wdg

#endif
