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
    QImage image;
    int totalColors;

public slots:
    void addColor(QColor newColor);

protected:
    void paintEvent(QPaintEvent *event) override;
signals:
    void updateDisplay(QColor color);
    void colorsChanged(const QMap<QString, int>& colors, int totalColors);
};

#endif // CAULDRON_H
