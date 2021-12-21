#ifndef OSC_MESSAGE_H
#define OSC_MESSAGE_H

#include <QHostAddress>
#include <QList>
#include <QNetworkDatagram>
#include <QVariant>

namespace Osc
{
class Osc_Message
{
  public:
  Osc_Message ( QNetworkDatagram * datagram );
  Osc_Message ( QHostAddress destinationAddress,
                quint16 destinationPort,
                QHostAddress sourceAddress,
                quint16 sourcePort,
                QString address,
                QVariantList values );

  QHostAddress sourceAddress;
  quint16 sourcePort;

  QHostAddress destinationAddress;
  quint16 destinationPort;

  QString address;

  QString format;
  QVariantList values;
};
} // namespace Osc
#endif