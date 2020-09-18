#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "addtablewindow.h"
#include "addcolumnwindow.h"
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

    void on_button_addKolumn_clicked();

    void on_button_deleteColumn_clicked();

    void on_button_addVehicle_clicked();

    void on_button_deleteTable_clicked();

private:
    bool eventFilter(QObject *obj, QEvent *evt) override;
    int DEFAULT = 0,
        ADDITIONAL_ROW = 1;
    Ui::MainWindow *ui;
    DataProcessor *dataProcessor;
    QString currentTable;
};
#endif // MAINWINDOW_H
