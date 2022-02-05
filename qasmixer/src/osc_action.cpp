#include "osc_action.hpp"

Osc::Action::Action (
    QRegularExpression address,
    std::function< bool ( Osc::Server * server, Osc::Message * message ) > run )
{
  this->address = address;
  this->run = run;
}
