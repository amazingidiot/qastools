#ifndef OSC_CLIENT_H
#define OSC_CLIENT_H

#include <QTime>
#include <QUrl>

namespace Osc
{
class Client
{
  public:
  Client ( QUrl address );
  ~Client ();

  QUrl address;
  QTime last_subscription;

  void
  subscribe ();
}
} // namespace Osc

#endif