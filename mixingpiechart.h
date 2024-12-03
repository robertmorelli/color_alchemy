#ifndef MIXINGPIECHART_H
#define MIXINGPIECHART_H

#include <QDebug>
#include <QScopedPointer>
#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>

// QChartView
class MixingPieChart : public QWidget {
    Q_OBJECT
private:
    QPieSeries* series;
    QChart* chart;
    QChartView* view;

public:
    explicit MixingPieChart(QWidget* parent = nullptr);

public slots:
    /// @brief Update the pie chart with the new colors
    void updatePieSeries(const QMap<QString, int>& colors, int totalColors);

signals:
};

#endif // MIXINGPIECHART_H
