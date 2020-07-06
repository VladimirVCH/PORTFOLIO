#include "server.h"
#include <QDebug>
#include <QDataStream>

Server::Server(
    QString _host,
    uint16_t _port
  )
{
  m_serverSocket = new QTcpServer(this);

  if (!m_serverSocket->listen(QHostAddress(_host), _port))
  {
    emit endConn(m_serverSocket->errorString());

    return;
  }

  // новое соединение
  connect(m_serverSocket, &QTcpServer::newConnection, this, &Server::slotNewConnection);
}

Server::~Server()
{
  delete m_serverSocket;
}

void Server::slotNewConnection()
{
  emit newConn();

  m_client = m_serverSocket->nextPendingConnection();

  // отключение
  connect(m_client, &QTcpSocket::disconnected, m_client, &QTcpSocket::deleteLater);
  connect(m_client, &QTcpSocket::disconnected, this, [=]()
                                                     {
                                                       emit endConn("- Client disconnected");
                                                     }
                                                     );

  // пришло сообщение
  connect(m_client, &QTcpSocket::readyRead, this, &Server::slotReadClient);
}

void Server::slotReadClient()
{
  // По протоколу стыковки первые 10 байт - это размер сообщения
  // если доступных байт меньше 10ти, ждём пока придут
  if (m_client->bytesAvailable() < m_conn_size)
    return;

  int msgSize = m_client->read(m_conn_size).toInt();

  QString msg;

  // Если мы прочитали размер посылки, крутимся в вайле пока не вычитаем все сообщение
  while (msgSize > 0)
  {
    QByteArray buf = 0;

    if (msgSize > m_max_read_buf)
      buf = m_client->read(m_max_read_buf);

    else
      buf = m_client->read(msgSize);

    msg.append(buf);

    msgSize -= buf.size();
  }

  // первое сообщение с именем клиента
  if(m_clientName == "")
  {
    m_clientName = msg;

    emit clientName(m_clientName);
  }
  else
    emit newEvent(msg);
}

void Server::sendMsg(
    QString _msg
  )
{
  QString msgSize;

  QString tempSize = QString::number(_msg.size());

  for (int i = 0; i < 10 - tempSize.size(); i++)
    msgSize.append("0");

  msgSize.append(tempSize);

  QByteArray block;

  block.append(msgSize.toUtf8());

  block.append(_msg.toUtf8());

  if (m_client->state() == QTcpSocket::ConnectedState)
    m_client->write(block);
}

void Server::slotErrorFromClient(
    QAbstractSocket::SocketError _error
  )
{
  QString err(_error);

  emit endConn("- " + err);
}

void Server::slotErrorServer(
    QAbstractSocket::SocketError _error
  )
{
  QString err(_error);

  emit endConn("- " + err);
}
