#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QProcess>
#include <QCloseEvent>

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    QPushButton *startButton;
    QPushButton *displayButton;
    QPushButton *averageButton;
    QPushButton *graduateButton;

    QLineEdit *searchStudentEdit;

    QTextEdit *displayArea;
    QLabel *messageLabel;

    QProcess *studentProcess;

    void addStudentData(QString studentNumber, QString moduleCode, int mark);
    void displayStudent(QString studentNumber);

    void saveStudentListToXML();
    void loadStudentListFromXML();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void startGetStudent();
    void readStudentOutput();
    void processFinished();

    void displayStudentRecord();
    void displayAverage();
    void checkGraduation();

public:
    MainWindow(QWidget *parent = 0);
};

#endif
