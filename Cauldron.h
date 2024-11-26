#ifndef CAULDRON_H
#define CAULDRON_H
#include <QWidget>
#include <QPieSeries>
#include <QString>

class Cauldron : public QWidget
{
    Q_OBJECT
public:
    explicit Cauldron(QWidget* parent = nullptr);

    QColor getColor();
    void getPieColorSeries(QPieSeries&);
    void dump();

private:
    QMap<QString, int> colors;
    int totalColors;

public slots:
    void addColor(QColor newColor);

signals:
    void updateDisplay(QColor color);

};

#endif // CAULDRON_H
