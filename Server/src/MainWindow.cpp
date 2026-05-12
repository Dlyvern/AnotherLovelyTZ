#include "MainWindow.hpp"

#include <QLabel>
#include <QSizePolicy>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

namespace alt
{
    MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
    {
        m_server = new Server(this);
        m_cpuGraph = new CPUGraph(this);
        m_statusLabel = new QLabel(this);
        m_clientTimeoutTimer = new QTimer(this);

        this->setWindowTitle("CPU load monitor");
        this->resize(800, 600);

        auto* centralWidget = new QWidget(this);
        auto* layout = new QVBoxLayout(centralWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(m_statusLabel);
        layout->addWidget(m_cpuGraph, 1);
        this->setCentralWidget(centralWidget);

        m_statusLabel->setContentsMargins(8, 2, 8, 2);
        m_statusLabel->setFixedHeight(24);
        m_statusLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        setStatus("Waiting for client...", "#666666");

        m_clientTimeoutTimer->setInterval(3000);
        m_clientTimeoutTimer->setSingleShot(true);

        if(!m_server->start())
            throw std::runtime_error("Failed to create server");

        connect(m_server, &Server::newCPUDataReceived, this, &MainWindow::onNewCPUDataReceived);
        connect(m_clientTimeoutTimer, &QTimer::timeout, this, &MainWindow::onClientTimeout);
    }

    void MainWindow::setStatus(const QString& text, const QString& color)
    {
        m_statusLabel->setText(text);
        m_statusLabel->setStyleSheet(QString("color: %1; font-weight: 600;").arg(color));
    }

    void MainWindow::onNewCPUDataReceived(CPULoadProtocol data)
    {
        m_clientTimeoutTimer->start();

        if(data.status != 0)
        {
            setStatus("CPU read error", "#b00020");
            return;
        }

        setStatus(QString("Connected - %1 cores").arg(data.coreCount), "#1b7f3a");
        m_cpuGraph->addCPUData(data);
    }

    void MainWindow::onClientTimeout()
    {
        setStatus("Client disconnected", "#b36b00");
    }

} //namespace alt
