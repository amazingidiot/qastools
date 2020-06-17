/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_sliders_pad_data_hpp__
#define __INC_sliders_pad_data_hpp__

#include <QObject>
#include <QPicture>
#include <QString>
#include <vector>

// Forward declarations
namespace Wdg
{
class Pad_Proxies_Group;
class Pad_Proxies_Column;
class Sliders_Pad_Data;
class Sliders_Pad_Data_Group;
class Sliders_Pad_Data_Column;
class Sliders_Pad_Header_Data;
class Sliders_Pad_Header;
class Sliders_Pad_Footer;
} // namespace Wdg

namespace Wdg
{

class Sliders_Pad_Data_Column : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Sliders_Pad_Data_Column ( QObject * parent_n = 0 );

  Sliders_Pad_Data_Group *
  sp_group ();

  Sliders_Pad_Data *
  sp_data ();

  // Public slots
  public slots:

  void
  update_footer_label ();

  // Public attributes
  public:
  unsigned int col_idx;
  unsigned int col_total_idx;

  unsigned int col_pos;
  unsigned int col_width;

  QPicture hd_pics[ 2 ]; // Header/footer pictures

  bool show_value_label;
  Pad_Proxies_Column * sppc;
};

class Sliders_Pad_Data_Group : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Sliders_Pad_Data_Group ( QObject * parent_n = 0 );

  Sliders_Pad_Data *
  sp_data ();

  // Public attributes
  public:
  unsigned int group_idx;
  unsigned int group_pos;
  unsigned int group_width;
  unsigned int num_sliders;
  unsigned int num_switches;
  ::std::vector< Sliders_Pad_Data_Column * > columns;

  QPicture center_pic;   // Center picture
  QPicture hd_pics[ 2 ]; // Header/footer pictures
  Pad_Proxies_Group * sppg;
};

/// @brief Sliders_Pad_Data
///
class Sliders_Pad_Data : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Sliders_Pad_Data ( QObject * parent_n = 0 );

  Sliders_Pad_Header *
  header_cast ();

  Sliders_Pad_Footer *
  footer_cast ();

  // Public attributes
  ::std::vector< Sliders_Pad_Data_Group * > groups;

  // Areas
  QRect header_area;
  QRect inputs_area; // Contains sliders_area and switches_area
  int sliders_area_y;
  int sliders_area_height;
  int sub_slider_area_y;
  int sub_slider_area_height;
  int switches_area_y;
  int switches_area_height;
  QRect footer_area;

  Sliders_Pad_Header_Data * header_data;
  Sliders_Pad_Header_Data * footer_data;

  QWidget * header;
  QWidget * footer;

  bool show_value_labels;
};

inline Sliders_Pad_Data_Group *
Sliders_Pad_Data_Column::sp_group ()
{
  return static_cast< Sliders_Pad_Data_Group * > ( parent () );
}

inline Sliders_Pad_Data *
Sliders_Pad_Data_Column::sp_data ()
{
  return sp_group ()->sp_data ();
}

inline Sliders_Pad_Data *
Sliders_Pad_Data_Group::sp_data ()
{
  return static_cast< Sliders_Pad_Data * > ( parent () );
}

} // namespace Wdg

#endif
