/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_dpe_image_request_hpp__
#define __INC_dpe_image_request_hpp__

#include <vector>

// Forward declaration
namespace dpe
{
class Image_Set_Meta;
class Image_Set_Group;
class Image_Set_State;
} // namespace dpe

namespace dpe
{

/// @brief Image_Request request
///
class Image_Request
{
  // Public methods
  public:
  Image_Request ( ::dpe::Image_Set_Group * group_n );

  ~Image_Request ();

  /// @brief To be called by the requesting thread
  ///
  /// This method must be called at some point after
  /// sending this request to an Image_Allocator.
  void
  wait_for_finish ();

  // Public attributes
  public:
  ::dpe::Image_Set_Group * group;
  ::std::vector<::dpe::Image_Set_Meta * > meta;
  ::std::vector<::dpe::Image_Set_State * > states;
};

} // namespace dpe

#endif
