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
    ui->main_Table->installEventFilter(this);
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    addTablesToMainMenu(dataProcessor->getTableNames());
    addToolBar(Qt::TopToolBarArea, createToolBar());
    display(query, DEFAULT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::display(QSqlQuery query, int param)
{
    qDebug() << query.lastQuery();
    QStringList colNames = dataProcessor->getColNames(query);
    int colCount = dataProcessor->getColNum(query);
    ui->main_Table->clear();
    ui->main_Table->setColumnCount(colCount);
    ui->main_Table->setHorizontalHeaderLabels(colNames);
    initColumnsWidth(colNames);
    //qDebug() << dataProcessor->getColTypes(dataProcessor->getCurrentTable());
    int i=0;
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

                    if (dataProcessor->hasDependency(currentTable, colNames[j]) && !dataProcessor->isAutoFill(currentTable, colNames[j]))
                        {
                            ui->main_Table->setCellWidget(i,j,initComboBox(currentTable, colNames[j], dataProcessor->getValue(query.lastQuery(), query.at(), j)));
                        }
                        else
                        {
                            ui->main_Table->setItem(i, j, item);
                            item -> setText(dataProcessor->getValue(query.lastQuery(), query.at(), j));
                        }
                 }
                else
                {
                    if (dataProcessor->hasDependency(currentTable,colNames[j]) && !dataProcessor->isAutoFill(currentTable, colNames[j]))

                        ui->main_Table->setCellWidget(i,j,initComboBox(currentTable, colNames[j], dataProcessor->getValue(query.lastQuery(), query.at(), j)));
                    else
                        item -> setText(dataProcessor->getValue(query.lastQuery(), query.at(), j));
                }

            }
            i++;

        }
        //ui->main_Table->resizeRowsToContents();
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

                        if (dataProcessor->hasDependency(currentTable, colNames[j]) && !dataProcessor->isAutoFill(currentTable, colNames[j]))
                        {
                            ui->main_Table->setCellWidget(i,j,initComboBox(currentTable, colNames[j], dataProcessor->getValue(query.lastQuery(), query.at(), j)));
                        }
                        else
                        {
                            ui->main_Table->setItem(i, j, item);
                            item -> setText(dataProcessor->getValue(query.lastQuery(), query.at(), j));
                        }
                    }
                    else
                    {
                        if (dataProcessor->hasDependency(currentTable,colNames[j]) && !dataProcessor->isAutoFill(currentTable, colNames[j]))
                            ui->main_Table->setCellWidget(i,j,initComboBox(currentTable, colNames[j], dataProcessor->getValue(query.lastQuery(), query.at(), j)));
                        else
                        item -> setText(dataProcessor->getValue(query.lastQuery(), query.at(), j));
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
                    else
                    {
                        QTableWidgetItem *item = ui->main_Table->item(i, j);
                        if (!item)
                        {
                            item = new QTableWidgetItem();

                            if (dataProcessor->hasDependency(currentTable, colNames[j]) && !dataProcessor->isAutoFill(currentTable, colNames[j]))
                            {
                                ui->main_Table->setCellWidget(i,j,initComboBox(currentTable, colNames[j]));
                            }
                            else
                            {
                                ui->main_Table->setItem(i, j, item);
                                item -> setText("");
                            }
                        }
                        else
                        {
                            if (dataProcessor->hasDependency(currentTable,colNames[j]) && !dataProcessor->isAutoFill(currentTable, colNames[j]))
                                ui->main_Table->setCellWidget(i,j,initComboBox(currentTable, colNames[j]));
                            else
                            item -> setText("");
                        }
                    }

                }

                else if (i > rowCount)
                    endOfLoop = true;
               }
            i++;
        }
        ui -> retranslateUi(this);
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

        if (item->text() != "")
        {
            insertValues.append(item->text());
        }
        else if (i == ui->main_Table->currentColumn())
        {
         insertValues.append(item->text());
        }
        else if (colTypes[i].contains("INTEGER") || colTypes[i].contains("int"))
        {
            insertValues.append("0");
        }
        else
            insertValues.append("-");
    }
    strInsertValues = dataProcessor->getValueParams(colTypes, insertValues);
    dataProcessor->insertRecord(currentTable, strInsertValues, colNames);

}

