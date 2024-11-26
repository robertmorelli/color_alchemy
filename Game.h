#ifndef GAME_H
#define GAME_H
#include <QObject>
#include <Box2D/Box2D.h>
#include <colorflask.h>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);

private:
    b2Vec2 gravity;
    b2World world;
    QVector<QColor> colorGoals;
    QVector<QImage> instructions; // TODO: implement
    int currentLevel;

signals:
    void newGoal(QColor);
    void newInstructions(QImage);

public slots:
    void checkGoal(QColor color);
};

#endif // GAME_H
