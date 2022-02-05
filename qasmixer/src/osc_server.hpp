#ifndef OSC_SERVER_H
#define OSC_SERVER_H

#include "osc_action.hpp"
#include "osc_message.hpp"
#include "qsnd/cards_model.hpp"
#include "qsnd/controls_database.hpp"
#include "qsnd/controls_model.hpp"
#include "qsnd/ctl_address.hpp"
#
#include <QList>
#include <QNetworkDatagram>
#include <QObject>
#include <QRegularExpression>
#include <QUdpSocket>

namespace Osc
{
class Action;

class Server : public QObject
{
  Q_OBJECT

  friend Osc::Action;

  private:
  ::QSnd::Cards_Model * _cards_model;

  QUdpSocket * _socket;
  bool _enabled = false;
  quint16 _port = 1;

  QList< Osc::Action > actions;

  public:
  Server ();
  ~Server ();

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
  sendOscMessage ( Osc::Message * message );

  signals:
  void
  portChanged ( quint16 value );
  bool
  enabledChanged ( bool value );
};

} // namespace Osc

#endif // OSC_SERVER_H
