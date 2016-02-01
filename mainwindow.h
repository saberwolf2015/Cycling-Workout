#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QMenuBar;
class QLabel;
class QTimer;
class CreateExerciseDialog;
enum class Step {
    READY,
    STEADY,
    GO,
    WORK
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void startPressed();
    void timerTick();
    void loadExercise();
    void createExercise();
private:
    CreateExerciseDialog* createExerciseDialog;
    QVector<QString> exercises;
    int exPos;
    void updateSet();
    void updateTime();
    int timePerEx;
    int setCount;
    QTimer *timer;
    QMenuBar* createMainMenu(QWidget* wdg);//create main Menu
    QLabel* timerLabel;
    QLabel* setLabel;
    QLabel* remainLabel;
    QLabel* exersiseLabel;
    int elapsedTime;
    int remainedTime;
    int setPos;
    Step step;
};

#endif // MAINWINDOW_H
