#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QToolBar>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QTableWidget>
#include "addtablewindow.h"
#include "addcolumnwindow.h"
#include "dataprocessor.h"
#include "selectwindow.h"
#include "myprinter.h"
#include "ioprocessor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum DisplayParameters
{
    DEFAULT,
    ADDITIONAL_ROW
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void display(QSqlQuery, int);

    void insertQTableWidgetRow(QStringList colNames, QStringList colTypes);

    void updateQTableWidgetRow(QStringList colNames, QStringList colTypes);

    void initColumnsWidth(QStringList colNames);

    QComboBox *initComboBox(QString tableName, QString colName, QString currentText = nullptr);

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



    void on_pushButton_clicked();

private:
    bool eventFilter(QObject *obj, QEvent *evt) override;
    Ui::MainWindow *ui;
    DataProcessor *dataProcessor;
    SelectWindow *sw;
    QToolBar *createToolBar();
    QString currentTable;
    QString lastSelectQuery = "";
    QStringList lastSelectConditions;
    void addTablesToMainMenu(QStringList tableNames);
    void on_print_prewiew_needs(QPrinter *);

};
#endif // MAINWINDOW_H
