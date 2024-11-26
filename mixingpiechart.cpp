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


//overload paint call to use svg render of pie chart
void MixingPieChart::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    /*paint this svg
    <svg height="20" width="20" viewBox="0 0 20 20">
  <circle r="10" cx="10" cy="10" fill="white" />
  <circle r="5" cx="10" cy="10" fill="transparent"
          stroke="tomato"
          stroke-width="10"
          stroke-dasharray="calc(35 * 31.4 / 100) 31.4"
          transform="rotate(-90) translate(-20)" />
</svg>
    */
   //make svg with dataurl
   QImage image("data:image/svg+xml,<svg height='20' width='20' viewBox='0 0 20 20'><circle r='10' cx='10' cy='10' fill='white' /><circle r='5' cx='10' cy='10' fill='transparent' stroke='tomato' stroke-width='10' stroke-dasharray='calc(35 * 31.4 / 100) 31.4' transform='rotate(-90) translate(-20)' /></svg>");
    QPainter svgPainter(&image);
    svgPainter.setRenderHint(QPainter::Antialiasing);
    svgPainter.setRenderHint(QPainter::SmoothPixmapTransform);
    svgPainter.setRenderHint(QPainter::TextAntialiasing);

}
