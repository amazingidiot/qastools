/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_dpe_image_set_meta_hpp__
#define __INC_dpe_image_set_meta_hpp__

#include <QSize>

namespace dpe
{

/// @brief Image_Set_Meta
///
class Image_Set_Meta
{
  // Public methods
  public:
  Image_Set_Meta ( unsigned int num_images_n,
                   unsigned int group_type_n,
                   unsigned int type_id_n );

  virtual ~Image_Set_Meta ();

  /// @brief Tests if an other meta information set matches this one
  ///
  /// @return true on match
  virtual bool
  matches ( const Image_Set_Meta * cfg_n ) const;

  /// @brief Creates an exact copy of this meta information
  ///
  /// @return pointer to the new copy
  virtual Image_Set_Meta *
  new_copy () const;

  // Attributes

  /// @brief Image set type
  ///
  /// Used to specify the sub element of an image group.
  /// E.g. slider handle images have a different type_id
  /// than slider background images.
  unsigned int type_id;

  /// @brief Style selection code
  ///
  /// Used to specify a widget color scheme.
  unsigned int style_id;

  /// @brief Style selection sub code
  ///
  /// Used to specify a widget's normal/disabled/inactive state
  unsigned int style_sub_id;

  /// @brief Image size
  QSize size;

  /// @brief Type of the group this set belongs to
  ///
  /// Used to determine the widget type (slider, switch, etc.)
  /// This one must match the group_id value of the respective ::dpe::Painter
  unsigned int group_type;

  /// @brief Variant of the group this set belongs to
  ///
  /// Used to switch to a different style/painter of the same widget
  /// This one must match the group_variant value of the respective
  /// ::dpe::Painter
  unsigned int group_variant;

  /// @brief Number of images in the set
  const unsigned int num_images;
};

} // namespace dpe

#endif
