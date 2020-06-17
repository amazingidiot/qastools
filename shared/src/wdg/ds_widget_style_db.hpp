/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_ds_imaging_style_hpp__
#define __INC_wdg_ds_imaging_style_hpp__

#include "wdg/ds_widget_types.hpp"
#include <QHash>
#include <QPalette>

namespace Wdg
{

class DS_Widget_Style_Db
{
  // Public typedefs
  public:
  enum STYLES
  {
    ST_NORMAL = 0,
    ST_USER = 32 // User defined style should start here
  };

  // Public methods
  public:
  DS_Widget_Style_Db ();

  QPalette
  palette ( unsigned int style_id_n ) const;

  const QColor &
  color ( unsigned int style_id_n, QPalette::ColorRole role_n ) const;

  const QColor &
  color ( unsigned int style_id_n,
          QPalette::ColorGroup group_n,
          QPalette::ColorRole role_n ) const;

  QPalette::ColorGroup
  color_group ( ::Wdg::DS_Widget_State state_n ) const;

  QPalette::ColorGroup
  color_group ( unsigned int state_n ) const;

  // Public attributes
  public:
  QHash< unsigned int, QPalette > palettes;
};

inline QPalette
DS_Widget_Style_Db::palette ( unsigned int style_id_n ) const
{
  return palettes.value ( style_id_n );
}

inline const QColor &
DS_Widget_Style_Db::color ( unsigned int style_id_n,
                            QPalette::ColorRole role_n ) const
{
  return palette ( style_id_n ).color ( role_n );
}

inline const QColor &
DS_Widget_Style_Db::color ( unsigned int style_id_n,
                            QPalette::ColorGroup group_n,
                            QPalette::ColorRole role_n ) const
{
  return palette ( style_id_n ).color ( group_n, role_n );
}

inline QPalette::ColorGroup
DS_Widget_Style_Db::color_group ( ::Wdg::DS_Widget_State state_n ) const
{
  QPalette::ColorGroup res;
  switch ( state_n ) {
  case ::Wdg::ST_DISABLED:
    res = QPalette::Disabled;
    break;
  case ::Wdg::ST_INACTIVE:
    res = QPalette::Inactive;
    break;
  default:
    res = QPalette::Normal;
  }
  return res;
}

inline QPalette::ColorGroup
DS_Widget_Style_Db::color_group ( unsigned int state_n ) const
{
  return color_group ( ( ::Wdg::DS_Widget_State)state_n );
}

} // namespace Wdg

#endif
