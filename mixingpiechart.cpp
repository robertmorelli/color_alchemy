#include "mixingpiechart.h"

MixingPieChart::MixingPieChart(QWidget *parent): QWidget{parent}, series() {
    series.append("red",0.5);
    series.append("blue",0.25);
    series.append("green",0.125);
    series.append("orange",0.125);
    QChart* chart = new QChart();
    chart->addSeries(&series);
    QChartView* view = new QChartView(chart);




    view->setRenderHint(QPainter::Antialiasing);
    view->setParent(this);
    view->resize(200,200);
    view->setStyleSheet(QString::fromUtf8("image: url(:/images/pot.png) 0 0 0 0 stretch stretch;\n"
"border-image: none;\n"
""));
    view->setBackgroundBrush(Qt::transparent);
    view->setBackgroundRole(QPalette::Window);
    view->setAutoFillBackground(false);
    view->setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
    setBackgroundRole(QPalette::Window);
    view->show();




    // setParent(this);
    // resize(200,200);
    // show();
    // setStyleSheet(QString::fromUtf8("image: url(:/images/pot.png) 0 0 0 0 stretch stretch;\n" "border-image: none;\n"""));
    // setBackgroundRole(QPalette::Window);
    // setAutoFillBackground(false);
}
