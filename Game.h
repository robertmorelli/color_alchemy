#ifndef GAME_H
#define GAME_H
#include <Box2D/Box2D.h>
#include <QObject>
#include <colorflask.h>

class Game : public QObject {
    Q_OBJECT
public:
    explicit Game(QObject* parent = nullptr);

private:
    b2Vec2 gravity;
    b2World world;
    QVector<QColor> colorGoals;
    QVector<QImage> instructions; // TODO: implement
    int currentLevel;
    /// @brief get a textual hint of what is wrong with the color
    /// @param color the color to check
    /// @param targetColor the target color
    /// @return a hint of what is wrong with the color
    QString checkGoalHint(const QColor, const QColor);

signals:
    /// @brief signal to show the next goal color
    void newGoal(QColor);
    /// @brief signal to show the new instructions for the tutorial
    void newInstructions(QImage);
    /// @brief signal to show a hint about the current color and the goal color
    void newHint(QString);

public slots:
    /// @brief check if the color is the goal color
    void checkGoal(QColor color);
};

#endif // GAME_H
