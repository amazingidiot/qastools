/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_sliders_pad_style_hpp__
#define __INC_sliders_pad_style_hpp__

#include <QPainter>
#include <QPen>
#include <QPicture>

// Forward declaration
namespace Wdg
{
class Sliders_Pad_Style_BData;
class Sliders_Pad_Data;
class Sliders_Pad_Data_Group;
class Sliders_Pad_Data_Column;
} // namespace Wdg

namespace Wdg
{

/// @brief Sliders_Pad_Header
///
class Sliders_Pad_Style
{
  // Public methods
  public:
  Sliders_Pad_Style ( Sliders_Pad_Data * data_n );

  virtual ~Sliders_Pad_Style ();

  Sliders_Pad_Data *
  sp_data () const;

  void
  paint_base_decoration ();

  void
  paint_header_decoration ();

  void
  paint_footer_decoration ();

  // Protected methods
  protected:
  // Utility

  double
  calc_col_center ( unsigned int col_width_n,
                    unsigned int col_idx_n,
                    unsigned int num_cols_n ) const;

  QPainterPath
  bridge_path ( Sliders_Pad_Data_Group * sp_grp_n, double y_top_n ) const;

  QPainterPath
  inclined_bridge_path ( Sliders_Pad_Data_Group * sp_grp_n,
                         double y_top_n ) const;

  QPainterPath
  inclined_multi_bridge_path ( Sliders_Pad_Data_Group * sp_grp_n,
                               double y_top_n ) const;

  // Base drawing

  virtual void
  draw_base ();

  void
  dbase_sliders_bridge ( QPainter & pnt_n, Sliders_Pad_Data_Group * sp_grp_n );

  void
  dbase_switches_bridge ( QPainter & pnt_n, Sliders_Pad_Data_Group * sp_grp_n );

  void
  dbase_single_slider_stem ( QPainter & pnt_n,
                             Sliders_Pad_Data_Group * sp_grp_n );

  void
  dbase_single_switch_stem ( QPainter & pnt_n,
                             Sliders_Pad_Data_Group * sp_grp_n );

  void
  dbase_multi_switch_stems ( QPainter & pnt_n,
                             Sliders_Pad_Data_Group * sp_grp_n );

  void
  dbase_labels_connectors ( QPainter & pnt_n,
                            Sliders_Pad_Data_Group * sp_grp_n );

  // Header drawing

  virtual void
  draw_header ();

  void
  dheader_stem ( QPainter & pnt_n, Sliders_Pad_Data_Group * sp_grp_n );

  void
  dheader_bridge ( QPainter & pnt_n, Sliders_Pad_Data_Group * sp_grp_n );

  // Footer drawing

  virtual void
  draw_footer ();

  void
  dfooter_stem ( QPainter & pnt_n,
                 Sliders_Pad_Data_Group * sp_grp_n,
                 Sliders_Pad_Data_Column * sp_col_n );

  // Public attributes
  public:
  QPen stem_pen;
  double stem_corner_indent;

  // Private attributes
  private:
  Sliders_Pad_Data * _sp_data;

  // Painting variables
  unsigned int _area_left;
  unsigned int _area_height;
  unsigned int _y_top;
  unsigned int _y_bottom;
  double _x_mid;
  double _y_mid;
};

inline Sliders_Pad_Data *
Sliders_Pad_Style::sp_data () const
{
  return _sp_data;
}

} // namespace Wdg

#endif
