/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_udev_device_lookout_hpp__
#define __INC_qsnd_udev_device_lookout_hpp__

#include <libudev.h>
#include <QObject>
#include <string>

namespace QSnd
{

/// @brief UDev_Device_Lookout
///
class UDev_Device_Lookout : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  UDev_Device_Lookout ( QObject * parent_n = 0 );

  ~UDev_Device_Lookout ();

  // Signals
  signals:

  void
  sig_change ();

  // Protected slots
  protected slots:

  void
  udev_process ();

  // Private methods
  private:
  void
  udev_init ();

  void
  udev_close ();

  // Private attributes
  private:
  // udev device detection
  ::udev * _udev;
  ::udev_monitor * _udev_mon;
};

} // namespace QSnd

#endif
