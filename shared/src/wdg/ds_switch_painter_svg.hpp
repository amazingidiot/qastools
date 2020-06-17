/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_ds_switch_painter_svg_hpp__
#define __INC_ds_switch_painter_svg_hpp__

#include "wdg/ds_widget_painter.hpp"
#include <QImage>
#include <QString>

namespace Wdg
{
namespace Painter
{

class DS_Switch_Painter_SVG : public ::Wdg::Painter::DS_Widget_Painter
{
  // Public methods
  public:
  DS_Switch_Painter_SVG ();

  const QString &
  base_dir () const;

  void
  set_base_dir ( const QString & dir_n );

  const QString &
  file_prefix_bg () const;

  const QString &
  file_prefix_handle () const;

  void
  set_file_prefix_bg ( const QString & file_prefix_n );

  void
  set_file_prefix_handle ( const QString & file_prefix_n );

  /// @brief Checks if all required SVG images exist
  bool
  ready () const;

  // Protected methods
  protected:
  int
  paint_image ( ::dpe::Paint_Job * pjob_n );

  // Private methods
  private:
  int
  paint_bg ( ::dpe::Paint_Job * pjob_n ) const;

  int
  paint_handle ( ::dpe::Paint_Job * pjob_n ) const;

  QString
  file_name ( const QString & prefix_n, const QString & suffix_n ) const;

  bool
  file_ready ( const QString & file_name_n ) const;

  /// @return 0 on success (no error)
  int
  render_svg ( ::dpe::Image & img_n, const QString & svg_file_n ) const;

  // Private attributes
  private:
  static const unsigned int num_bg = 4;
  static const unsigned int num_handle = 10;

  QString _suffix_bg[ num_bg ];
  QString _suffix_handle[ num_handle ];

  QString _base_dir;
  QString _prefix_bg;
  QString _prefix_handle;
};

inline const QString &
DS_Switch_Painter_SVG::base_dir () const
{
  return _base_dir;
}

inline const QString &
DS_Switch_Painter_SVG::file_prefix_bg () const
{
  return _prefix_bg;
}

inline const QString &
DS_Switch_Painter_SVG::file_prefix_handle () const
{
  return _prefix_handle;
}

} // namespace Painter
} // namespace Wdg

#endif
