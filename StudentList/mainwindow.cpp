#include "mainwindow.h"
#include "student.h"
#include "studentlist.h"

#include <QGridLayout>
#include <QCoreApplication>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Student Records - Question 4");

    startButton = new QPushButton("Add Student Data");
    displayButton = new QPushButton("Display Student");
    averageButton = new QPushButton("Show Average");
    graduateButton = new QPushButton("Check Graduation");

    searchStudentEdit = new QLineEdit;
    searchStudentEdit->setPlaceholderText("Enter student number");

    displayArea = new QTextEdit;
    displayArea->setReadOnly(true);

    messageLabel = new QLabel("Click Add Student Data to start GetStudent.");

    studentProcess = new QProcess(this);

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(messageLabel, 0, 0, 1, 3);

    layout->addWidget(startButton, 1, 0, 1, 3);

    layout->addWidget(new QLabel("Student Number:"), 2, 0);
    layout->addWidget(searchStudentEdit, 2, 1, 1, 2);

    layout->addWidget(displayButton, 3, 0);
    layout->addWidget(averageButton, 3, 1);
    layout->addWidget(graduateButton, 3, 2);

    layout->addWidget(displayArea, 4, 0, 1, 3);

    setLayout(layout);

    connect(startButton, SIGNAL(clicked()), this, SLOT(startGetStudent()));

    connect(studentProcess, SIGNAL(readyReadStandardOutput()),
            this, SLOT(readStudentOutput()));

    connect(studentProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processFinished()));

    connect(displayButton, SIGNAL(clicked()), this, SLOT(displayStudentRecord()));
    connect(averageButton, SIGNAL(clicked()), this, SLOT(displayAverage()));
    connect(graduateButton, SIGNAL(clicked()), this, SLOT(checkGraduation()));

    loadStudentListFromXML();
}

void MainWindow::startGetStudent()
{
    QString programPath = QCoreApplication::applicationDirPath() + "/GetStudent.exe";

    studentProcess->start(programPath);

    if (!studentProcess->waitForStarted(3000))
    {
        QMessageBox::warning(this,
                             "Process Error",
                             "Could not start GetStudent.exe.\n\nMake sure GetStudent.exe is in this program's debug folder.");
        return;
    }

    messageLabel->setText("GetStudent started. Enter student details there.");
}

void MainWindow::readStudentOutput()
{
    QByteArray data = studentProcess->readAllStandardOutput();
    QString output = QString(data).trimmed();

    if (output.isEmpty())
    {
        return;
    }

    QStringList parts = output.split(",");

    if (parts.size() == 3)
    {
        QString studentNumber = parts[0].trimmed();
        QString moduleCode = parts[1].trimmed();
        int mark = parts[2].trimmed().toInt();

        addStudentData(studentNumber, moduleCode, mark);

        displayArea->append("Added:");
        displayArea->append("Student Number: " + studentNumber);
        displayArea->append("Module Code: " + moduleCode);
        displayArea->append("Mark: " + QString::number(mark));
        displayArea->append("----------------------------");
    }
    else
    {
        displayArea->append("Invalid data received from GetStudent:");
        displayArea->append(output);
    }
}

void MainWindow::processFinished()
{
    messageLabel->setText("GetStudent process finished.");
}

void MainWindow::addStudentData(QString studentNumber, QString moduleCode, int mark)
{
    StudentList *list = StudentList::getInstance();

    int index = list->findStudent(studentNumber);

    if (index == -1)
    {
        Student *newStudent = new Student(studentNumber);
        newStudent->addModule(moduleCode, mark);
        list->addStudent(newStudent);
    }
    else
    {
        Student *student = list->getStudent(index);

        if (student != 0)
        {
            student->addModule(moduleCode, mark);
        }
    }
}

void MainWindow::displayStudent(QString studentNumber)
{
    StudentList *list = StudentList::getInstance();

    int index = list->findStudent(studentNumber);

    if (index == -1)
    {
        displayArea->append("Student " + studentNumber + " was not found.");
        displayArea->append("----------------------------");
        return;
    }

    Student *student = list->getStudent(index);

    if (student == 0)
    {
        displayArea->append("Student could not be accessed.");
        displayArea->append("----------------------------");
        return;
    }

    displayArea->append("Student Number: " + student->getNumber());

    QMap<QString, int> modules = student->getModules();
    QMap<QString, int>::const_iterator it;

    for (it = modules.begin(); it != modules.end(); ++it)
    {
        displayArea->append(it.key() + " : " + QString::number(it.value()));
    }

    displayArea->append("----------------------------");
}

void MainWindow::displayStudentRecord()
{
    QString studentNumber = searchStudentEdit->text().trimmed();

    if (studentNumber.isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "Enter a student number first.");
        return;
    }

    displayStudent(studentNumber);
}

