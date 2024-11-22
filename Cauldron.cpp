#include "Cauldron.h"
#include <QColor>

Cauldron::Cauldron(QWidget *parent)
    : QWidget{parent}
{
}

void Cauldron::addColor(QColor newColor) {
    if(colors.contains(newColor)) {
        colors[newColor]++;
    } else {
        colors.insert(newColor, 1);
    }
    totalColors++;
}

QColor Cauldron::getColor() {
    if(!totalColors) return Qt::white;
    double reds = 0;
    double greens = 0;
    double blues = 0;
    for(const QColor &key : colors.keys()) {
        double weight = colors[key]/totalColors;
        reds += weight * (1 - (key.red()/255.0));
        greens += weight * (1 - (key.green()/255.0));
        blues += weight * (1 - (key.blue()/255.0));
    }
    QColor color((char)(255*(1-reds)),(char)(255*(1-greens)),(char)(255*(1-blues)));
    return color;
}

void Cauldron::getPieColorSeries(QPieSeries &pieSeries) {
    for(const QColor &key : colors.keys()) {
        QPieSlice* pieSlice = new QPieSlice(key.name(), colors[key]/totalColors);
        pieSlice->setColor(key);
        pieSeries.append(pieSlice);
    }
}

void Cauldron::dump() {
    colors.clear();
    totalColors = 0;
}
