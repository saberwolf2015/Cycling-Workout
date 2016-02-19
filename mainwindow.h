// Copyright (c) 2016 Tchernopyatov Alexey. Contacts: alexey@tchernopyatov.com
// Under MIT license, view LICENSE.txt

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
    /**
     * Структура чтобы хранить связь между меню и языками
     * @author Чернопятов А.В.
     * @date 2015.02.02
     */
    struct ActionLanguageStruct {
        QAction* action;
        LanguageStruct lang;
    };

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void startPressed();
    void timerTick();
    void loadExercise();
    void createExercise();
    void actionTriggered();
private:
    /**
     * Загружаем сет из файла
     * @param file путь к файлу
     * @return структура с сетом
     * @author Чернопятов А.В.
     * @date 2015.02.07
     */
    SetStruct loadExerciseData(const QString &file);
    /**
     * Загружаем язык из файла
     * @param file путь к файлу
     * @return структура с языком
     * @author Чернопятов А.В.
     * @date 2015.02.07
     */
    LanguageStruct loadLanguageData(const QString &file);
    /**
     * Обновляем везде перевод текстов
     * @author Чернопятов А.В.
     * @date 2015.02.07
     */
    void updateLanguage();
    /**
     * Записываем данные в файл
     * @param file путь к файлу
     * @param data данные для записи
     * @author Чернопятов А.В.
     * @date 2015.02.07
     */
    void writeDataToFile(const QString& file, const QByteArray&data);
    QVector<ActionLanguageStruct> languages; /*! array of translation */
    LanguageStruct currentLanguage;/*! current translation of program */
    ConfigStruct configStruct;/*! config */
    /**
     * Загружаем набор языков
     * @author Чернопятов А.В.
     * @date 2015.02.07
     */
    void loadLanguages();
    /**
     * Загружаем конфиг
     * @author Чернопятов А.В.
     * @date 2015.02.07
     */
    void loadConfig();
    SetStruct selectedSet; /*! selected training set */
    QVector<ActionSetStruct> ass; /*! all training sets */
    QMenu *fileMenu;/*! file menu*/
    QAction *createEx;/*! create exercise submenu */
    QAction *loadEx;/*! load exercise submenu */
    QAction *runEx;/*! run exercise submenu */
    QMenu *exMenu; /*! menu with exercises*/
    QMenu* langMenu; /*! menu with languages */
    QMenu *helpMenu; /*! help menu */
    QAction *helpTrigger;/*! help trigger submenu */
    QAction *helpAbout;/*! about submenu */
    /**
     * Обновляем пункт меню с сетами
     * @author Чернопятов А.В.
     * @date 2015.02.07
     */
    void scanDataAndUpdateMenu();
    CreateExerciseDialog* createExerciseDialog;
    int exPos;/*! position in exercise array*/
    /**
     * Обновляем текст с сетом
     * @author Чернопятов А.В.
     * @date 2015.02.07
     */
    void updateSet();
    /**
     * Обновляем текст со временем
     * @author Чернопятов А.В.
     * @date 2015.02.07
     */
    void updateTime();
    int timePerEx;/*! time per current exercise */
    int setCount;/*! count of set */
    QTimer *timer;
    QMenuBar* createMainMenu(QWidget* wdg);//create main Menu
    QLabel* timerLabel;/*! total time label */
    QLabel* setNameLabel; /*! name of set label */
    QLabel* setLabel; /*! label with from _ to _ set count */
    QLabel* remainLabel; /*! remained exercise time label */
    QLabel* exersiseLabel; /*! label with current exercise name */
    int elapsedTime;
    int remainedTime;
    int setPos; /*! current set number */
    Step step;  /*! current step */
};

#endif // MAINWINDOW_H