void MainWindow::displayAverage()
{
    QString studentNumber = searchStudentEdit->text().trimmed();

    if (studentNumber.isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "Enter a student number first.");
        return;
    }

    StudentList *list = StudentList::getInstance();

    int index = list->findStudent(studentNumber);

    if (index == -1)
    {
        displayArea->append("Student " + studentNumber + " was not found.");
        displayArea->append("----------------------------");
        return;
    }

    Student *student = list->getStudent(index);

    if (student != 0)
    {
        displayArea->append("Average for " + studentNumber + ": " +
                            QString::number(student->average(), 'f', 2));
        displayArea->append("----------------------------");
    }
}

void MainWindow::checkGraduation()
{
    QString studentNumber = searchStudentEdit->text().trimmed();

    if (studentNumber.isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "Enter a student number first.");
        return;
    }

    StudentList *list = StudentList::getInstance();

    int index = list->findStudent(studentNumber);

    if (index == -1)
    {
        displayArea->append("Student " + studentNumber + " was not found.");
        displayArea->append("----------------------------");
        return;
    }

    Student *student = list->getStudent(index);

    if (student != 0)
    {
        if (student->graduate())
        {
            displayArea->append("Student " + studentNumber + " qualfies for graduation.");
        }
        else
        {
            displayArea->append("Student " + studentNumber + " does NOT qualify for graduation.");
        }

        displayArea->append("----------------------------");
    }
}

void MainWindow::saveStudentListToXML()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/students.xml";

    QDomDocument document;

    QDomElement root = document.createElement("StudentList");
    document.appendChild(root);

    StudentList *list = StudentList::getInstance();

    QList<Student*> students = list->getStudents();

    for (int i = 0; i < students.size(); i++)
    {
        Student *student = students[i];

        QDomElement studentElement = document.createElement("student");
        root.appendChild(studentElement);

        QDomElement numberElement = document.createElement("number");
        QDomText numberText = document.createTextNode(student->getNumber());
        numberElement.appendChild(numberText);
        studentElement.appendChild(numberElement);

        QDomElement modulesElement = document.createElement("modules");
        studentElement.appendChild(modulesElement);

        QMap<QString, int> modules = student->getModules();
        QMap<QString, int>::const_iterator it;

        for (it = modules.begin(); it != modules.end(); ++it)
        {
            QDomElement moduleElement = document.createElement("module");
            modulesElement.appendChild(moduleElement);

            QDomElement codeElement = document.createElement("code");
            QDomText codeText = document.createTextNode(it.key());
            codeElement.appendChild(codeText);
            moduleElement.appendChild(codeElement);

            QDomElement markElement = document.createElement("mark");
            QDomText markText = document.createTextNode(QString::number(it.value()));
            markElement.appendChild(markText);
            moduleElement.appendChild(markElement);
        }
    }

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::warning(this, "File Error", "Could not write students.xml.");
        return;
    }

    QTextStream out(&file);
    document.save(out, 4);

    file.close();
}

void MainWindow::loadStudentListFromXML()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/students.xml";

    QFile file(filePath);

    if (!file.exists())
    {
        messageLabel->setText("No existing students.xml file. Starting with an empty list.");
        return;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "File Error", "Could not open students.xml.");
        return;
    }

    QDomDocument document;

    if (!document.setContent(&file))
    {
        QMessageBox::warning(this, "XML Error", "Could not read students.xml.");
        file.close();
        return;
    }

    file.close();

    QDomElement root = document.documentElement();

    if (root.tagName() != "StudentList")
    {
        QMessageBox::warning(this,
                             "XML Error",
                             "Invalid XML file. Root must be StudentList.");
        return;
    }

    StudentList *list = StudentList::getInstance();
    list->clear();

    QDomNode studentNode = root.firstChild();

    while (!studentNode.isNull())
    {
        QDomElement studentElement = studentNode.toElement();

        if (!studentElement.isNull() && studentElement.tagName() == "student")
        {
            QString number = studentElement.firstChildElement("number").text();

            Student *student = new Student(number);

            QDomElement modulesElement = studentElement.firstChildElement("modules");

            QDomNode moduleNode = modulesElement.firstChild();

            while (!moduleNode.isNull())
            {
                QDomElement moduleElement = moduleNode.toElement();

                if (!moduleElement.isNull() && moduleElement.tagName() == "module")
                {
                    QString code = moduleElement.firstChildElement("code").text();
                    int mark = moduleElement.firstChildElement("mark").text().toInt();

                    student->addModule(code, mark);
                }

                moduleNode = moduleNode.nextSibling();
            }

            list->addStudent(student);
        }

        studentNode = studentNode.nextSibling();
    }

    messageLabel->setText("Loaded " + QString::number(list->size()) +
                          " student(s) from students.xml.");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveStudentListToXML();
    event->accept();
}
