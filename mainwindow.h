#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Game.h>
#include <QMainWindow>
#include <QSoundEffect>
#include <colornamer.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

///
/// \brief The LabelInfo struct stores the text to be displayed and position to introduce the
/// player to the game.
///
struct LabelInfo {
    QString text;
    QPoint position;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    /// @brief ???
    void flasks();

public slots:
    /// @brief update the ui to show the goal color
    void updateGoal(QColor color);
    /// @brief update the ui to show the instructions
    void updateInstructions(QImage image);
    /// @brief update the ui to show a hint
    void newHint(QString);

    ///
    /// \brief walkThroughLabels a function to walk the user through the setup of the game, called
    /// on setup timer.
    ///
    void walkThroughLabels();

private:
    Ui::MainWindow* ui;
    Game game;
    ColorNamer colorNamer;
    QTimer* timer; // timer for label display
    int labelsIdx; // index for label display
    QVector<LabelInfo> labels; // stores label info
    /// @brief go back to the previous instruction
    void backInstruction();
    /// @brief go to the next instruction
    void nextInstruction();
    /// @brief skip all instructions
    void skipInstructions();
    /// @brief toggle the hint visibility
    void toggleHint();
    bool hintVisiblity;
    QSoundEffect glassSound;
    QSoundEffect boilingSound;

    /// @brief play the glass sound
    void playGlassSound();
};
#endif // MAINWINDOW_H
