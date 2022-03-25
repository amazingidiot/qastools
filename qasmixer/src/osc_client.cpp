#include "osc_client.hpp"

Osc::Client::Client ( QUrl address )
{
  this->address = address;
  this->last_subscription = QTime::currentTime ();
}
Osc::Client::~Client () {}

Osc::Client::subscribe ()
{
  this->last_subscription = QTime::currentTime ();
}