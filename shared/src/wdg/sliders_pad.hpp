/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_sliders_pad_hpp__
#define __INC_wdg_sliders_pad_hpp__

#include "wdg/pad_focus_info.hpp"
#include <QList>
#include <QWidget>

// Forward declaration
namespace dpe
{
class Image_Allocator;
}
namespace Wdg
{
class Sliders_Pad_Data;
class Sliders_Pad_Data_Group;
class Sliders_Pad_Data_Column;
class Sliders_Pad_Header_Data;
class Sliders_Pad_Header;
class Sliders_Pad_Footer;
class Sliders_Pad_Style;
class Pad_Proxies_Group;
class DS_Widget_Style_Db;
} // namespace Wdg

namespace Wdg
{

/// @brief Sliders_Pad
///
class Sliders_Pad : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  Sliders_Pad ( QWidget * parent_n = 0,
                ::dpe::Image_Allocator * isg_alloc_n = 0 );

  ~Sliders_Pad ();

  // Proxies groups list

  const QList< Pad_Proxies_Group * > &
  proxies_groups () const;

  void
  set_proxies_groups ( const QList< Pad_Proxies_Group * > & list_n );

  void
  clear_proxies_groups ();

  // Proxies group access

  unsigned int
  num_proxies_groups () const;

  Pad_Proxies_Group *
  proxies_group ( unsigned int idx_n );

  // Widget style database

  const ::Wdg::DS_Widget_Style_Db *
  wdg_style_db () const;

  void
  set_wdg_style_db ( const ::Wdg::DS_Widget_Style_Db * style_db_n );

  // Image_Set_Group allocator

  void
  set_image_alloc ( ::dpe::Image_Allocator * alloc_n );

  ::dpe::Image_Allocator *
  image_alloc () const;

  // Show footer

  bool
  footer_visible () const;

  void
  set_footer_visible ( bool flag_n );

  // Wheel degrees

  void
  set_wheel_degrees ( unsigned int delta_n );

  unsigned int
  wheel_degrees () const;

  // Widgets

  QWidget *
  header ();

  QWidget *
  footer ();

  ::Wdg::Sliders_Pad_Header *
  header_cast ();

  ::Wdg::Sliders_Pad_Footer *
  footer_cast ();

  ::Wdg::Sliders_Pad_Header_Data *
  header_data ();

  ::Wdg::Sliders_Pad_Header_Data *
  footer_data ();

  unsigned int
  num_widgets () const;

  QWidget *
  widget ( unsigned int idx_n );

  // Focus info

  const Pad_Focus_Info &
  focus_info () const;

  /// @brief Event handler reimplementation
  bool
  event ( QEvent * event_n );

  // Public signals
  signals:

  void
  sig_focus_changed ();

  void
  sig_footer_label_selected ( unsigned int group_idx_n,
                              unsigned int column_idx_n );

  // Public slots
  public slots:

  bool
  set_focus_proxy ( unsigned int group_idx_n );

  bool
  set_focus_proxy ( unsigned int group_idx_n,
                    unsigned int column_idx_n,
                    unsigned int row_idx_n );

  // Protected methods
  protected:
  void
  clear_widgets ();

  void
  create_widgets ();

  // Event methods

  void
  resizeEvent ( QResizeEvent * event );

  void
  paintEvent ( QPaintEvent * event );

  // Protected slots
  protected slots:

  void
  header_label_selected ( unsigned int group_idx_n, unsigned int column_idx_n );

  void
  footer_label_selected ( unsigned int group_idx_n, unsigned int column_idx_n );

  // Private methods
  private:
  void
  update_colors ();

  // Private attributes
  private:
  QList<::Wdg::Pad_Proxies_Group * > _proxies_groups;
  ::Wdg::Sliders_Pad_Data * _sp_data;

  QList< QWidget * > _widgets;
  ::Wdg::Pad_Focus_Info _focus_info;

  bool _update_decoration;

  unsigned int _wheel_degrees;

  ::Wdg::Sliders_Pad_Style * _sp_style; // Paints decoration graphics
  const ::Wdg::DS_Widget_Style_Db * _wdg_style_db;
  ::dpe::Image_Allocator * _image_alloc;
};

inline unsigned int
Sliders_Pad::wheel_degrees () const
{
  return _wheel_degrees;
}

inline const QList<::Wdg::Pad_Proxies_Group * > &
Sliders_Pad::proxies_groups () const
{
  return _proxies_groups;
}

inline unsigned int
Sliders_Pad::num_proxies_groups () const
{
  return _proxies_groups.size ();
}

inline Pad_Proxies_Group *
Sliders_Pad::proxies_group ( unsigned int idx_n )
{
  return _proxies_groups[ idx_n ];
}

inline const ::Wdg::DS_Widget_Style_Db *
Sliders_Pad::wdg_style_db () const
{
  return _wdg_style_db;
}

inline ::dpe::Image_Allocator *
Sliders_Pad::image_alloc () const
{
  return _image_alloc;
}

inline unsigned int
Sliders_Pad::num_widgets () const
{
  return _widgets.size ();
}

inline QWidget *
Sliders_Pad::widget ( unsigned int idx_n )
{
  return _widgets[ idx_n ];
}

inline const Pad_Focus_Info &
Sliders_Pad::focus_info () const
{
  return _focus_info;
}

} // namespace Wdg

#endif
