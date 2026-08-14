#include "studentlist.h"

StudentList* StudentList::instance = 0;

StudentList::StudentList()
{
}

StudentList* StudentList::getInstance()
{
    if (instance == 0)
    {
        instance = new StudentList();
    }

    return instance;
}

StudentList::~StudentList()
{
    clear();
}

void StudentList::addStudent(Student *student)
{
    students.append(student);
}

QList<Student*> StudentList::getStudents() const
{
    return students;
}

int StudentList::findStudent(QString studentNumber) const
{
    for (int i = 0; i < students.size(); i++)
    {
        if (students[i]->getNumber() == studentNumber)
        {
            return i;
        }
    }

    return -1;
}

Student* StudentList::getStudent(int index) const
{
    if (index < 0 || index >= students.size())
    {
        return 0;
    }

    return students[index];
}

int StudentList::size() const
{
    return students.size();
}

void StudentList::clear()
{
    for (int i = 0; i < students.size(); i++)
    {
        delete students[i];
    }

    students.clear();
}
