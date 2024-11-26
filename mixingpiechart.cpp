#include "mixingpiechart.h"
#include <QVBoxLayout>

MixingPieChart::MixingPieChart(QWidget *parent): QWidget{parent}, series() {
    series = new QPieSeries();
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    QPieSlice* slice = series->append("No Colors", 1.0);
    slice->setColor("white");

    chart = new QChart();
    chart->setBackgroundVisible(false);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->addSeries(series);
    chart->legend()->hide();

    view = new QChartView(chart, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setStyleSheet("background-color: transparent;");
    view->setAttribute(Qt::WA_TranslucentBackground);

    layout->addWidget(view);

    setLayout(layout);
}

void MixingPieChart::updatePieSeries(const QMap<QString, int>& colors, int totalColors) {
    series->clear();

    if(colors.isEmpty()) {
        QPieSlice* slice = series->append("No Colors", 1.0);
        slice->setColor("white");
    } else {
        foreach(const QString& key,colors.keys()) {
            QColor color(key);
            if (!color.isValid()) {
                continue;
            }

            double sliceSize = static_cast<double>(colors[key]) / totalColors;
            if(sliceSize > 0) {
                QPieSlice* slice = series->append(key, sliceSize);
                if (slice) {
                    slice->setColor(color);
            }
        }

        if (series->count() == 0) {
            QPieSlice* slice = series->append("No Colors", 1.0);
            slice->setColor("white");
        }
    }

    chart->update();
    view->repaint();
    }
}
