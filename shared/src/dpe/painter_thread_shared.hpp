//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe_paint_thread_shared_hpp__
#define __INC_dpe_paint_thread_shared_hpp__

#include <QMutex>
#include <QWaitCondition>
#include <QList>
#include <QQueue>


// Forward declaration
namespace dpe {
	class Paint_Job;
	class Painter;
	class Image_Set_Meta;
}


namespace dpe
{


/// @brief Painter_Thread_Shared
///
class Painter_Thread_Shared
{
	// Public methods
	public:

	Painter_Thread_Shared ( );

	~Painter_Thread_Shared ( );


	::dpe::Painter *
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


	// Public attributes
	public:

	QList < ::dpe::Painter * > painters;
	/// @brief Fallback painter
	::dpe::Painter * painter_def;

	// Statistics
	QAtomicInt num_rendering;


	// Private attributes;
	private:

	QMutex _queue_mutex;
	QQueue < ::dpe::Paint_Job * > _queue;
	QWaitCondition _queue_cond;

	bool _finished;
	QWaitCondition _finished_cond;
};


} // End of namespace


#endif
