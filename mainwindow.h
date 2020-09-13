#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addtablewindow.h"
#include "dataprocessor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void display(int);
private slots:
    void on_button_addTable_clicked();

private:
    int DEFAULT = 0,
        ADDITIONAL_ROW = 1;
    Ui::MainWindow *ui;
    DataProcessor dataProcessor;
};
#endif // MAINWINDOW_H
