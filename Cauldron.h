#ifndef CAULDRON_H
#define CAULDRON_H
#include <QWidget>
#include <QPieSeries>
#include <QString>
#include <Box2D/Box2D.h>
#include <QTimer>

class Cauldron : public QWidget
{
    Q_OBJECT
public:
    explicit Cauldron(QWidget* parent = nullptr);

    QColor getColor();
    void dump();

private:
    QMap<QString, int> colors;
    QImage image;
    int totalColors;
    b2World world;
    QTimer timer;
    void simulatePhysics();
    QMap<b2Body*, QColor> bubbleColors;

public slots:
    void addColor(QColor newColor);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void newMixedColor(QColor color);
    void colorsChanged(const QMap<QString, int>& colors, int totalColors);
};

#endif // CAULDRON_H
