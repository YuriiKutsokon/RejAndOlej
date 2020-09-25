#define DEFAULT 0
#define ADDITIONAL_ROW 1
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "addtablewindow.h"
#include "addcolumnwindow.h"
#include "dataprocessor.h"
#include "selectwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void display(QSqlQuery, int);
    void insertQTableWidgetRow(QStringList colNames, QStringList colTypes);
    void updateQTableWidgetRow(QStringList colNames, QStringList colTypes);

private slots:
    void on_button_addTable_clicked();

    void on_button_addKolumn_clicked();

    void on_button_deleteColumn_clicked();

    void on_button_addVehicle_clicked();

    void on_button_deleteTable_clicked();

    void on_button_deleteVehicle_clicked();

    void on_button_sort_clicked();

    void on_getSelectConditions();

private:
    bool eventFilter(QObject *obj, QEvent *evt) override;
  //  int DEFAULT = 0,
   //     ADDITIONAL_ROW = 1;
    Ui::MainWindow *ui;
    DataProcessor *dataProcessor;
    SelectWindow *sw;
    QString currentTable;
    QStringList lastSelectConditions;
};
#endif // MAINWINDOW_H
