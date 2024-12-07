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

public slots:
    ///
    /// \brief updateGoals update the ui to show the goal color
    /// \param goals
    ///
    void updateGoals(Goal goals[4]);

    ///
    /// \brief updateInstructions update the ui to show the instructions
    /// \param imagePath the image to update them to
    ///
    void updateInstructions(QString& imagePath);

    void updateStageName(QString name);

    ///
    /// \brief showCheckmark enable the cooresponding checkmark label in the ui
    /// \param idx the index of the checkmark
    ///
    void showCheckmark(int idx);

    ///
    /// \brief walkThroughLabels a function to walk the user through the setup of the game, called
    /// on setup timer.
    ///
    void walkThroughLabels();

    void changeLevel(int);

private:
    Ui::MainWindow* ui;         /// the ui file
    Game game;                  /// the model
    ColorNamer colorNamer;      /// colorNamer to generate names for the colors
    QSoundEffect glassSound;    /// sound effect for the flasks
    QSoundEffect boilingSound;  /// sound effect for the cauldron
    QTimer* timer;              /// timer for label display
    QVector<LabelInfo> labels;  /// stores label info
    int labelsIdx;              /// index for label display

    ///
    /// \brief backInstruction go back to the previous instruction
    ///
    void backInstruction();

    ///
    /// \brief nextInstruction go to the next instruction
    ///
    void nextInstruction();

    ///
    /// \brief skipInstructions skip all instructions
    ///
    void skipInstructions();

    ///
    /// \brief playGlassSound play the glass sound
    ///
    void playGlassSound();
};
#endif // MAINWINDOW_H
