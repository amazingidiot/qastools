//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_dpe_paint_thread_hpp__
#define __INC_dpe_paint_thread_hpp__

#include <QThread>


// Forward declaration
namespace dpe {
	class Painter_Thread_Shared;
	class Paint_Job;
}


namespace dpe
{


/// @brief Painter_Thread
///
class Painter_Thread :
	public QThread
{
	// Public methods
	public:

	Painter_Thread (
		::dpe::Painter_Thread_Shared & shared_n );


	// Protected methods
	protected:

	void
	run ( );


	// Private attributes
	private:

	::dpe::Painter_Thread_Shared & _shared;
};


} // End of namespace


#endif
