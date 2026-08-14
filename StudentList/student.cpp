#include "student.h"

Student::Student()
{
    studentNumber = "";
}

Student::Student(QString number)
{
    studentNumber = number;
}

void Student::setNumber(QString number)
{
    studentNumber = number;
}

QString Student::getNumber() const
{
    return studentNumber;
}

void Student::addModule(QString moduleCode, int mark)
{
    modules.insert(moduleCode, mark);
}

QMap<QString, int> Student::getModules() const
{
    return modules;
}

double Student::average() const
{
    if (modules.isEmpty())
    {
        return 0;
    }

    int total = 0;

    QMap<QString, int>::const_iterator it;

    for (it = modules.begin(); it != modules.end(); ++it)
    {
        total += it.value();
    }

    return (double)total / modules.size();
}

bool Student::graduate() const
{
    int passedModules = 0;
    int firstYearModules = 0;
    int thirdYearModules = 0;

    QMap<QString, int>::const_iterator it;

    for (it = modules.begin(); it != modules.end(); ++it)
    {
        QString moduleCode = it.key();
        int mark = it.value();

        if (mark >= 50)
        {
            passedModules++;

            /*
              Module format example: COS3711
              Character at index 3 is the year:
              COS3711 -> 3
              COS2611 -> 2
              COS1511 -> 1
            */
            if (moduleCode.length() >= 4)
            {
                QChar year = moduleCode.at(3);

                if (year == '1')
                {
                    firstYearModules++;
                }
                else if (year == '3')
                {
                    thirdYearModules++;
                }
            }
        }
    }

    /*
      Graduation condition:
      - passed 5 modules
      - at most 2 are first-year modules
      - at least 1 is a third-year module
    */
    if (passedModules >= 5 &&
        firstYearModules <= 2 &&
        thirdYearModules >= 1)
    {
        return true;
    }

    return false;
}
