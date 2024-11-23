#include "colorflask.h"
#include <QColor>
#include <QPainter>
#include <Box2D/Box2D.h>
#include<QMouseEvent>
#include <QTimer>

ColorFlask::ColorFlask(QWidget* parent, QColor colorHex, QPoint* position)
    : QWidget{ parent }, color(colorHex), position(position), world(b2Vec2(0.0f, 10.0f)), isDragging(false) {

    setAttribute(Qt::WA_Hover);

    //TODO: this does nothing currently cannot access the proper ui position
    QPoint initialPosition = this->pos();
    move(initialPosition);

    // box2d setup
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 500.0f);
    groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(1000.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(this->pos().x(), this->pos().y()); // TODO: fix, they all still start at 0,0 because this for some reason doesn't access the ui pos
    body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(width() / 2.0f, height() / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    body->SetAwake(false);

    // timer ticks
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ColorFlask::simulatePhysics);
    timer->start(16);
}

void ColorFlask::mouseMoveEvent(QMouseEvent* event) {
    if (isDragging) {
        move(event->globalPosition().toPoint() - QPoint(400, 120)); // current workaround for offset, TODO: fix!
    }
}

void ColorFlask::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        //offset = event->pos();
        body->SetAwake(false);
    }
}

void ColorFlask::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        body->SetTransform(b2Vec2(x(), y()), body->GetAngle());
        body->SetAwake(true);
    }
}

void ColorFlask::simulatePhysics() {
    if (!isDragging) {
        world.Step(1.0f / 60.0f, 6, 2);
        b2Vec2 position = body->GetPosition();
        move(static_cast<int>(position.x), static_cast<int>(position.y));
    }
}

void ColorFlask::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    //painter.drawImage();
}
