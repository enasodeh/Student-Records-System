#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    QLineEdit *studentNumberEdit;
    QLineEdit *moduleCodeEdit;
    QLineEdit *markEdit;

    QPushButton *submitButton;
    QLabel *messageLabel;

    bool validateStudentNumber(QString studentNumber);
    bool validateModuleCode(QString moduleCode);
    bool validateMark(QString mark);

private slots:
    void submitStudentData();

public:
    MainWindow(QWidget *parent = 0);
};

#endif
