#include "Cauldron.h"
#include <QColor>

Cauldron::Cauldron(QWidget *parent)
    : QWidget{parent}
{
}

void Cauldron::addColor(QColor newColor) {
    if(colors.contains(newColor.name())) {
        colors[newColor.name()]++;
    } else {
        colors.insert(newColor.name(), 1);
    }
    totalColors++;
}

QColor Cauldron::getColor() {
    if(!totalColors) return Qt::white;
    double reds = 0;
    double greens = 0;
    double blues = 0;
    for(const QString &key : colors.keys()) {
        double weight = colors[key]/totalColors;
        QColor color(key);
        reds += weight * (1 - (color.red()/255.0));
        greens += weight * (1 - (color.green()/255.0));
        blues += weight * (1 - (color.blue()/255.0));
    }
    QColor color((char)(255*(1-reds)),(char)(255*(1-greens)),(char)(255*(1-blues)));
    return color;
}

void Cauldron::getPieColorSeries(QPieSeries &pieSeries) {
    for(const QColor key : colors.keys()) {
        QPieSlice* pieSlice = new QPieSlice(key.name(), colors[key.name()]/totalColors);
        pieSlice->setColor(key);
        pieSeries.append(pieSlice);
    }
}

void Cauldron::dump() {
    colors.clear();
    totalColors = 0;
}
