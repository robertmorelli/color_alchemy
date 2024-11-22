#include "Cauldron.h"

Cauldron::Cauldron(QWidget *parent)
    : QWidget{parent}
{
}

void Cauldron::addColor(QColor newColor) {

}

QColor Cauldron::getColor() {

}

QPieSeries* Cauldron::getPieColorSeries() {
    int totalColors = 0;
    QPieSeries* pieSeries = new QPieSeries();

    for(auto count : colors.values()) {
        totalColors += count;
    }

    for(const QColor &key : colors.keys()) {
        QPieSlice* pieSlice = new QPieSlice(key.name(), colors[key]/totalColors);
        pieSlice->setColor(key);
        pieSeries->append(pieSlice);
    }

    return pieSeries;
}

void Cauldron::dump() {
    colors.clear();
}
