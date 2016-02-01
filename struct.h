#ifndef STRUCT
#define STRUCT

#include <QString>

struct ExerciseStruct {
    QString name;
    int time;
    ExerciseStruct() {
        name = QString();
        time = 1;
    }
};

#endif // STRUCT

