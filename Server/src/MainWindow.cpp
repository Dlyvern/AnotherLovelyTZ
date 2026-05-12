#include "MainWindow.hpp"
#include <QDebug>

namespace alt
{
    MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
    {
        m_server = new Server(this);
        m_cpuGraph = new CPUGraph(this);

        this->setWindowTitle("CPU load monitor");
        this->resize(800, 600);
        this->setCentralWidget(m_cpuGraph);

        //This is how google writes its code...
        if(!m_server->start())
            throw std::runtime_error("Failed to create server");

        connect(m_server, &Server::newCPUDataReceived, this, &MainWindow::onNewCPUDataReceived);
    }

    void MainWindow::onNewCPUDataReceived(CPULoadProtocol data)
    {
        if(data.status != 0)
            return;

        m_cpuGraph->addCPUData(data);
    }

} //namespace alt
