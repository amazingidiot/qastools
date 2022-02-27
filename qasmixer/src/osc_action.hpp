#ifndef OSC_ACTION_HPP
#define OSC_ACTION_HPP

#include "osc_message.hpp"
#include "osc_server.hpp"
#include <QRegularExpression>
#include <functional>

namespace Osc
{
class Server;

class Action
{
  public:
  Action ( QRegularExpression address,
           std::function< bool ( Osc::Server * server,
                                 Osc::Message * message ) > run );

  QRegularExpression address;
  std::function< bool ( Osc::Server * server, Osc::Message * message ) > run;
};
} // namespace Osc

#endif
