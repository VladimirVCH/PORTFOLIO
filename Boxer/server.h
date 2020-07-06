#ifndef SERVER_H
#define SERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>

class Server : public QObject
{
  Q_OBJECT

public:
  Server(
      QString _host,
      uint16_t _port
    );

  ~Server();

private:
  QTcpServer     *m_serverSocket = nullptr ;

  QTcpSocket           *m_client = nullptr ;

  QString           m_clientName =   ""    ;

  int                m_clientNum =    0    ,
                  m_max_read_buf =   512   , // размер чтения буфера
                     m_conn_size =   10    ; // протокол стыковки

  QThread             *m_threadP = nullptr ;

public slots:
  void slotNewConnection();

  void slotReadClient();

  void sendMsg(
      QString
    );

  void slotErrorFromClient(
      QAbstractSocket::SocketError
    );

  void slotErrorServer(
      QAbstractSocket::SocketError
    );

signals:
  void newConn();

  void endConn(
      QString _text
    );

  void clientDisc();

  void newEvent(
      QString
    );

  void newTXT(
      QString,
      int
    );

  void clientName(
      QString
    );
};

#endif // SERVER_H
