#include "colorflask.h"
#include <Box2D/Box2D.h>
#include <QColor>
#include <QGraphicsColorizeEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QDebug>

ColorFlask::ColorFlask(QWidget* parent)
    : QWidget { parent }
    , world(b2Vec2(0.0f, 10.0f))
    , isDragging(false)
    , timer(this)
    , setupTimer(this) {

    // box2d setup
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 455.0f);
    groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(1000.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef leftShelfBodyDef;
    leftShelfBodyDef.position.Set(0.0f, 145.0f);
    leftShelfBody = world.CreateBody(&leftShelfBodyDef);

    b2PolygonShape leftShelfBox;
    leftShelfBox.SetAsBox(400.0f, 50.0f);
    leftShelfBody->CreateFixture(&leftShelfBox, 0.0f);

    b2BodyDef rightShelfBodyDef;
    rightShelfBodyDef.position.Set(975.0f, 150.0f);
    rightShelfBody = world.CreateBody(&rightShelfBodyDef);

    b2PolygonShape rightShelfBox;
    rightShelfBox.SetAsBox(450.0f, 50.0f);
    rightShelfBody->CreateFixture(&rightShelfBox, 0.0f);

    // to set position
    setupTimer.setSingleShot(true);
    connect(&setupTimer, &QTimer::timeout, this, &ColorFlask::setUpFlask);
    setupTimer.start(0);

    // timer ticks
    connect(&timer, &QTimer::timeout, this, &ColorFlask::simulatePhysics);
    timer.start(1);
}

void ColorFlask::setUpFlask() {

    this->initialPosition = QPoint(this->pos().x(), this->pos().y());
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(this->pos().x(), this->pos().y());
    body = world.CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(width() / 2.0f, height() / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    // make the flask bouncy. remove if you dont like it lol
    fixtureDef.restitution = 0.2f;

    body->CreateFixture(&fixtureDef);
    body->SetAwake(false);
}

void ColorFlask::changeColor(QColor color) {
    QGraphicsColorizeEffect* colorizeEffect = new QGraphicsColorizeEffect(this);
    colorizeEffect->setColor(color);
    this->setGraphicsEffect(colorizeEffect);
    this->color = color;
}

void ColorFlask::mouseMoveEvent(QMouseEvent* event) {
    if (isDragging) {
        QPoint newPos = mapToParent(event->pos()) - dragOffset;
        // clamp the position to the parent widget
        int x = std::min(std::max(newPos.x(), 0), parentWidget()->width() - width());
        int y = std::min(std::max(newPos.y(), 0), parentWidget()->height() - height());
        move(x, y);
    }
}

void ColorFlask::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        dragOffset = mapTo(this, event->pos());
        body->SetAwake(false);
    }
}

void ColorFlask::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        body->SetTransform(b2Vec2(x(), y()), body->GetAngle());
        body->SetAwake(true);
        // little jump. remove if you dont like it lol
        body->SetLinearVelocity(b2Vec2(0, -30));
    }
}

void ColorFlask::simulatePhysics() {
    if (!isDragging) {

        world.Step(1.0f / 60.0f, 6, 2);
        b2Vec2 position = body->GetPosition();
        if (position.x > 300 && position.y > 180 && position.x < 500) {
            // return to shelf
            body->SetAwake(false);
            body->SetTransform(b2Vec2(this->initialPosition.x(), this->initialPosition.y() - 200 /*falls from ceiling*/), 0);
            body->SetLinearVelocity(b2Vec2(0, 50));
            body->SetAwake(true);
            emit addToCauldron(this->color);
        } else{
            if((lastY < 405 && position.y > 405)){
                emit makeSound();
            }
            // qDebug() << lastY;
            // qDebug() << position.y;
            lastY = position.y;
            move(QPoint(position.x, position.y));
        }
    }
}

void ColorFlask::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    QPainter painter(this);
    QPixmap pixmap(":/images/potion.png");
    painter.drawPixmap(rect(), pixmap);
}
