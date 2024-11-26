#ifndef COLORFLASK_H
#define COLORFLASK_H

#include <QWidget>
#include <QColor>
#include <QPoint>
#include <Box2D/Box2D.h>
#include <QLabel>

class ColorFlask : public QWidget
{
    Q_OBJECT
private:
    QColor color;
    QPoint initialPosition;
    QPoint dragOffset;
    b2World world;
    b2Body* groundBody;
    b2Body* leftShelfBody;
    b2Body* rightShelfBody;
    b2Body* body;
    QLabel* background;
    QLabel* flask;
    bool isDragging;
    void setUpFlask();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

public:
    explicit ColorFlask(QWidget* parent = nullptr);
    void changeColor(QColor color);

private slots:
    void simulatePhysics();

signals:
    void addToCauldron(QColor color);

};

#endif // COLORFLASK_H
