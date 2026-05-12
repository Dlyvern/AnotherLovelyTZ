#include "Server.hpp"
#include "Protocol.h"

namespace alt
{
    Server::Server(QObject* parent) : QObject(parent)
    {
        m_socket = new QUdpSocket(this);
    }

    bool Server::start()
    {
        if(!m_socket->bind(QHostAddress::LocalHost, 1234))
        {
            qWarning() << "Failed to bind UDP socket: " << m_socket->errorString();
            return false;
        }

        connect(m_socket, &QUdpSocket::readyRead, this, &Server::readyToReadCallback);

        return true;
    }

    void Server::stop()
    {
        if(!m_socket)
            return;

        disconnect(m_socket, &QUdpSocket::readyRead, this, &Server::readyToReadCallback);

        if(m_socket->state() != QAbstractSocket::UnconnectedState)
            m_socket->close();
    }

    void Server::readyToReadCallback()
    {
        while(m_socket->hasPendingDatagrams())
        {
            CPULoadProtocol message;

            qint64 size = m_socket->readDatagram(
                reinterpret_cast<char*>(&message),
                sizeof(message)
            );

            if (size != sizeof(message))
            {
                qWarning() << "Invalid datagram size:" << size;
                continue;
            }

            emit newCPUDataReceived(message);
        }
    }

} //namespace alt