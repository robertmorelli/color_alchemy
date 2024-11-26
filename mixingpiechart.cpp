#include "mixingpiechart.h"
#include <QVBoxLayout>

MixingPieChart::MixingPieChart(QWidget *parent): QWidget{parent}, series() {
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    series.append("red", 0.5);
    series.append("blue", 0.25);
    series.append("green", 0.125);
    series.append("orange", 0.125);

    QChart* chart = new QChart();
    chart->setBackgroundVisible(false);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->addSeries(&series);
    chart->legend()->hide();

    QChartView* view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->setStyleSheet("background-color: transparent;");
    view->setAttribute(Qt::WA_TranslucentBackground);

    layout->addWidget(view);

    setLayout(layout);
}
