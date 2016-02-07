#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "struct.h"

class QMenuBar;
class QLabel;
class QTimer;
class CreateExerciseDialog;
class QMenu;


class MainWindow : public QMainWindow
{
    Q_OBJECT
    /**
     * Структура чтобы хранить связь между меню и сетами
     * @author Чернопятов А.В.
     * @date 2015.02.02
     */
    struct ActionSetStruct {
        QAction* action;
        SetStruct set;
    };

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void startPressed();
    void timerTick();
    void loadExercise();
    void createExercise();
    SetStruct loadExerciseData(const QString &file);
    void actionTriggered();
private:
    QVector<LanguageStruct> languages; /*! array of translation */
    LanguageStruct currentLanguage;/*! current translation of program */
    ConfigStruct configStruct;/*! config */
    void loadLanguages();
    void loadConfig();
    SetStruct selectedSet; /*! selected training set */
    QVector<ActionSetStruct> ass; /*! all training sets */
    QMenu *exMenu; /*! menu with exercices*/
    QMenu* langMenu; /*! menu with languages */
    void scanDataAndUpdateMenu();
    CreateExerciseDialog* createExerciseDialog;
    int exPos;
    void updateSet();
    void updateTime();
    int timePerEx;
    int setCount;
    QTimer *timer;
    QMenuBar* createMainMenu(QWidget* wdg);//create main Menu
    QLabel* timerLabel;
    QLabel* setNameLabel;
    QLabel* setLabel;
    QLabel* remainLabel;
    QLabel* exersiseLabel;
    int elapsedTime;
    int remainedTime;
    int setPos;
    Step step;
};

#endif // MAINWINDOW_H
