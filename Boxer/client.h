#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>

class Client : public QObject
{
  Q_OBJECT

public:
  Client(
      QString _host,
      uint16_t _port
    );

  ~Client();

private:
  QTcpSocket            *m_socketConnection = nullptr ;

  QString                           m_host,
                               m_serverName =   ""    ;

  uint16_t                          m_port;

  int                        m_max_read_buf =   512   , // размер чтения буфера
                                m_conn_size =   10    ; // протокол стыковки

public slots:
  void sendMsg(
      QString
    );

  void endConnection();

  void slotRead();

  void slotConnected();

  void slotError(
      QAbstractSocket::SocketError
    );

signals:
  void connected();

  void disconnected(
      QString _text
    );

  void newEvent(
      QString
    );

  void serverName(
      QString
    );
};

#endif // CLIENT_H
