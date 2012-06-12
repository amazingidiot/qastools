//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_paint_thread_hpp__
#define __INC_dpe2_paint_thread_hpp__

#include "painter.hpp"
#include <QScopedPointer>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>

// Forward declaration
namespace dpe2 {
	class Pixmap;
	class Pixmap_Handle;
	class Painter;
}


namespace dpe2
{


/// @brief Paint_Thread_Shared
///
class Paint_Thread_Shared
{
	// Public methods
	public:

	Paint_Thread_Shared ( );

	~Paint_Thread_Shared ( );


	/* TODO
	::dpe2::Painter *
	find_painter (
		::dpe::Image_Set_Meta * meta_n );

	int
	paint_job (
		::dpe::Paint_Job * pjob_n );


	// Threading

	void
	enqueue_job (
		::dpe::Paint_Job * pjob_n );

	::dpe::Paint_Job *
	fetch_job ( );

	void
	job_finished (
		::dpe::Paint_Job * pjob_n );

	void
	wait_for_finish ( );


	/// @brief sends and abort signal to all threads
	void
	abort_threads (
		unsigned int num_n );

	*/


	// Public attributes
	public:

	QList < ::dpe2::Painter * > painters;
	/// @brief Fallback painter
	QScopedPointer < ::dpe2::Painter > painter_default;

	/// @brief Statistics
	QAtomicInt num_rendering;


	// Private attributes;
	private:

	QMutex _queue_mutex;
	QQueue < ::dpe2::Pixmap_Handle * > _queue;
	QWaitCondition _queue_cond;

	bool _finished;
	QWaitCondition _finished_cond;
};



/// @brief Paint_Thread
///
class Paint_Thread :
	public QThread
{
	// Public methods
	public:

	Paint_Thread (
		::dpe2::Paint_Thread_Shared & shared_n );


	// Protected methods
	protected:

	void
	run ( );


	// Private attributes
	private:

	::dpe2::Paint_Thread_Shared & _shared;
};


} // End of namespace


#endif
