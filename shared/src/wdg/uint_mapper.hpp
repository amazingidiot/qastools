/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_uint_mapper_hpp__
#define __INC_wdg_uint_mapper_hpp__

namespace Wdg
{

///
/// @return The distance between the values as unsigned int
///
unsigned int
integer_distance ( int int_min_n, int int_max_n );

unsigned long
integer_distance ( long int_min_n, long int_max_n );

unsigned int
permille ( unsigned long current_n, unsigned long total_n );

///
/// @brief ulong to ulong span mapper interface definition class
///
class UInt_Mapper
{
  // Public methods
  public:
  virtual ~UInt_Mapper ();

  virtual unsigned long
  map ( unsigned long val_n ) const = 0;
};

///
/// @brief Maps down to a coarser range
///
class UInt_Mapper_Down : public UInt_Mapper
{
  // Public methods
  public:
  ///
  /// @param max_idx_fine The maximum index of the fine ladder (num_steps - 1).
  /// @param max_idx_coarse The maximum index of the coarse ladder (num_steps -
  /// 1).
  UInt_Mapper_Down ( unsigned long max_idx_fine = 1,
                     unsigned long max_idx_coarse = 1 );

  unsigned long
  map ( unsigned long val_n ) const;

  // Private attributes
  private:
  unsigned long _max_idx_fine;
  unsigned long _max_idx_coarse;

  unsigned long _dist_small;
  unsigned long _dist_large;
  unsigned long _len_mod;
  unsigned long _mid_pos;
};

///
/// @brief Maps up to a finer range
///
class UInt_Mapper_Up : public UInt_Mapper
{
  // Public methods
  public:
  ///
  /// @param max_idx_coarse The maximum index of the coarse ladder (num_steps -
  /// 1).
  /// @param max_idx_fine The maximum index of the fine ladder (num_steps - 1).
  UInt_Mapper_Up ( unsigned long max_idx_coarse = 1,
                   unsigned long max_idx_fine = 1 );

  unsigned long
  map ( unsigned long val_n ) const;

  unsigned long
  min_dist () const;

  // Private attributes
  private:
  unsigned long _max_idx_coarse;
  unsigned long _max_idx_fine;

  unsigned long _len_div;
  unsigned long _len_mod;
};

inline unsigned long
UInt_Mapper_Up::min_dist () const
{
  return _len_div;
}

class UInt_Mapper_Auto
{
  // Public methods
  public:
  UInt_Mapper_Auto ( unsigned long max_idx_1_n = 0,
                     unsigned long max_idx_2_n = 0 );

  unsigned long
  max_idx_1 () const;

  unsigned long
  max_idx_2 () const;

  void
  set_max_idx_all ( unsigned long max_idx_1_n, unsigned long max_idx_2_n );

  unsigned long
  v1_to_v2 ( unsigned long val_n ) const;

  unsigned long
  v2_to_v1 ( unsigned long val_n ) const;

  // Protected methods
  protected:
  // Private attributes
  private:
  unsigned long _max_idx_1;
  unsigned long _max_idx_2;

  UInt_Mapper_Up _mapper_up;
  UInt_Mapper_Down _mapper_down;
};

inline unsigned long
UInt_Mapper_Auto::max_idx_1 () const
{
  return _max_idx_1;
}

inline unsigned long
UInt_Mapper_Auto::max_idx_2 () const
{
  return _max_idx_2;
}

} // namespace Wdg

#endif
