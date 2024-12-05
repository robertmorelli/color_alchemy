#ifndef GAME_H
#define GAME_H
#include <Box2D/Box2D.h>
#include <QObject>
#include <colorflask.h>

struct Goal {
    QColor color;
    bool done = false;
};

struct Section {
    Goal goals[4];
    QString instructions;
};

struct Level {
    QVector<Section> sections;
};

class Game : public QObject {
    Q_OBJECT
public:
    explicit Game(QObject* parent = nullptr);

    void update();

private:
    b2Vec2 gravity;
    b2World world;
    int currentLevel;
    int currentSection;
    QVector<Level> levels;

    /// @brief get a textual hint of what is wrong with the color
    /// @param color the color to check
    /// @param targetColor the target color
    /// @return a hint of what is wrong with the color
    QString checkGoalHint(const QColor, const QColor);
    /// @brief Compute the distance between two colors in the OKLAB color space
    /// @param target the target color
    /// @param current the current color
    /// @return the distance between the two colors (with some scaling)
    double colorDistanceInOKLAB(QColor target, QColor current);

signals:
    /// @brief signal to show the next goal color
    void newGoals(Goal[4]);
    /// @brief signal to show the new instructions for the tutorial
    void newInstructions(QString&);
    /// @brief signal to show a hint about the current color and the goal color
    void newHint(QString);
    /// @brief signal to indicate a correct color
    void correctColor(int);

    /// @brief signal to show the cauldron effect
    void cauldronEffect();

    /// @brief signal to show a new level has been unlocked, with that level index as the parameter
    void newLevel(int);

public slots:
    /// @brief check if the color is the goal color
    void checkGoal(QColor color);
    ///
    /// \brief setIndex for when the button is clicked in the ui
    /// \param idx which level the user wants to navigate to
    ///
    void setIndex(int idx);

    ///
    /// \brief skipPressed for when the skip button is pressed, advance section
    ///
    void skipPressed();
};

#endif // GAME_H
