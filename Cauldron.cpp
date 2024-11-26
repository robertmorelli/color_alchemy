#include "Cauldron.h"
#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include <QImage>

Cauldron::Cauldron(QWidget* parent) : QWidget{ parent }, image(":/images/pot.png"), totalColors{ 0 } {
}

void Cauldron::addColor(QColor newColor) {
    if (colors.contains(newColor.name())) {
        colors[newColor.name()]++;
    }
    else {
        colors.insert(newColor.name(), 1);
    }
    totalColors++;

    emit updateDisplay(getColor());
    qDebug() << getColor();
}

QColor Cauldron::getColor() {
    if (!totalColors) return Qt::white;
    double reds = 0, greens = 0, blues = 0;
    for (const QString &key : colors.keys()) {
        double weight = static_cast<double>(colors[key]) / static_cast<double>(totalColors);
        QColor color(key);
        reds += weight * (1.0 - (static_cast<double>(color.red()) / 255.0));
        greens += weight * (1.0 - (static_cast<double>(color.green()) / 255.0));
        blues += weight * (1.0 - (static_cast<double>(color.blue()) / 255.0));
    }
    return QColor(255 * (1 - reds), 255 * (1 - greens), 255 * (1 - blues));
}

void Cauldron::getPieColorSeries(QPieSeries& pieSeries) {
    for (const QColor key : colors.keys()) {
        QPieSlice* pieSlice = new QPieSlice(key.name(), colors[key.name()] / totalColors);
        pieSlice->setColor(key);
        pieSeries.append(pieSlice);
    }
}

void Cauldron::dump() {
    colors.clear();
    totalColors = 0;
    emit updateDisplay(getColor());
}

void Cauldron::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect widgetRect = event->rect();
    painter.drawImage(widgetRect, image, image.rect());
}
