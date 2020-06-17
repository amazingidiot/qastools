/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_dpe_painter_hpp__
#define __INC_dpe_painter_hpp__

// Forward declaration
namespace dpe
{
class Image_Set;
class Image_Set_Meta;
class Paint_Job;
} // namespace dpe

namespace dpe
{

/// @brief Painter
///
class Painter
{
  // Public methods
  public:
  Painter ( unsigned int group_type_n, unsigned int group_variant_n = 0 );

  virtual ~Painter ();

  unsigned int
  group_type () const;

  unsigned int
  group_variant () const;

  void
  set_group_variant ( unsigned int variant_n );

  int
  check_and_paint ( ::dpe::Paint_Job * pjob_n );

  // Protected methods
  protected:
  /// @brief Single image painting method
  ///
  /// @return 0 on success ( no error )
  virtual int
  paint_image ( ::dpe::Paint_Job * pjob_n ) = 0;

  // Private attributes
  private:
  /// @brief Type of the group this painter knows how to paint
  unsigned int _group_type;
  /// @brief Group variant id
  unsigned int _group_variant;
};

inline unsigned int
Painter::group_type () const
{
  return _group_type;
}

inline unsigned int
Painter::group_variant () const
{
  return _group_variant;
}

} // namespace dpe

#endif
