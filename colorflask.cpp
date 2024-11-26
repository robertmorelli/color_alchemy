#include "colorflask.h"
#include <QColor>
#include <QPainter>
#include <Box2D/Box2D.h>
#include <QMouseEvent>
#include <algorithm>
#include <QTimer>
#include <QPixmap>
#include <QVBoxLayout>
#include <QGraphicsItem>
#include <QGraphicsColorizeEffect>
#include <QDebug>
#include <MainWindow.h>

ColorFlask::ColorFlask(QWidget* parent, QColor colorHex, QPoint* position)
    : QWidget{ parent }, color(colorHex), position(position), world(b2Vec2(0.0f, 10.0f)), isDragging(false) {

    // Creates an overlay of the lineart and the actual color portion (TODO: Have the colorize effect only effect the color portion)


    setStyleSheet("border-image: none; image: url(:/images/potionColor.svg);");

    flask = new QLabel(this);
    flask->setAlignment(Qt::AlignCenter);

    QPixmap flaskPixmap(":/images/potion.png");
    flask->setPixmap(flaskPixmap);
    flask->setScaledContents(true);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(flask);
    setLayout(layout);

    flask->resize(this->size());

    // box2d setup
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 455.0f);
    groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(1000.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // to set position
    QTimer* setup = new QTimer(this);
    setup->setSingleShot(true);
    connect(setup, &QTimer::timeout, this, &ColorFlask::setUpFlask);
    setup->start(0);

    // timer ticks
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ColorFlask::simulatePhysics);
    timer->start(1);
}


void ColorFlask::setUpFlask(){
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

    body->CreateFixture(&fixtureDef);
    body->SetAwake(false);
}

void ColorFlask::changeColor(QColor color) {
    QGraphicsColorizeEffect* colorizeEffect = new QGraphicsColorizeEffect(this);
    colorizeEffect->setColor(color);
    this->setGraphicsEffect(colorizeEffect);
}


void ColorFlask::mouseMoveEvent(QMouseEvent* event) {
    if (isDragging) {
        QPoint newPos = mapToParent(event->pos()) - dragOffset;
        //clamp the position to the parent widget
        int x = std::min(
            std::max(newPos.x(), 0),
            parentWidget()->width() - width());
        int y = std::min(
            std::max(newPos.y(), 0),
            parentWidget()->height() - height());
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
        //little jump. remove if you dont like it lol
        body->SetLinearVelocity(b2Vec2(0, -30));
    }
}

void ColorFlask::simulatePhysics() {
    if (!isDragging) {
        world.Step(1.0f / 60.0f, 6, 2);
        b2Vec2 position = body->GetPosition();
        move(QPoint(position.x, position.y));
    }
}

void ColorFlask::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    //painter.drawImage();
}
