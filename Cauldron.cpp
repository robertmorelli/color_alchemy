#include "Cauldron.h"

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
}
