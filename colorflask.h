#ifndef COLORFLASK_H
#define COLORFLASK_H

#include <QWidget>
#include <QColor>
#include <QPoint>
#include <box2d/box2d.h>

class ColorFlask : public QWidget
{
private:
    QColor color;
    QPoint* position;
    b2World world;
    b2Body *groundBody;
    b2Body *body;
    bool isDragging;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    explicit ColorFlask(QWidget *parent = nullptr, QColor color = nullptr, QPoint* position = nullptr);

private slots:
    void simulatePhysics();

signals:
};

#endif // COLORFLASK_H
