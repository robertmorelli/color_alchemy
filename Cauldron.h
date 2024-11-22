#ifndef CAULDRON_H
#define CAULDRON_H
#include "qwidget.h"
#include <QPieSeries>

class Cauldron : public QWidget
{
    Q_OBJECT
public:
    explicit Cauldron(QWidget *parent = nullptr);

    void addColor(QColor newColor);
    QColor getColor();
    QPieSeries* getPieColorSeries();
    void dump();

private:
    QMap<QColor, int> colors;

signals:
};

#endif // CAULDRON_H
