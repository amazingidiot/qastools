#ifndef OSC_MESSAGE_H
#define OSC_MESSAGE_H

#include <QByteArray>
#include <QHostAddress>
#include <QList>
#include <QNetworkDatagram>
#include <QVariant>

namespace Osc
{
class Message
{
  public:
  Message ( QNetworkDatagram * datagram );
  Message ( QHostAddress destinationAddress,
            quint16 destinationPort,
            QHostAddress sourceAddress,
            quint16 sourcePort,
            QString address,
            QList< QVariant > values );
  Message ( QString address, QList< QVariant > values );

  QHostAddress sourceAddress;
  quint16 sourcePort;

  QHostAddress destinationAddress;
  quint16 destinationPort;

  QString address;

  QString
  format ();
  QVariantList values;

  QByteArray
  toByteArray ();
};
} // namespace Osc
#endif