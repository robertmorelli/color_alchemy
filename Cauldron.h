#ifndef CAULDRON_H
#define CAULDRON_H
#include <QWidget>
#include <QPieSeries>
#include <QString>

class Cauldron : public QWidget
{
    Q_OBJECT
public:
    explicit Cauldron(QWidget *parent = nullptr);

    void addColor(QColor newColor);
    QColor getColor();
    void getPieColorSeries(QPieSeries&);
    void dump();

private:
    QMap<QString, int> colors;
    int totalColors;

signals:
};

#endif // CAULDRON_H
