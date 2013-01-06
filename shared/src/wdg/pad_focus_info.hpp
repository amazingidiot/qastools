//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//


#ifndef __INC_pad_focus_info_hpp__
#define __INC_pad_focus_info_hpp__


namespace Wdg
{

/// @brief Specifies which element of a slider/switches pad has the focus
///
class Pad_Focus_Info
{
	// Public methods
	public:

	Pad_Focus_Info ( );

	void
	clear ( );


	// Public attributes

	bool has_focus;
	unsigned int group_idx;
	unsigned int column_idx;
	unsigned int row_idx;
};


} // End of namespace


#endif
