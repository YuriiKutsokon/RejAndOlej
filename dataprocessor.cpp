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
        qDebug() << query.lastError().text();
    else
        qDebug() << "Table " + tableName + " is created!";
    currentTable = tableName;
}

void DataProcessor::addColumn(QString tableName, QString newColName, QString newColType)
{
    bool b;
    QSqlQuery query;
    QString strQuery = "";
    strQuery = "ALTER TABLE " + tableName + " ADD COLUMN %1 %2";
    strQuery = strQuery.arg(newColName).arg(newColType);

    b = query.exec(strQuery);
    if (!b)
        qDebug() << query.lastError().text();
    else
        qDebug() << "column " << newColName << " had been successfully added!" ;
}

void DataProcessor::addColumn(QString tableName, QStringList colNames, QStringList colTypes, QString params)
{
    bool b;
    QSqlQuery query;
    QStringList strQueryList;
    QString strColumns, strColNames;
    int colNum = getColNum(tableName);

    for (int i = 0; i < colNum + 1; i++)
    {
         strColumns += colNames[i] + " " + colTypes[i] + ", ";
         if (i < colNum)
        strColNames += colNames[i] + ", ";
    }
    strColNames = strColNames.left(strColNames.lastIndexOf(','));
    strColumns = strColumns.left(strColumns.lastIndexOf(','));

    strQueryList.append("CREATE TABLE backup(" + strColumns + " " +  params + ");");
    strQueryList.append("INSERT INTO backup SELECT " + strColNames + " FROM " + tableName +';');
    strQueryList.append("DROP TABLE " + tableName + ';');
    strQueryList.append("ALTER TABLE backup RENAME TO " + tableName + ';');

    for (int i=0; i<strQueryList.size();i++ )
    {
        b = query.exec(strQueryList[i]);
        //qDebug() << strQueryList[i];
        if (!b)
            qDebug() << query.lastError().text();
        else
            qDebug() << "query had beed successfully added";
    }

}

void DataProcessor::deleteColumn(QString tableName, QString strColNames)
{
    bool b;
    QSqlQuery query;
    QStringList strQueryList;

    strQueryList.append("CREATE TABLE backup(" + strColNames + ");");
    strQueryList.append("INSERT INTO backup SELECT " + strColNames + " FROM " + tableName +';');
    strQueryList.append("DROP TABLE " + tableName + ';');
    strQueryList.append("ALTER TABLE backup RENAME TO " + tableName + ';');

    for (int i=0; i<strQueryList.size();i++ )
    {
        b = query.exec(strQueryList[i]);
        //qDebug() << strQueryList[i];
        if (!b)
            qDebug() << "Error! query had not been added!";
        else
            qDebug() << "query had beed successfully added";
    }


}

QSqlQuery DataProcessor::selectAll(QString tableName)
{
    bool b;
    QSqlQuery query;
    QString strQuery = "SELECT * FROM " + tableName + ";";
    qDebug() << strQuery;
    b = query.exec(strQuery);
    if (!b)
        qDebug() << "Data is not selected";
    else
        qDebug() << "Data is selected";
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

QStringList DataProcessor::getColTypes(QString tableName)
{
    QVariant variant;
    QStringList result;
    QSqlRecord rec = mdb.record(tableName);
    for (int i=0; i<rec.count(); i++ )
    result.append(variant.typeToName(rec.field(i).type()));
    return result;
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

QString DataProcessor::getValueParams(QStringList typeNames, QStringList paramVals)
{
    QString tempstr = "", result = "";
     for (int i=0, pos=0; i<typeNames.size(); i++)
     {
         if (typeNames[i] == "int")
             tempstr = "\%%1, ";
         else
             tempstr = "\'\%%1\', ";
         tempstr = tempstr.arg(1);
         result += tempstr;
         result = result.arg(paramVals[i]);

         if (typeNames.size() == i+1)
         {
             pos = result.lastIndexOf(QChar(','));
             result = result.left(pos);
         }
      }
     return result;

}