void MainWindow::updateQTableWidgetRow(QStringList colNames, QStringList colTypes)
{
    IOProcessor inputCheck;
    QStringList values;
    QStringList columnNames;
    QMap<QString, QString> foreginVals;
    QTableWidgetItem *item = ui->main_Table->currentItem();
    if(!item)
    {
        item = new QTableWidgetItem();
        ui->main_Table->setItem(ui->main_Table->currentRow(), ui->main_Table->currentColumn(), item);
    }

    QString cellType;
    int parentID;

    if (dataProcessor->hasDependency(currentTable, colNames[ui->main_Table->currentColumn()]) &&
            !dataProcessor->isAutoFill(currentTable,colNames[ui->main_Table->currentColumn()]))
        cellType = item->tableWidget()->cellWidget(item->tableWidget()->currentRow(), item->tableWidget()->currentColumn())->metaObject()->className();
    else
        cellType = "";

    if (cellType.contains("QComboBox") && colTypes[ui->main_Table->currentColumn()].contains("INTEGER"))
    {
        values.append(qobject_cast<QComboBox*>(item->tableWidget()->cellWidget(item->tableWidget()->currentRow(),
                                                                               item->tableWidget()->currentColumn()))->currentText());
        foreginVals[colNames[ui->main_Table->currentColumn()]] = values[0];
        parentID = dataProcessor->getValue(currentTable, ui->main_Table->currentRow(),
                                           dataProcessor->getParentColumnName(currentTable,colNames[ui->main_Table->currentColumn()])).toInt();
    }
    else if (cellType.contains("QComboBox"))
    {
        values.append("'" + qobject_cast<QComboBox*>(item->tableWidget()->cellWidget(item->tableWidget()->currentRow(),
                                                                                     item->tableWidget()->currentColumn()))->currentText() + "'");
        foreginVals[colNames[ui->main_Table->currentColumn()]] = values[0];
        parentID = dataProcessor->getValue(currentTable, ui->main_Table->currentRow(),
                                           dataProcessor->getParentColumnName(currentTable,colNames[ui->main_Table->currentColumn()])).toInt();
    }

    else if (colTypes[ui->main_Table->currentColumn()].contains("INTEGER"))
        values.append(item->text());
    else
        values.append("'" + item->text() + "'");


    if (!foreginVals.isEmpty())
    {
        if (inputCheck.checkInput(currentTable,foreginVals,parentID))
        {
            columnNames.append(colNames[ui->main_Table->currentColumn()]);
            dataProcessor->updateRecord(currentTable, columnNames, values, ui->main_Table->currentRow());
        }
        else
            qDebug() << "Updated Data does not exist in parent table!";
    }
    else
    {
        columnNames.append(colNames[ui->main_Table->currentColumn()]);
        dataProcessor->updateRecord(currentTable, columnNames, values, ui->main_Table->currentRow());
    }
}

void MainWindow::initColumnsWidth(QStringList colNames)
{

    for (int i = 0; i < colNames.size(); i++)
    {
        if (colNames[i].length() < 12 && i > 0)
            ui->main_Table->setColumnWidth(i, 120);
        else
            ui->main_Table->resizeColumnToContents(i);


       // qDebug() << ui->main_Table->columnWidth(i);
    }
}

