#include "dataprocessor.h"

DataProcessor::DataProcessor()
{
    QSqlDatabase mdb = QSqlDatabase::database();
    if(mdb.open())
        qDebug() << "DB is connected";
    else
    {
        qDebug() << "connecting to database...";
        mdb = QSqlDatabase::addDatabase("QSQLITE");
        mdb.setDatabaseName("MainData.sqlite");
        if(mdb.open())
            qDebug() << "DB is connected";
        else
        {
            qDebug() << "Failed to connect to the database!";
         }
    }
    tableNames = mdb.tables();

    if (!tableNames.isEmpty())
        setCurrentTable(tableNames[0]);
    else
        setCurrentTable("BD jest pusta");

}

DataProcessor::DataProcessor(QString tableName)
{


}

QString DataProcessor::getCurrentTable()
{
    return currentTable;
}

void DataProcessor::setCurrentTable(QString tableName)
{
    currentTable = tableName;
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
    {
        qDebug() << "Table " + tableName + " is created!";
        tableNames.append(tableName);
        currentTable = tableName;
    }
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
        if (!b)
            qDebug() << "Error! query had not been added!";
        else
            qDebug() << "query had beed successfully added";
    }


}

void DataProcessor::deleteTable(QString tableName)
{
    bool b;
    QSqlQuery query;
    QString strQuery = "DROP TABLE " + tableName;

    b = query.exec(strQuery);
        qDebug() << query.at();
    if (!b)
        qDebug() << query.lastError().text();
    else
        qDebug() << "Table " + tableName + " was successfully deleted!";
    tableNames.removeAt(tableNames.indexOf(tableName));
    if (!tableNames.isEmpty())
        currentTable = tableNames[0];
    else
        currentTable = "BD jest pusta!";
}

void DataProcessor::deleteRow(QString tableName, int id)
{
    bool b;
    QStringList colNames, values;
    QSqlQuery query = selectAll(tableName);
    QString strnum;
    QString strQuery = "DELETE FROM " + tableName + " WHERE id=%1";
    strQuery = strQuery.arg(id);
    b = query.exec(strQuery);
    if (!b)
        qDebug() << query.lastError().text();
    else
        qDebug() << "row " + strQuery.setNum(id) + " was successfully deleted!";
    query = selectAll(tableName);
    colNames.append("id");
    for (int i = 0; query.next(); )
    {
        values.append(strnum.setNum(i));
        if ( i < id )
        {
            updateRecord(tableName,colNames,values,i);
            values.clear();
            i++;
        }
        else
        {
            updateRecord(tableName,colNames,values,i+1);
            values.clear();
            i++;
        }
    }

}

void DataProcessor::insertRecord(QString tableName, QString values, QStringList colNames)
{
    bool b;
    QString strQuery = "", strColNames = "";
    QSqlQuery query;

    for (int i = 0; i < colNames.size(); i++)
        strColNames += colNames[i] + ", ";

    strColNames = strColNames.left(strColNames.lastIndexOf(','));

    strQuery = "INSERT INTO " + tableName + "(" + strColNames + ") "
            "VALUES (" + values + ")";
    qDebug() << strQuery;

    b = query.exec(strQuery);
    if (!b)
        qDebug() << query.lastError().text();
    else
        qDebug() << "Data insertion is successfull!";



}

void DataProcessor::updateRecord(QString tableName, QStringList colNames, QStringList values, int recordId)
{
    bool b;
    QSqlQuery query;
    QString updateValues, strRecordId = "";
    strRecordId = strRecordId.setNum(recordId);

    for (int i = 0; i < values.size(); i++)
    {
        updateValues = colNames[i] + " = " + values[i] + ", ";
    }
    updateValues = updateValues.left(updateValues.lastIndexOf(','));
    QString strQuery = "UPDATE " + tableName + " SET " + updateValues + " WHERE id=" + strRecordId;
    b = query.exec(strQuery);
    if (!b)
        qDebug() << query.lastError();
    else
        qDebug () << "record " + strRecordId + " was successfully updated!";

}

QSqlQuery DataProcessor::selectAll(QString tableName)
{
    bool b;
    QSqlQuery query;
    QString strQuery = "SELECT * FROM " + tableName + ";";
    b = query.exec(strQuery);
  /*  if (!b)
        qDebug() << query.lastError();
    else
        qDebug() << "Data is selected";
  */
  return query;

}

QSqlQuery DataProcessor::selectExactRecords(QString tableName, QStringList colNames, QStringList values, QStringList compSigns)
{
    bool b;
    QSqlQuery query;
    QString strQuery, strSelect;

    for (int i = 0; i < values.size(); i++)
    {
        strSelect += colNames[i] + compSigns[i] + values[i] + ", ";
    }

    strSelect = strSelect.left(strSelect.lastIndexOf(','));

    strQuery = "SELECT * FROM " + tableName + " WHERE " + strSelect;
    b = query.exec(strQuery);
    if (!b)
            qDebug() << query.lastError();
        else
            qDebug() << "Data is selected";

    return query;
}

int DataProcessor::getColNum(QString tableName)
{
    int colCount = 0;
    QSqlDatabase mdb = QSqlDatabase::database();
    QSqlRecord rec = mdb.record(tableName);
    for ( int i=0; i<rec.count(); i++)
    {
        colCount++;
    }

    return colCount;
}

int DataProcessor::getRowNum(QSqlQuery query)
{
    int rowCount = 0;
    while (query.next())
    {
        rowCount++;
    }
    return rowCount;
}

QStringList DataProcessor::getColNames(QString tableName)
{
    QStringList colNames;
    QSqlDatabase mdb = QSqlDatabase::database();
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
    QSqlDatabase mdb = QSqlDatabase::database();
    QSqlRecord rec = mdb.record(tableName);
    for (int i=0; i<rec.count(); i++ )
    result.append(variant.typeToName(rec.field(i).type()));
    return result;
}

QStringList DataProcessor::getTableNames()
{
    return tableNames;
}

QString DataProcessor::getValue(QString tableName, int recordNum, int colNum)
{
    QSqlQuery query = selectAll(tableName);
    QString value = "";
    while(query.next())
    {
        if (query.at() == recordNum)
        {
            value = query.value(colNum).toString();
        }

    }

    return value;
}

QString DataProcessor::getValueParams(QStringList typeNames, QStringList paramVals)
{
    QString tempstr = "", result = "";
     for (int i=0; i<typeNames.size(); i++)
     {
        if (typeNames[i] == "int")
             tempstr = "\%%1, ";
        else
             tempstr = "\'\%%1\', ";
         tempstr = tempstr.arg(1);
         result += tempstr;
         result = result.arg(paramVals[i]);

      }

     int pos = result.lastIndexOf(QChar(','));
     result = result.left(pos);
     return result;

}




