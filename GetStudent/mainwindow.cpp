#include "mainwindow.h"

#include "mainwindow.h"

#include <QTextStream>
#include <QCoreApplication>
#include <QGridLayout>
#include <QRegExp>
#include <QRegExpValidator>
#include <QIntValidator>

// Constructor: creates the GUI
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QLabel *studentNumberLabel = new QLabel("Student Number:");
    QLabel *moduleCodeLabel = new QLabel("Module Code:");
    QLabel *markLabel = new QLabel("Mark:");

    studentNumberEdit = new QLineEdit;
    moduleCodeEdit = new QLineEdit;
    markEdit = new QLineEdit;

    submitButton = new QPushButton("Submit");
    messageLabel = new QLabel;

    /*
       Input masks:
       Student number must be 4 digits.
       Module code format:
       3 uppercase letters + 1/2/3 + 2 digits + 1 final alphanumeric character.
       Example: COS3711
    */

    studentNumberEdit->setInputMask("0000");

    /*
       > means convert letters to uppercase.
       AAA means 3 alphabetic characters.
       900 means 3 digit positions.
       N means final alphanumeric character.
    */
    moduleCodeEdit->setInputMask(">AAA900N");

    /*
       The mark must be between 0 and 100.
       We use QIntValidator here and also check again manually.
    */
    markEdit->setValidator(new QIntValidator(0, 100, this));

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(studentNumberLabel, 0, 0);
    layout->addWidget(studentNumberEdit, 0, 1);

    layout->addWidget(moduleCodeLabel, 1, 0);
    layout->addWidget(moduleCodeEdit, 1, 1);

    layout->addWidget(markLabel, 2, 0);
    layout->addWidget(markEdit, 2, 1);

    layout->addWidget(submitButton, 3, 1);

    layout->addWidget(messageLabel, 4, 0, 1, 2);

    setLayout(layout);

    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitStudentData()));
}

// Checks that the student number is exactly 4 digits
bool MainWindow::validateStudentNumber(QString studentNumber)
{
    QRegExp regex("^[0-9]{4}$");
    return regex.exactMatch(studentNumber);
}

// Checks the module code
// Format: 3 uppercase letters, then 1 or 2 or 3, then 2 digits, then one letter/digit
// Example: COS3711
bool MainWindow::validateModuleCode(QString moduleCode)
{
    QRegExp regex("^[A-Z]{3}[123][0-9]{2}[A-Za-z0-9]$");
    return regex.exactMatch(moduleCode);
}

// Checks that mark is an integer from 0 to 100
bool MainWindow::validateMark(QString mark)
{
    QRegExp regex("^([0-9]|[1-9][0-9]|100)$");
    return regex.exactMatch(mark);
}

// Runs when Submit button is clicked
void MainWindow::submitStudentData()
{
    QString studentNumber = studentNumberEdit->text();
    QString moduleCode = moduleCodeEdit->text();
    QString mark = markEdit->text();

    if (!validateStudentNumber(studentNumber))
    {
        messageLabel->setText("Error: Student number must be a 4-digit number.");
        return;
    }

    if (!validateModuleCode(moduleCode))
    {
        messageLabel->setText("Error: Module code format is invalid. Example: COS3711");
        return;
    }

    if (!validateMark(mark))
    {
        messageLabel->setText("Error: Mark must be an integer between 0 and 100.");
        return;
    }

    messageLabel->setText("Student data accepted. Check Application Output.");

    QTextStream out(stdout);

    out << studentNumber << "," << moduleCode << "," << mark << endl;
    out.flush();

    QCoreApplication::quit();
}
