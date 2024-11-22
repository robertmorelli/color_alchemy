#include "Cauldron.h"

Cauldron::Cauldron(QWidget *parent)
    : QWidget{parent}
{
}

void Cauldron::addColor(QColor newColor) {

}

QColor Cauldron::getColor() {

}

void Cauldron::getPieColorSeries(QPieSeries &pieSeries) {
    int totalColors = 0;

    for(auto count : colors.values()) {
        totalColors += count;
    }

    for(const QColor &key : colors.keys()) {
        QPieSlice* pieSlice = new QPieSlice(key.name(), colors[key]/totalColors);
        pieSlice->setColor(key);
        pieSeries.append(pieSlice);
    }
}

void Cauldron::dump() {
    colors.clear();
}
