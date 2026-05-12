#ifndef ALT_CPU_GRAPH_HPP //ALT - Prefix for AnotherLovelyTZ
#define ALT_CPU_GRAPH_HPP

#include <QWidget>
#include <QVector>

#include "Protocol.h"

class QCheckBox;
class QCustomPlot;
class QGridLayout;

namespace alt
{
    class CPUGraph : public QWidget
    {
        Q_OBJECT
    public:
        explicit CPUGraph(QWidget* parent = nullptr);

        void addCPUData(const CPULoadProtocol& data);

    private:
        void ensureCoreGraphs(int coreCount);
        void setGraphVisible(int graphIndex, bool visible);

        QCustomPlot* m_plot{nullptr};
        QGridLayout* m_controlsLayout{nullptr};
        QVector<QCheckBox*> m_graphToggles;
        double m_timeSeconds{0.0};
        int m_coreGraphCount{0};
    };
} //namespace alt

#endif //ALT_CPU_GRAPH_HPP
