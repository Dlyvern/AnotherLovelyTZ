#ifndef ALT_SERVER_HPP //ALT - Prefix for AnotherLovelyTZ
#define ALT_SERVER_HPP

#include <QUdpSocket>
#include <QObject>

#include "Protocol.h"

namespace alt
{
    class Server : public QObject
    {
        Q_OBJECT
    public:
        explicit Server(QObject* parent = nullptr);

        bool start();
        void stop();

    signals:
        void newCPUDataReceived(CPULoadProtocol data);

    private:
        QUdpSocket* m_socket{nullptr};

    private slots:
        void readyToReadCallback();
    };

} //namespace alt

#endif //ALT_SERVER_HPP