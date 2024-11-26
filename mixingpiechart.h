#ifndef MIXINGPIECHART_H
#define MIXINGPIECHART_H

#include <QWidget>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QScopedPointer>
#include <QDebug>

//QChartView
class MixingPieChart : public QWidget
{
    Q_OBJECT
private:
    QPieSeries* series;
    QChart* chart;
    QChartView* view;

public:
    explicit MixingPieChart(QWidget *parent = nullptr);

public slots:
    void updatePieSeries(const QMap<QString, int>& colors, int totalColors);

signals:
};

#endif // MIXINGPIECHART_H
