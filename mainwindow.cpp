#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataProcessor = DataProcessor("przeglady_rejestracyjne");
    display(DEFAULT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::display(int param)
{
    QString currentTable = dataProcessor.getCurrentTable();
    QSqlQuery query = dataProcessor.selectAll(currentTable);
    QStringList colNames = dataProcessor.getColNames(currentTable);
    int colCount = dataProcessor.getColNum(currentTable);
    int rowCount = dataProcessor.getRowNum(currentTable);
    int i=0;
    switch (param)
    {
        case 0:
        ui->main_Table->setColumnCount(colCount);
        ui->main_Table->setRowCount(rowCount);
        ui->main_Table->setHorizontalHeaderLabels(colNames);
        while (query.next())
        {

            for (int j=0; j<colCount; j++)
            {
               QTableWidgetItem *item = ui->main_Table->item(i, j);
               if (!item)
               {
                   item = new QTableWidgetItem();
                   ui->main_Table->setItem(i, j, item);
                   item -> setText(dataProcessor.getValue(currentTable, i, j));
               }
             }
            i++;
        }
           ui -> retranslateUi(this);

        break;

        case 1:

        break;

    }
}


void MainWindow::on_button_addTable_clicked()
{
     AddTableWindow atw;

     if (atw.exec())
         dataProcessor.addTable(atw.getNewTableName());

}
