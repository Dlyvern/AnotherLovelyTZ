#ifndef ALT_MAIN_WINDOW //ALT - Prefix for AnotherLovelyTZ
#define ALT_MAIN_WINDOW

#include <QMainWindow>

#include "CPUGraph.hpp"
#include "Server.hpp"

namespace alt
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget* parent = nullptr);

    private:
        Server* m_server{nullptr};
        CPUGraph* m_cpuGraph{nullptr};

    private slots:
        void onNewCPUDataReceived(CPULoadProtocol data);
    };
} //namespace alt

#endif //ALT_MAIN_WINDOW
