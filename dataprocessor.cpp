#include "dataprocessor.h"

DataProcessor::DataProcessor()
{

}

DataProcessor::DataProcessor(QString tableName)
{
    mdb = QSqlDatabase::addDatabase("QSQLITE");
    mdb.setDatabaseName("MainData.sqlite");
    if(mdb.open())
        qDebug() << "DB is connected";
    else
        qDebug() << "DB is not connected";

    currentTable = tableName;

}

QString DataProcessor::getCurrentTable()
{
    return currentTable;
}

void DataProcessor::addTable(QString tableName)
{
    bool b;
    QSqlQuery query;
    QString strQuery = "CREATE TABLE " + tableName + " (id integer);";
    b = query.exec(strQuery);
    if (!b)
        qDebug() << "Table " + tableName + " is created!";
    else
        qDebug() << "Table " + tableName + " is NOT created!";
    currentTable = tableName;
}

QSqlQuery DataProcessor::selectAll(QString tableName)
{
    bool b;
    QSqlQuery query;
    QString strQuery = "SELECT * FROM " + tableName;
    qDebug() << strQuery;
    b = query.exec(strQuery);
    if (!b)
        qDebug() << "Data is selected";
    else
        qDebug() << "Data is not selected";
    return query;

}

int DataProcessor::getColNum(QString tableName)
{
    int colCount = 0;
    QSqlRecord rec = mdb.record(tableName);
    for ( int i=0; i<rec.count(); i++)
    {
        colCount++;
    }

    return colCount;
}

int DataProcessor::getRowNum(QString tableName)
{
    int rowCount = 0;
    QSqlQuery query = selectAll(tableName);
    while (query.next())
    {
        rowCount++;
    }
    return rowCount;
}

QStringList DataProcessor::getColNames(QString tableName)
{
    QStringList colNames;
    QSqlRecord rec = mdb.record(tableName);
    for ( int i=0; i<rec.count(); i++)
    {
        colNames.append(rec.fieldName(i));
    }
    return colNames;
}

QString DataProcessor::getValue(QString tableName, int recordNum, int colNum)
{
    QSqlQuery query = selectAll(tableName);
    QString value = "";
    while(query.next())
    {
        if (query.next() == recordNum)
        {
            value = query.value(colNum).toString();
        }
    }

    return value;
}


