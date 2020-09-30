#define DEFAULT 0
#define ADDITIONAL_ROW 1
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QToolBar>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include "addtablewindow.h"
#include "addcolumnwindow.h"
#include "dataprocessor.h"
#include "selectwindow.h"
#include "myprinter.h"

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

    void on_button_addKolumn_clicked();

    void on_button_deleteColumn_clicked();

    void on_button_addVehicle_clicked();

    void on_button_deleteVehicle_clicked();

    void on_button_sort_clicked();

    void on_getSelectConditions();

    void on_actionDodaj_tabel_2_triggered();

    void on_actionUsun_tabel_triggered();

    void on_select_current_table();

    void on_actionRefresh_triggered();

    void on_actionDrukuj_triggered();



private:
    bool eventFilter(QObject *obj, QEvent *evt) override;
  //  int DEFAULT = 0,
   //     ADDITIONAL_ROW = 1;
    Ui::MainWindow *ui;
    DataProcessor *dataProcessor;
    SelectWindow *sw;
    //QPrinter printer;
    QToolBar *createToolBar();
    QString currentTable;
    QString lastSelectQuery = "";
    QStringList lastSelectConditions;
    void addTablesToMainMenu(QStringList tableNames);
    void on_print_prewiew_needs(QPrinter *);

};
#endif // MAINWINDOW_H
