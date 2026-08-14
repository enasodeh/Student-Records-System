#ifndef STUDENTLIST_H
#define STUDENTLIST_H

#include <QList>
#include "student.h"

class StudentList
{
private:
    QList<Student*> students;

    static StudentList *instance;

    StudentList();

public:
    static StudentList* getInstance();

    ~StudentList();

    void addStudent(Student *student);
    QList<Student*> getStudents() const;

    int findStudent(QString studentNumber) const;
    Student* getStudent(int index) const;
    int size() const;

    void clear();
};

#endif
