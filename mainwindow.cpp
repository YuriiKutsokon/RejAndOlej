#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataProcessor = new DataProcessor();
    currentTable = dataProcessor->getCurrentTable();
    ui->label_tableName->setText(currentTable);
    //qDebug() << currentTable;
    ui->main_Table->installEventFilter(this);
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    //qDebug() << query.lastQuery();
    display(query, DEFAULT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::display(QSqlQuery query, int param)
{
    qDebug() << query.lastQuery();
    QStringList colNames = dataProcessor->getColNames(currentTable);
    int colCount = dataProcessor->getColNum(currentTable);
    ui->main_Table->setColumnCount(colCount);
    ui->main_Table->setHorizontalHeaderLabels(colNames);
    int i=0, row;
    switch (param)
    {
    case 0:
    {
        int rowCount = dataProcessor->getRowNum(query);
        ui->main_Table->setRowCount(rowCount);
        query.exec(query.lastQuery());
        while (query.next())
        {
            for (int j=0; j<colCount; j++)
            {
                QTableWidgetItem *item = ui->main_Table->item(i, j);
                if (!item)
                {
                    item = new QTableWidgetItem();
                    ui->main_Table->setItem(i, j, item);
                    row = query.value("id").toInt();
                    qDebug() << row;
                    item -> setText(dataProcessor->getValue(currentTable, query.value("id").toInt(), j));
                }
                else
                {
                    row = query.value("id").toInt();
                    qDebug() << row;
                    item -> setText(dataProcessor->getValue(currentTable, query.value("id").toInt(), j));

                }
            }
            i++;
        }
        ui -> retranslateUi(this);
    }

        break;

        case 1:
    {
        int rowCount = dataProcessor->getRowNum(query);
        query.exec(query.lastQuery());
        ui->main_Table->setRowCount(rowCount+1);
        QString str = "";
        str = str.setNum(rowCount);
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
                else if (i == rowCount)
                {
                    if (j == 0)
                    {
                        QTableWidgetItem *item = ui->main_Table->item(i, j);
                        if (!item)
                        {
                            item = new QTableWidgetItem();
                            ui->main_Table->setItem(i, j, item);
                            item -> setText(str);
                        }
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
    }

        break;

        case 2:
    {

    }
        break;

    }
}

void MainWindow::insertQTableWidgetRow(QStringList colNames, QStringList colTypes)
{
    QStringList insertValues;
    QString strInsertValues = "";

    for (int i = 0; i < colNames.size(); i++)
    {
        QTableWidgetItem *item = ui->main_Table->item(ui->main_Table->currentRow(), i);
        if(!item)
        {
            item = new QTableWidgetItem();
            ui->main_Table->setItem(ui->main_Table->currentRow(), i, item);
        }

//           qDebug() << item->text() << "  " << i;

        if (item->text() != "")
        {
            //qDebug() << "i'm in !isNull";
            insertValues.append(item->text());
        }
        else if (i == ui->main_Table->currentColumn())
        {
         insertValues.append(item->text());
                        //qDebug() << "i'm in currentColumn!";
        }
        else
            insertValues.append("0");
    }
    strInsertValues = dataProcessor->getValueParams(colTypes, insertValues);
    //qDebug() << colTypes << "\n" << insertValues;
    dataProcessor->insertRecord(currentTable, strInsertValues, colNames);

}

void MainWindow::updateQTableWidgetRow(QStringList colNames, QStringList colTypes)
{
    QStringList values;
    QStringList columnNames;
    QTableWidgetItem *item = ui->main_Table->currentItem();
    if(!item)
    {
        item = new QTableWidgetItem();
        ui->main_Table->setItem(ui->main_Table->currentRow(), ui->main_Table->currentColumn(), item);
    }

    if (colTypes[ui->main_Table->currentColumn()] == "int")
        values.append(item->text());
    else
        values.append("'" + item->text() + "'");

    columnNames.append(colNames[ui->main_Table->currentColumn()]);
    dataProcessor->updateRecord(currentTable, columnNames, values, ui->main_Table->currentRow());
}


void MainWindow::on_button_addTable_clicked()
{
     AddTableWindow atw;

     if (atw.exec())
         dataProcessor->addTable(atw.getNewTableName());
     currentTable = dataProcessor->getCurrentTable();
     ui->label_tableName->setText(currentTable);
     QSqlQuery query = dataProcessor->selectAll(currentTable);
    display(query, DEFAULT);
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
  //      qDebug() << colNames;
    //                qDebug() << parameters;
        if (parameters != "")
            dataProcessor->addColumn(currentTable, colNames, colTypes, parameters);
        else
        {
            dataProcessor->addColumn(currentTable, acw.getColName(), acw.getColType());
        }
    }

    QSqlQuery query = dataProcessor->selectAll(currentTable);
    display(query, DEFAULT);
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
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    display(query, DEFAULT);
}

void MainWindow::on_button_addVehicle_clicked()
{
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    display(query ,ADDITIONAL_ROW);
}

void MainWindow::on_button_deleteTable_clicked()
{
    dataProcessor->deleteTable(currentTable);
    currentTable = dataProcessor->getCurrentTable();
    ui->label_tableName->setText(currentTable);
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    display(query, DEFAULT);
}

void MainWindow::on_button_deleteVehicle_clicked()
{
    QTableWidgetItem *item = ui->main_Table->item(ui->main_Table->currentRow(), 0);
    //ui->main_Table->setItem(ui->main_Table->currentRow(), 0, item);
    dataProcessor->deleteRow(currentTable, item->text().toInt());
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    display(query, DEFAULT);
}

void MainWindow::on_button_sort_clicked()
{
    sw = new SelectWindow();
    QSignalMapper mapper;
    connect(sw->getOkButton(),SIGNAL(clicked()),this,SLOT(on_getSelectConditions()));
    sw->show();
}

void MainWindow::on_getSelectConditions()
{
    QSqlQuery query;
    QStringList conditionSplitter, names, values, signs;
    lastSelectConditions = sw->getSelectConditions();
    //qDebug() << lastSelectConditions;
    for(int i=0; i<lastSelectConditions.size(); i++)
    {
         conditionSplitter = lastSelectConditions[i].split(" ");
         names.append(conditionSplitter[0]);
         signs.append(conditionSplitter[1]);
         values.append(conditionSplitter[2]);
         conditionSplitter.clear();
    }

    qDebug() << names << signs << values;
    query = dataProcessor->selectExactRecords(currentTable, names, values, signs);
    display(query, DEFAULT);

}


bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
    bool returnStatement = false;
    QStringList colNames = dataProcessor->getColNames(currentTable);
    QStringList colTypes = dataProcessor->getColTypes(currentTable);
    QStringList selectColNames, selectColValues, comparitionSigns;
    QSqlQuery query;
    //int i = 0, j = 0;

    if (evt->type() == QEvent::KeyRelease)
    {
         QKeyEvent *ke = static_cast<QKeyEvent *>(evt);
         if (ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return)
         {
             QTableWidgetItem *item = ui->main_Table->item(ui->main_Table->currentRow(), 0);
             if(!item)
             {
                 item = new QTableWidgetItem();
                 ui->main_Table->setItem(ui->main_Table->currentRow(), 0, item);
             }

             selectColNames.append("id");
             selectColValues.append(item->text());
             comparitionSigns.append("=");
             query = dataProcessor->selectExactRecords(currentTable, selectColNames, selectColValues, comparitionSigns);

             if (query.next())
             {
                updateQTableWidgetRow(colNames, colTypes);
             }
             else
                insertQTableWidgetRow(colNames, colTypes);
             QSqlQuery query = dataProcessor->selectAll(currentTable);
             display(query, DEFAULT);
             returnStatement = true;
         }
             else if (ke->key() == Qt::Key_Left || ke->key() == Qt::Key_Right || ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Down)
         {    QSqlQuery query = dataProcessor->selectAll(currentTable);
              display(query, DEFAULT);
             //qDebug() << "display was pressed!";
             returnStatement = true;
         }
         else
         {
             //qDebug() << ke->key() << " was pressed!";
             returnStatement = false;
         }

    }
    return returnStatement;
}





