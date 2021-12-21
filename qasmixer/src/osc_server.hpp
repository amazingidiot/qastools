#ifndef OSC_SERVER_H
#define OSC_SERVER_H

#include "osc_message.hpp"
#include "qsnd/cards_model.hpp"
#include <QNetworkDatagram>
#include <QObject>
#include <QUdpSocket>

namespace Osc
{
class Osc_Server : public QObject
{
  Q_OBJECT

  private:
  QSnd::Cards_Model * _cards_model;
  QUdpSocket * _socket;
  bool _enabled = false;
  quint16 _port = 1;

  public:
  Osc_Server ();
  ~Osc_Server ();

  quint16
  port ();

  bool
  enabled ();

  public slots:
  void
  setPort ( quint16 value );
  void
  setEnabled ( bool value );

  void
  receiveDatagram ();

  void
  sendDatagram ( Osc::Osc_Message * message );

  signals:
  void
  portChanged ( quint16 value );
  bool
  enabledChanged ( bool value );
};

} // namespace Osc

#endif // OSC_SERVER_H
