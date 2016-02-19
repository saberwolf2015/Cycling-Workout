// Copyright (c) 2016 Tchernopyatov Alexey. Contacts: alexey@tchernopyatov.com
// Under MIT license, view LICENSE.txt

#ifndef STRUCT
#define STRUCT

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMap>

/**
 * Структура для хранения языка, this is my first try
 * make a multilanguages program, so try to improvise
 * @param shortEnName краткое название на английском
 * @param fullName название в оригинале
 * @param words список слов в программе
 * @author Чернопятов А.В.
 * @date 2015.02.07
 */
struct LanguageStruct {
    QString shorEnName;
    QString fullName;
    QMap<QString, QString> words;
    LanguageStruct() {
        qDebug() << __func__;
        this->shorEnName = QString::fromUtf8("ru");
        this->fullName = QString::fromUtf8("русский");
        //menus
        words["MENU_FILE"] = QString::fromUtf8("Файл");
        words["MENU_CREATE_EXERCISE"] = QString::fromUtf8("Создать упраженение");
        words["MENU_LOAD_EXERCISE"] = QString::fromUtf8("Загрузка упраженения");
        words["MENU_RUN_EXERCISE"] = QString::fromUtf8("Запуск");
        words["MENU_EXERCISE"] = QString::fromUtf8("Упражнения");
        words["MENU_LANGUAGE"] = QString::fromUtf8("Язык");
        words["MENU_HELP"] = QString::fromUtf8("Справка");
        words["MENU_HELP_TRIGGER"] = QString::fromUtf8("Вызов справки");
        words["MENU_HELP_ABOUT"] = QString::fromUtf8("О программе");
        //steps
        words["STEP_READY"] = QString::fromUtf8("Настарт");
        words["STEP_STEADY"] = QString::fromUtf8("Внимание");
        words["STEP_GO"] = QString::fromUtf8("Марш");
        words["STEP_FINISH"] = QString::fromUtf8("ГОТОВО");
        //main window labels
        words["MW_NAME_LABEL"] = QString::fromUtf8("Выберите упражнение");
        words["MW_EXERCISE_LABEL"] = QString::fromUtf8("Ожидание");
        words["MW_TITLE"] = QString::fromUtf8("Тренировки");
        words["MW_SET_LABEL_SET"] = QString::fromUtf8("Сет ");
        words["MW_SET_LABEL_FROM"] = QString::fromUtf8(" из ");
        //file dialogs
        words["DLG_OPEN_EXERCISE"] = QString::fromUtf8("Открыть упражнение");
        words["DLG_SAVE_EXERCISE"] = QString::fromUtf8("Сохранить упражнение");
        //dialgo
        words["CREATE_EXERCISE_DLG_CYCLE_NAME"] = QString::fromUtf8("Название цикла");
        words["CREATE_EXERCISE_DLG_SET_COUNT"] = QString::fromUtf8("Число сетов: ");
        words["CREATE_EXERCISE_DLG_ADD_EXERCISE"] = QString::fromUtf8("Добавить упражнение");
        words["CREATE_EXERCISE_DLG_EXERCISE"] = QString::fromUtf8("Упраженение");
        //widget
        words["EXERCISE_WDG_EXERCISE"] = QString::fromUtf8("Упраженение");
        words["EXERCISE_WDG_EXERCISE_NAME"] = QString::fromUtf8("Название: ");
        words["EXERCISE_WDG_DEFAULT_EXERCISE_NAME"] = QString::fromUtf8("упраженение");
        words["EXERCISE_WDG_EXERCISE_TIME_PREFIX"] = QString::fromUtf8("Время упражнения: ");
        words["EXERCISE_WDG_EXERCISE_TIME_SUFFIX"] = QString::fromUtf8(" c");
        //btn
        words["BTN_OK"] = QString::fromUtf8("OK");
        words["BTN_CANCEL"] = QString::fromUtf8("Отмена");
        words["BTN_DELETE"] = QString::fromUtf8("Удалить");
    }
    LanguageStruct(QJsonDocument &doc) : LanguageStruct() {
        qDebug() <<" wtf";
        QJsonObject item = doc.object();
        if(item.contains("shorEnName")) {
            this->shorEnName = item["shorEnName"].toString();
        } else {
            qDebug() << "shorEnName undefined";
        }
        if(item.contains("fullName")) {
            this->fullName = item["fullName"].toString();
        } else {
            qDebug() << "fullName undefined";
        }
        QMap<QString, QString>::const_iterator iter = this->words.constBegin();
        while (iter != this->words.constEnd()) {
            if(item.contains(iter.key())) {
                words[iter.key()] = item[iter.key()].toString();
            }
            iter++;
        }
    }
};

/**
 * Структура для хранения конфига программы
 * @author Чернопятов А.В.
 * @date 2015.02.07
 */
struct ConfigStruct {
    QString lang;
    ConfigStruct() {
        this->lang = QString();
    }
    ConfigStruct(QJsonDocument &doc) {
        QJsonObject item = doc.object();
        qDebug() << " lang = " << item["lang"].toString();
        this->lang = item["lang"].toString();
    }
    QByteArray toJson() {
        QJsonDocument doc;
        QVariantMap map;
        map.insert("lang", this->lang);
        QJsonObject json = QJsonObject::fromVariantMap(map);
        doc.setObject(json);
        return doc.toJson();
    }
};

/**
 * Струтура упражнения, имя упражнения, сколько секунд выполнять
 * @author Чернопятов А.В.
 * @date 2015.02.01
 */
struct ExerciseStruct {
    QString name;
    int time;
    ExerciseStruct() {
        name = QString();
        time = 1;
    }
};
/**
 * Струтура сета имя, сета, число сетов, вектор упражнений
 * @author Чернопятов А.В.
 * @date 2015.02.03
 */
struct SetStruct {
    QString name;
    int count;
    QVector<ExerciseStruct> exercise;
    SetStruct() {
        name = QString();
        count = 1;
    }
    SetStruct(QJsonDocument &doc) {
        QJsonObject item = doc.object();
        this->name = item["name"].toString();
        if(item.contains("name")) {
            this->count = item["set_count"].toInt();
        } else {
            qDebug() << "undefined";
        }
        QJsonArray arr = item["exercises"].toArray();
        for(int i = 0; i < arr.size(); i++) {
            qDebug() << "[" << i << "]" << arr[i].toObject()["name"].toString() << " time = " << arr[i].toObject()["time"].toInt();
            ExerciseStruct es;
            es.name = arr[i].toObject()["name"].toString();
            es.time = arr[i].toObject()["time"].toInt();
            this->exercise.push_back(es);
        }
    }
};
/**
 * Этапы трениовки - настарт, внимание, марш - тренировка
 * @author Чернопятов А.В.
 * @date 2015.02.01
 */
enum class Step {
    READY,
    STEADY,
    GO,
    WORK
};

#endif // STRUCT

