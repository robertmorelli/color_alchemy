#ifndef MIXINGPIECHART_H
#define MIXINGPIECHART_H

#include <QWidget>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>

//QChartView
class MixingPieChart : public QWidget
{
    Q_OBJECT
private:
    QPieSeries series;

public:
    explicit MixingPieChart(QWidget *parent = nullptr);

signals:
};

#endif // MIXINGPIECHART_H
