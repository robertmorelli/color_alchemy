#ifndef COLORFLASK_H
#define COLORFLASK_H

#include <Box2D/Box2D.h>
#include <QColor>
#include <QLabel>
#include <QPoint>
#include <QTimer>
#include <QWidget>

class ColorFlask : public QWidget {
    Q_OBJECT
private:
    QColor color; // color of the flask
    QPoint initialPosition; // location of the flask
    float lastY;
    QPoint dragOffset; // offset relative to the pointer to keep it under the cursor
    b2World world; // box 2d world body
    b2Body* groundBody; // box 2d body for the floor
    b2Body* leftShelfBody; // box 2d body of the left shelf
    b2Body* rightShelfBody; // box 2d body of the right shelf
    b2Body* body; // box 2d body for the flask
    QLabel* background;
    QLabel* flask;
    bool isDragging; // true if the flask is being dragged, false otherwise
    ///
    /// \brief setUpFlask function to render the flask after the ui is rendered
    ///
    void setUpFlask();
    QTimer timer; // triggers the setUpFlask function after the game loads in
    QTimer setupTimer;

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
    void makeSound();
};

#endif // COLORFLASK_H
