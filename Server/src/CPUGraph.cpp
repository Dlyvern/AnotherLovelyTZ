#include "CPUGraph.hpp"

#include "qcustomplot.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QPen>
#include <QtGlobal>

namespace alt
{
    CPUGraph::CPUGraph(QWidget* parent) : QWidget(parent)
    {
        m_plot = new QCustomPlot(this);

        m_plot->addGraph();
        m_plot->graph(0)->setName("Total CPU");
        m_plot->graph(0)->setPen(QPen(Qt::blue, 2));

        m_plot->xAxis->setLabel("Time (s)");
        m_plot->yAxis->setLabel("CPU load (%)");
        m_plot->xAxis->setRange(0.0, 60.0);
        m_plot->yAxis->setRange(0.0, 100.0);
        m_plot->legend->setVisible(true);

        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(6, 2, 6, 6);
        layout->setSpacing(2);

        m_controlsLayout = new QGridLayout();
        m_controlsLayout->setContentsMargins(0, 0, 0, 0);
        m_controlsLayout->setHorizontalSpacing(8);
        m_controlsLayout->setVerticalSpacing(0);

        auto* totalToggle = new QCheckBox("Total CPU", this);
        totalToggle->setChecked(true);
        totalToggle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        connect(totalToggle, &QCheckBox::toggled, this, [this](bool checked) {
            setGraphVisible(0, checked);
        });

        m_graphToggles.push_back(totalToggle);
        m_controlsLayout->addWidget(totalToggle, 0, 0);

        layout->addLayout(m_controlsLayout);
        layout->addWidget(m_plot, 1);
    }

    void CPUGraph::setGraphVisible(int graphIndex, bool visible)
    {
        if(!m_plot || graphIndex < 0 || graphIndex >= m_plot->graphCount())
            return;

        m_plot->graph(graphIndex)->setVisible(visible);
        m_plot->replot();
    }

    void CPUGraph::ensureCoreGraphs(int coreCount)
    {
        static const QColor colors[] = {
            Qt::red,
            Qt::darkGreen,
            Qt::magenta,
            Qt::cyan,
            Qt::darkYellow,
            Qt::darkRed,
            Qt::darkCyan,
            Qt::darkMagenta
        };

        while(m_coreGraphCount < coreCount)
        {
            int graphIndex = m_plot->graphCount();
            m_plot->addGraph();
            m_plot->graph(graphIndex)->setName(QString("CPU %1").arg(m_coreGraphCount));
            m_plot->graph(graphIndex)->setPen(QPen(colors[m_coreGraphCount % 8], 1));

            auto* toggle = new QCheckBox(QString("CPU %1").arg(m_coreGraphCount), this);
            toggle->setChecked(false);
            toggle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            m_plot->graph(graphIndex)->setVisible(false);

            connect(toggle, &QCheckBox::toggled, this, [this, graphIndex](bool checked) {
                setGraphVisible(graphIndex, checked);
            });

            m_graphToggles.push_back(toggle);

            int toggleIndex = m_graphToggles.size() - 1;
            int row = toggleIndex / 8;
            int column = toggleIndex % 8;
            m_controlsLayout->addWidget(toggle, row, column);

            m_coreGraphCount++;
        }
    }

    void CPUGraph::addCPUData(const CPULoadProtocol& data)
    {
        if(!m_plot)
            return;

        double totalValue = qBound(0.0, data.totalCPULoad, 100.0);
        m_plot->graph(0)->addData(m_timeSeconds, totalValue);

        ensureCoreGraphs(data.coreCount);

        for(int i = 0; i < data.coreCount && i < m_coreGraphCount; ++i)
        {
            double coreValue = qBound(0.0, data.coreLoads[i], 100.0);
            m_plot->graph(i + 1)->addData(m_timeSeconds, coreValue);
        }

        m_plot->xAxis->setRange(m_timeSeconds, 60.0, Qt::AlignRight);
        m_plot->replot();

        m_timeSeconds += 1.0;
    }
} //namespace alt
