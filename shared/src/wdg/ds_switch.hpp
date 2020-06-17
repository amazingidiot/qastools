/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_ds_switch_hpp__
#define __INC_wdg_ds_switch_hpp__

#include "dpe/image_set_meta.hpp"
#include "wdg/ds_imaging.hpp"
#include <QAbstractButton>

namespace Wdg
{

/// @brief Dynamic sized switch
///
/// Background images:
/// 0 - idle
/// 1 - focus
/// 2 - hover idle
/// 3 - hover focus
///
/// Handle images:
/// 0 - unchecked idle
/// 1 - unchecked focus
/// 2 - unchecked hover
/// 3 - unchecked hover & focus
///
/// 4 - checked idle
/// 5 - checked focus
/// 6 - checked hover
/// 7 - checked hover & focus
///
/// 8 - half checked focus
/// 9 - half checked hover & focus
///
/// It is possible to use this switch widget as a push button
/// by setting setCheckable ( false ).
/// Push buttons don't require the images 4-7.
///
class DS_Switch : public QAbstractButton
{
  Q_OBJECT

  // Public methods
  public:
  DS_Switch ( QWidget * parent_n = 0, ::dpe::Image_Allocator * alloc_n = 0 );

  ~DS_Switch ();

  QSize
  sizeHint () const;

  QSize
  minimumSizeHint () const;

  // Image allocator and style id

  void
  set_image_alloc ( ::dpe::Image_Allocator * alloc_n );

  void
  set_variant_id ( unsigned int id_n );

  void
  set_style_id ( unsigned int id_n );

  // Protected methods
  protected:
  void
  changeEvent ( QEvent * event_n );

  void
  enterEvent ( QEvent * );

  void
  leaveEvent ( QEvent * );

  void
  resizeEvent ( QResizeEvent * event );

  void
  paintEvent ( QPaintEvent * event );

  void
  update_pixmaps ();

  // Private attributes
  private:
  static const unsigned int num_images_bg = 4;
  static const unsigned int num_images_handle = 10;

  ::Wdg::DS_Imaging _dsi;

  QRect _pxmap_rect;
  bool _update_pixmaps_pending;

  ::dpe::Image_Set_Meta _meta_bg;
  ::dpe::Image_Set_Meta _meta_handle;
};

} // namespace Wdg

#endif
