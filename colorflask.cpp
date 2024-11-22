#include "colorflask.h"
#include <QColor>
#include <QPainter>

ColorFlask::ColorFlask(QWidget *parent, QColor colorHex, QPoint* position)
    : QWidget{parent} , color(colorHex), position(position)
{

}

void ColorFlask::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    //painter.drawImage();
}
