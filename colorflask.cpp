#include "colorflask.h"
#include <QColor>

ColorFlask::ColorFlask(QWidget *parent, QColor colorHex)
    : QWidget{parent} , color(colorHex)
{

}
