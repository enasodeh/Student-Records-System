#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QMap>

class Student
{
private:
    QString studentNumber;
    QMap<QString, int> modules;

public:
    Student();
    Student(QString number);

    void setNumber(QString number);
    QString getNumber() const;

    void addModule(QString moduleCode, int mark);
    QMap<QString, int> getModules() const;

    double average() const;
    bool graduate() const;
};

#endif
