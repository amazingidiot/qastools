/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_mixer_hctl_elem_group_hpp__
#define __INC_qsnd_mixer_hctl_elem_group_hpp__

#include <QList>

// Forward declaration
namespace QSnd
{
class Mixer_HCTL_Elem;
}

namespace QSnd
{

/// @brief Mixer_HCTL_Elem_Group
///
class Mixer_HCTL_Elem_Group
{
  // Public methods
  public:
  void
  clear ();

  unsigned int
  num_elems () const;

  void
  append_elem ( Mixer_HCTL_Elem * elem_n );

  Mixer_HCTL_Elem *
  elem ( unsigned int idx_n ) const;

  // Private attributes
  private:
  QList< Mixer_HCTL_Elem * > _snd_elems;
};

inline unsigned int
Mixer_HCTL_Elem_Group::num_elems () const
{
  return _snd_elems.size ();
}

inline void
Mixer_HCTL_Elem_Group::append_elem ( Mixer_HCTL_Elem * elem_n )
{
  _snd_elems.append ( elem_n );
}

inline Mixer_HCTL_Elem *
Mixer_HCTL_Elem_Group::elem ( unsigned int idx_n ) const
{
  return _snd_elems[ idx_n ];
}

} // namespace QSnd

#endif
