#include "client.h"
#include <QDebug>
#include <QCoreApplication>
#include <QByteArray>
#include <QDataStream>
#include <QHostAddress>
#include <iostream>

Client::Client(
    QString _host, uint16_t _port
  ) :
    m_host(_host),
    m_port(_port)
{
  m_socketConnection = new QTcpSocket(this);

  m_socketConnection->connectToHost(m_host, m_port);

  // отключение
  connect(m_socketConnection, &QTcpSocket::disconnected, this, &QTcpSocket::deleteLater);
  connect(m_socketConnection, &QTcpSocket::disconnected, this, [=]()
                                                               {
                                                                 emit disconnected("- Server disconnected");
                                                               }
                                                               );

  // подключение
  connect(m_socketConnection, &QTcpSocket::connected, this, &Client::slotConnected);

  // пришло сообщение
  connect(m_socketConnection, &QTcpSocket::readyRead, this, &Client::slotRead);

  // ошибка
  connect(m_socketConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));
}

Client::~Client()
{
  delete m_socketConnection;
}

void Client::sendMsg(
    QString _msg
  )
{
    // Шаг 1 - считаем кол-во символов (получаем int)
    // int переводим в QString
    // считаем кол-во символов получившейся стройки
    // формируем строчку по маске хххххххххх
  QString msgSize;

  QString tempSize = QString::number(_msg.size());

  for (int i = 0; i < m_conn_size - tempSize.size(); i++)
    msgSize.append("0");

  msgSize.append(tempSize);

  QByteArray block;

  block.append(msgSize.toUtf8());

  block.append(_msg.toUtf8());

  if (m_socketConnection->state() == QTcpSocket::ConnectedState)
    m_socketConnection->write(block);
}

void Client::endConnection()
{
  m_socketConnection->disconnectFromHost();
}

void Client::slotRead()
{
  // По протоколу стыковки первые 10 байт - это размер сообщения
  // если доступных байт меньше 10ти, ждём пока придут
  if (m_socketConnection->bytesAvailable() < m_conn_size)
    return;

  int msgSize = m_socketConnection->read(m_conn_size).toInt();

  QString msg;

  // Если мы прочитали размер посылки, крутимся в вайле пока не вычитаем все сообщение
  while (msgSize > 0)
  {
    QByteArray buf = 0;

    if (msgSize > m_max_read_buf)
      buf = m_socketConnection->read(m_max_read_buf);

    else
      buf = m_socketConnection->read(msgSize);

    msg.append(buf);

    msgSize -= buf.size();
  }

  // первое сообщение с именем клиента
  if (m_serverName == "")
  {
    m_serverName = msg;

    emit serverName(m_serverName);

    }
  else
    emit newEvent(msg);
}

void Client::slotConnected()
{
  emit connected();
}

void Client::slotError(
    QAbstractSocket::SocketError _error
  )
{
  QString text = "";

  if (_error == QAbstractSocket::ConnectionRefusedError ||
      _error == QAbstractSocket::HostNotFoundError)
        text = "- Connection failed!";

  if (_error == QAbstractSocket::RemoteHostClosedError)
    text = "- HostClosedError!";

  emit disconnected(text);
}
