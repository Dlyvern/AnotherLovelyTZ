#ifndef ALT_MAIN_WINDOW //ALT - Prefix for AnotherLovelyTZ
#define ALT_MAIN_WINDOW

#include <QMainWindow>

#include "CPUGraph.hpp"
#include "Server.hpp"

class QLabel;
class QTimer;

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
        QLabel* m_statusLabel{nullptr};
        QTimer* m_clientTimeoutTimer{nullptr};

        void setStatus(const QString& text, const QString& color);

    private slots:
        void onNewCPUDataReceived(CPULoadProtocol data);
        void onClientTimeout();
    };
} //namespace alt

#endif //ALT_MAIN_WINDOW
