#include "mixingpiechart.h"
#include <QSvgRenderer>

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


//overload paint call to use svg render of pie chart
void MixingPieChart::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    // Create SVG renderer
    QSvgRenderer renderer;
    
    // Define the SVG content
    QString svgContent = 
        "<svg height=\"20\" width=\"20\" viewBox=\"0 0 20 20\">"
        "<circle r=\"10\" cx=\"10\" cy=\"10\" fill=\"white\" />"
        "<circle r=\"5\" cx=\"10\" cy=\"10\" fill=\"transparent\""
        "        stroke=\"tomato\""
        "        stroke-width=\"10\""
        "        stroke-dasharray=\"10.99\""
        "</svg>";
    
    // Load SVG content into renderer
    renderer.load(svgContent.toUtf8());
    
    // Define the rendering rectangle
    QRectF rect(0, 0, width(), height());
    
    // Render the SVG
    renderer.render(&painter, rect);
}
