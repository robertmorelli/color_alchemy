#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QColor>
#include <Box2D/Box2D.h>

///
/// \brief The Goal class stores data for one goal goal
///
struct Goal {
    QColor color;
    bool done = false;
};

///
/// \brief The Section class stores data for one level section
///
struct Section {
    Goal goals[4];
    QString instructions;
};

///
/// \brief The Level class stores data for one level
///
struct Level {
    QVector<Section> sections;
};

///
/// \brief The Game class the model for the application
///
class Game : public QObject {
    Q_OBJECT
public:
    explicit Game(QObject* parent = nullptr);

    ///
    /// \brief resetGoals resets the goals to all be not completed.
    /// don't fear the triple nested for loops, this runs in constant time
    ///
    void resetGoals();

    ///
    /// \brief update updates the goals and instructions
    ///
    void update();

private:
    b2Vec2 gravity;         /// gravity vector to simulate the flask physics
    b2World world;          /// world object for box2d
    int currentLevel;       /// the current level
    int currentSection;     /// the current section
    QVector<Level> levels;  /// the list of levels

    ///
    /// \brief checkNewLevel to see if a new level has been reached
    ///
    void checkNewLevel();

    ///
    /// \brief colorDistanceInOKLAB Compute the distance between two colors in the OKLAB color space
    /// \param target the target color
    /// \param current the current color
    /// \return the distance between the two colors (with some scaling)
    ///
    double colorDistanceInOKLAB(QColor target, QColor current);

signals:
    ///
    /// \brief newGoals signal to show the next goal color
    ///
    void newGoals(Goal[4]);

    ///
    /// \brief newInstructions signal to show the new instructions for the tutorial
    ///
    void newInstructions(QString&);

    ///
    /// \brief correctColor signal to indicate a correct color
    ///
    void correctColor(int);

    ///
    /// \brief cauldronEffect signal to show the cauldron effect
    ///
    void cauldronEffect();

    ///
    /// \brief newLevel signal to show a new level has been unlocked, with that level index as the parameter
    ///
    void newLevel(int);

    ///
    /// \brief lastSection signal for the previous section of the level
    ///
    void lastSection();

public slots:
    ///
    /// \brief checkGoal check if the color is the goal color
    /// \param color the color to check
    ///
    void checkGoal(QColor color);

    ///
    /// \brief setIndex for when the button is clicked in the ui
    /// \param idx which level the user wants to navigate to
    ///
    void setIndex(int idx);

    ///
    /// \brief setSection for when button is clicked in the ui, reset the section
    /// \param idx of the section to go to
    ///
    void setSection(int idx);

    ///
    /// \brief skipPressed for when the skip button is pressed, advance section
    ///
    void skipPressed();
};

#endif // GAME_H
