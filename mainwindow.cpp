#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataProcessor = new DataProcessor("przeglady_rejestracyjne");
    currentTable = dataProcessor->getCurrentTable();
    display(DEFAULT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::display(int param)
{
    QStringList colNames = dataProcessor->getColNames(currentTable);
    int colCount = dataProcessor->getColNum(currentTable);
    int rowCount = dataProcessor->getRowNum(currentTable);
    ui->main_Table->setColumnCount(colCount);
    ui->main_Table->setHorizontalHeaderLabels(colNames);
    int i=0;
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    switch (param)
    {
        case 0:
        ui->main_Table->setRowCount(rowCount);
            while (query.next())
        {

            for (int j=0; j<colCount; j++)
            {
               QTableWidgetItem *item = ui->main_Table->item(i, j);
               if (!item)
               {
                   item = new QTableWidgetItem();
                   ui->main_Table->setItem(i, j, item);
                   item -> setText(dataProcessor->getValue(currentTable, i, j));
               }
             }
            i++;
        }
           ui -> retranslateUi(this);

        break;

        case 1:
        ui->main_Table->setRowCount(rowCount+1);
        bool endOfLoop = false;
        i = 0;
        while (!endOfLoop)
        {
           query.next();
            for (int j=0; j<colCount; j++)
            {
                if (i<rowCount)
               {
                    QTableWidgetItem *item = ui->main_Table->item(i, j);
                    if (!item)
                    {
                        item = new QTableWidgetItem();
                        ui->main_Table->setItem(i, j, item);
                        item -> setText(dataProcessor->getValue(currentTable, i, j));
                    }
                }

                else if (i > rowCount)
                        endOfLoop = true;

                else
                {
                    QTableWidgetItem *item = ui->main_Table->item(i, j);
                    if (!item)
                    {
                        item = new QTableWidgetItem();
                    }
                 }
             }
           i++;
        }
           ui -> retranslateUi(this);

        break;

    }
}


void MainWindow::on_button_addTable_clicked()
{
     AddTableWindow atw;

     if (atw.exec())
         dataProcessor->addTable(atw.getNewTableName());

}

void MainWindow::on_button_addKolumn_clicked()
{
    QString parameters, strColumns;
    QStringList colNames = dataProcessor->getColNames(currentTable),
            colTypes = dataProcessor->getColTypes(currentTable);
    AddColumnWindow acw;

    if (acw.exec())
    {
        parameters = acw.getUniqueStatus() + acw.getPrimaryKeyStatus();
        colNames.append(acw.getColName());
        colTypes.append(acw.getColType());
        qDebug() << colNames;
                    qDebug() << parameters;
        if (parameters != "")
            dataProcessor->addColumn(currentTable, colNames, colTypes, parameters);
        else
        {
            dataProcessor->addColumn(currentTable, acw.getColName(), acw.getColType());
        }
    }


    display(DEFAULT);
}

void MainWindow::on_button_deleteColumn_clicked()
{
    int colNum = dataProcessor->getColNum(currentTable);
    QStringList columns = dataProcessor->getColNames(currentTable);
    QStringList types = dataProcessor->getColTypes(currentTable);
    QString strColNames = "", strcoltypes = "";
    for (int i = 0; i < colNum; i++)
    {
        if (i != ui->main_Table->currentColumn())
        {
            strColNames += columns[i] + " " + types[i] + ", ";
        }
    }

    strColNames = strColNames.left(strColNames.lastIndexOf(','));
    qDebug() << strColNames;

    dataProcessor->deleteColumn(currentTable, strColNames);

    display(DEFAULT);
}

void MainWindow::on_button_addVehicle_clicked()
{
    display(ADDITIONAL_ROW);
}