QComboBox *MainWindow::initComboBox(QString tableName, QString colName, QString currentText)
{
    QComboBox *comboBox = new QComboBox;
    TextFileProcessor tfp;
    QString tablePath = dataProcessor->getDependenciesPath();
    QString FK_tableName, FK_colName;
    QStringList columns = tfp.readFromFile(tablePath), values, colParts;

    for (auto column : columns)
    {
        colParts = column.split(".");
        if (colParts[1].contains(colName) && colParts[0].contains(tableName))
        {
            FK_tableName = colParts[3];
            FK_colName = colParts[4];
            break;
        }
    }


    QSqlQuery query = dataProcessor->selectAll(FK_tableName, FK_colName);

    while (query.next())
    {
        values.append(query.value(0).toString());
    }

    values.removeDuplicates();

    comboBox->addItems(values);
    comboBox->setCurrentText(currentText);

    return comboBox;
}


void MainWindow::on_button_addKolumn_clicked()
{
    //QVariant variant;
    QMetaType type;
    QString strColumns, dependency;
    QStringList colNames = dataProcessor->getColNames(currentTable),
            colTypes = dataProcessor->getColTypes(currentTable),
            parameters = dataProcessor->getColParams(currentTable);
    AddColumnWindow acw;
    TextFileProcessor tfp;

    if (acw.exec())
    {
        if (acw.getForeginKeyStatus() != "")
        {
            dependency = dataProcessor->getCurrentTable() + acw.getForeginKeyStatus();
            tfp.writeToFile(DEPENDENCIES_PATH, dependency, WriteToFileParams::To_End_Of_File);
        }

        QString param = "";

        if (acw.getUniqueStatus() != "" && acw.getPrimaryKeyStatus() != "")
            param = acw.getUniqueStatus() + " " + acw.getPrimaryKeyStatus();
        else
        {
            if (acw.getUniqueStatus() != "")
                param = acw.getUniqueStatus();

            if (acw.getPrimaryKeyStatus() != "")
                param = acw.getPrimaryKeyStatus();
        }

        parameters.append(param);

        colNames.append(acw.getColName());
        colTypes.append(acw.getColType());


        dataProcessor->addColumn(currentTable, colNames, colTypes, parameters);
    }


    qDebug() << dependency;


    QSqlQuery query = dataProcessor->selectAll(currentTable);
    display(query, DEFAULT);
}

void MainWindow::on_button_deleteColumn_clicked()
{
    dataProcessor->deleteColumn(currentTable, ui->main_Table->currentColumn());
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    display(query, DEFAULT);
}

void MainWindow::on_button_addVehicle_clicked()
{
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    display(query ,ADDITIONAL_ROW);
}

void MainWindow::on_button_deleteVehicle_clicked()
{
    QTableWidgetItem *item = ui->main_Table->item(ui->main_Table->currentRow(), 0);
    dataProcessor->deleteRow(currentTable, item->text().toInt());
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    display(query, DEFAULT);
}

void MainWindow::on_button_sort_clicked()
{
    sw = new SelectWindow(currentTable);
    QSignalMapper mapper;
    connect(sw->getOkButton(),SIGNAL(clicked()),this,SLOT(on_getSelectConditions()));
    sw->show();
}

void MainWindow::on_getSelectConditions()
{
    QSqlQuery query;
    QStringList conditionSplitter, names, values, signs;
    lastSelectConditions = sw->getSelectConditions();
    for(int i=0; i<lastSelectConditions.size(); i++)
    {
         conditionSplitter = lastSelectConditions[i].split(" ");
         names.append(conditionSplitter[0]);
         signs.append(conditionSplitter[1]);
         values.append(conditionSplitter[2]);
         conditionSplitter.clear();
    }

    qDebug() << names << signs << values;
    query = dataProcessor->selectExact(currentTable, names, values, signs, sw->getShowConditions());
    lastSelectQuery = query.lastQuery();
    display(query, DEFAULT);

}


bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
    bool returnStatement = false;
    QStringList colNames = dataProcessor->getColNames(currentTable);
    QStringList colTypes = dataProcessor->getColTypes(currentTable);
    QStringList selectColNames, selectColValues, comparitionSigns, colsToShow;
    QSqlQuery query;

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
             query = dataProcessor->selectExact(currentTable, selectColNames, selectColValues, comparitionSigns, colsToShow);

             if (query.next())
             {
                updateQTableWidgetRow(colNames, colTypes);
             }
             else
                insertQTableWidgetRow(colNames, colTypes);

             QSqlQuery query = dataProcessor->selectAll(currentTable);
             ui->main_Table->update();
             display(query, DEFAULT);
             returnStatement = true;
         }
             else if (ke->key() == Qt::Key_Left || ke->key() == Qt::Key_Right || ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Down)
         {
              QSqlQuery query = dataProcessor->selectAll(currentTable);
              display(query, DEFAULT);
             returnStatement = true;
         }
         else
         {
             returnStatement = false;
         }

    }
    return returnStatement;
}

QToolBar *MainWindow::createToolBar()
{
    QToolBar *tb = new QToolBar("Main Toolbar");

    tb->addAction(ui->actionRefresh);

    return tb;
}

void MainWindow::addTablesToMainMenu(QStringList tableNames)
{
    QMenu *selectTables = new QMenu("&Wybierz tabelę");
    ui->menuTabel->insertMenu(ui->actionUsun_tabel,selectTables);

    for (int i = 0; i < tableNames.size(); i++)
    {
        QAction *selectTableAction = selectTables->addAction("&" + tableNames[i], this, SLOT(on_select_current_table()));
    }
}


void MainWindow::on_actionDodaj_tabel_2_triggered()
{
    AddTableWindow atw;

    if (atw.exec())
        dataProcessor->addTable(atw.getNewTableName());
    currentTable = dataProcessor->getCurrentTable();
    ui->label_tableName->setText(currentTable);
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    this->repaint();
    display(query, DEFAULT);


}

void MainWindow::on_actionUsun_tabel_triggered()
{
    dataProcessor->deleteTable(currentTable);
    currentTable = dataProcessor->getCurrentTable();
    ui->label_tableName->setText(currentTable);
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    this->repaint();
    display(query, DEFAULT);

}

void MainWindow::on_select_current_table()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString str = action->text();
        str.remove(0,1);
        dataProcessor->setCurrentTable(str);
        currentTable = str;
        QSqlQuery query = dataProcessor->selectAll(currentTable);
        ui->label_tableName->setText(currentTable);

        display(query, DEFAULT);
    }

}


void MainWindow::on_actionRefresh_triggered()
{
    QSqlQuery query = dataProcessor->selectAll(currentTable);
    display(query, DEFAULT);
}

void MainWindow::on_actionDrukuj_triggered()
{
    #ifndef QT_NO_PRINTER
    QPrinter printer;
    printer.setOrientation(QPrinter::Landscape);
    printer.setPaperSize(QPrinter::A4);
    printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);
    QPrintPreviewDialog *printDlg = new QPrintPreviewDialog(&printer, this, Qt::Window);

    connect(printDlg, &QPrintPreviewDialog::paintRequested, this, &MainWindow::on_print_prewiew_needs);

    printDlg->showMaximized();
    if (printDlg->exec())
    {

    }
    #endif
}

void MainWindow::on_print_prewiew_needs(QPrinter *poPrinter_p)
{
       QRect qPageRect = poPrinter_p->pageRect();
       QPainter qPainter(poPrinter_p);
       MyPrinter myPrinter;
       QSqlQuery query;

       if (lastSelectQuery != "")
           query.exec(lastSelectQuery);
       else
           query = dataProcessor->selectAll(currentTable);

       QStringList printContents = dataProcessor->getQuerySelectResultRecords(query);
       QTextDocument *textDoc = myPrinter.createHtmlPrintDocument(poPrinter_p, printContents);

       textDoc->drawContents(&qPainter, QRect(0, 0, qPageRect.width(), qPageRect.height()));

       myPrinter.printPageHeaderFooter(&qPainter, qPageRect, 1);


}


void MainWindow::on_pushButton_clicked()
{
    dataProcessor->addParameter(dataProcessor->getCurrentTable(), "id", "PRIMARY KEY");
}
