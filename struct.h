#ifndef STRUCT
#define STRUCT

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

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
        qDebug() << " name = " << item["name"].toString();
        qDebug() << " set_count = " << item["set_count"].toString();
        this->name = item["name"].toString();
        if(!item["name"].isUndefined()) {
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

