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
    existingTableNames = mdb.tables();

    QString defPath = QDir::currentPath();
    defPath = defPath.left(defPath.lastIndexOf(char('/')));
    defPath += DEFAULTS_PATH;

    //qDebug() << defPath;

    initDatabase(defPath);

    if (!existingTableNames.isEmpty())
        setCurrentTable(existingTableNames[0]);
    else
        setCurrentTable("BD jest pusta");

    bool b;
    QSqlQuery query("PRAGMA foreign_keys=on;");
    b = query.exec();
    if (!b)
        qDebug() << query.lastError();
    else
        qDebug() << "foreign key is on";

}

DataProcessor::DataProcessor(QString tableName)
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
    existingTableNames = mdb.tables();

    if (!existingTableNames.isEmpty())
        setCurrentTable(tableName);
 }

QString DataProcessor::getCurrentTable()
{
    return currentTable;
}

void DataProcessor::setCurrentTable(QString tableName)
{
    currentTable = tableName;

    if (tableName.contains("_auto"))
    {
        QStringList depSplitted = findDependency(tableName, "id").split("."), colsToInsert;
        QString parentTable = depSplitted[3];
        QStringList parentCols = getColNames(parentTable), parentColsToInsert;
        QSqlQuery parent = selectAll(parentTable), inherited = selectAll(tableName);
        int parentRowCount = getRowNum(parent), inheritedRowCount = getRowNum(inherited);

        QStringList dependencies = getDependenies(tableName);

        for (auto dep : dependencies)
        {
            depSplitted = dep.split(".");
            colsToInsert.append(depSplitted[1]);
            parentColsToInsert.append(depSplitted[4].remove("\n"));
        }



        parent.exec(parent.lastQuery());
            while (parent.next())
               {
                if (parent.at() > inheritedRowCount - 1)
                   {
                       QStringList values, parentVals = getQuerySelectResultRecords(parent)[parent.at() + 1].split(" ");
                       int parentValCount = parentVals.size();

                       for (int i = 0, j = 0; i < parentValCount; i++)
                       {
                           if (parentColsToInsert.contains(parentCols[i]))
                           {
                               if (getType(tableName, colsToInsert[j]).contains("INTEGER") || getType(tableName, colsToInsert[j]).contains("int"))
                                   values.append(parentVals[i]);
                               else
                                   values.append("'" + parentVals[i] + "'");
                               j++;
                           }
                       }
                       insertRecord(tableName, attachStringList(values,", "), colsToInsert);
                   }
                else
                {
                    QStringList parentVals = getQuerySelectResultRecords(parent)[parent.at() + 1].split(" "), values;

                        for (int i = 0, j = 0; i < parentVals.size(); i++)
                        {
                            if (parentColsToInsert.contains(parentCols[i]))
                            {
                                if (getType(tableName, colsToInsert[j]).contains("INTEGER") || getType(tableName, colsToInsert[j]).contains("int"))
                                    values.append(parentVals[i]);
                                else
                                    values.append("'" + parentVals[i] + "'");
                                j++;
                            }
                        }
                        updateRecord(tableName, colsToInsert, values, parent.at());

                }
               }

    }
}

void DataProcessor::addTable(QString tableName)
{
    bool b;
    QSqlQuery query;
    TextFileProcessor tfp;
    QString strQuery = "CREATE TABLE " + tableName + " (id integer);", saveToFileValues, saveToFilePath = getTablePath(tableName);
    b = query.exec(strQuery);
    if (!b)
        qDebug() << query.lastError().text();
    else
    {
        qDebug() << "Table " + tableName + " is created!";
        existingTableNames.append(tableName);
        currentTable = tableName;
    }

    QStringList id;
    id.append("id");

    insertRecord(tableName, "0", id);

    saveToFileValues = "id INTEGER";

    tfp.writeToFile(saveToFilePath, saveToFileValues, WriteToFileParams::Rewrite_File);
}

void DataProcessor::addTable(QString tableName, QStringList colNames, QStringList colTypes, QStringList params)
{
    bool b;
    TextFileProcessor tfp;
    QSqlQuery query;
    QString strQuery, strColumns, strInsertValues, saveToFileValues, saveToFilePath = getTablePath(tableName),
            check;
    QStringList values, dependencies = tfp.readFromFile(getDependenciesPath()),  primaryKeys;

    for (int i = 0; i < colNames.size(); i++)
    {
        if (params[i] == "")
            saveToFileValues += colNames[i] + " " + colTypes[i] + "\n";
        else
            saveToFileValues += colNames[i] + " " + colTypes[i] + " " + params[i];

        if (params[i].contains("PK"))
        {
            primaryKeys.append(colNames[i]);
            params[i].remove("PK");
        }

        strColumns += colNames[i] + " " + colTypes[i] + " " + params[i] + ", ";


        if (colTypes[i].contains("INTEGER") || colTypes[i].contains("int"))
            values.append("0");
        else
            values.append("-");
    }

    if (!primaryKeys.isEmpty())
    {
        QString strPrimaryKeys = "";

        for (auto pk : primaryKeys)
        {
            strPrimaryKeys += pk + ", ";
        }

        strPrimaryKeys = strPrimaryKeys.left(strPrimaryKeys.lastIndexOf(','));
        strColumns += "PRIMARY KEY (" + strPrimaryKeys + "), ";
    }


   for (auto colname : colNames)
   {
       if (colname.contains("_fk"))
       {
           strColumns += "FOREIGN KEY (" + colname + ") REFERENCES " + colname.left(colname.indexOf("_")) + "(id), ";
       }
   }


    strColumns = strColumns.left(strColumns.lastIndexOf(','));

    strQuery = "CREATE TABLE " + tableName + " (" + strColumns + ");";
    qDebug() << strQuery;

    b = query.exec(strQuery);
    if (!b)
        qDebug() << query.lastError().text();
    else
    {
        qDebug() << "Table " + tableName + " is created!";
        existingTableNames.append(tableName);
        currentTable = tableName;
    }

    strInsertValues = getValueParams(colTypes, values);
    insertRecord(tableName, strInsertValues, colNames);

    tfp.writeToFile(saveToFilePath, saveToFileValues, WriteToFileParams::Rewrite_File);
}

void DataProcessor::addColumn(QString tableName, QString newColName, QString newColType)
{
    bool b;
    QSqlQuery query;
    TextFileProcessor tfp;
    QString strQuery = "", saveToFilePath = getTablePath(tableName), saveToFileValues;
    strQuery = "ALTER TABLE " + tableName + " ADD COLUMN %1 %2";
    strQuery = strQuery.arg(newColName).arg(newColType);

    b = query.exec(strQuery);
    if (!b)
        qDebug() << query.lastError().text();
    else
        qDebug() << "column " << newColName << " had been successfully added!" ;

    saveToFileValues = newColName + " " + newColType + "\n";


    tfp.writeToFile(saveToFilePath, saveToFileValues, WriteToFileParams::To_End_Of_File);
}

void DataProcessor::addColumn(QString tableName, QStringList colNames, QStringList colTypes, QStringList params)
{
    bool b;
    QSqlQuery query;
    TextFileProcessor tfp;
    QStringList strQueryList, dependencies = tfp.readFromFile(getDependenciesPath()),
            primaryKeys;
    QString strColumns, strColNames, saveToFilePath = getTablePath(tableName), saveToFileValues;
    int colNum = getColNum(tableName);

    for (int i = 0; i < colNum + 1; i++)
    {
        if (params[i] == "")
            saveToFileValues += colNames[i] + " " + colTypes[i] + "\n";
        else
            saveToFileValues += colNames[i] + " " + colTypes[i] + " " + params[i];

        if (params[i].contains("PK"))
        {
            primaryKeys.append(colNames[i]);
            params[i].remove("PK");
        }

        strColumns += colNames[i] + " " + colTypes[i] + " " + params[i] + ", ";

        if (i < colNum - 1)
            strColNames += colNames[i] + ", ";
    }

    if (!primaryKeys.isEmpty())
    {
        QString strPrimaryKeys = "";

        for (auto pk : primaryKeys)
        {
            strPrimaryKeys += pk + ", ";
        }

        strPrimaryKeys = strPrimaryKeys.left(strPrimaryKeys.lastIndexOf(','));
        strColumns += "PRIMARY KEY (" + strPrimaryKeys + "), ";
    }

    for (auto colname : colNames)
    {
        if (colname.contains("_fk"))
        {
            strColumns += "FOREIGN KEY (" + colname + ") REFERENCES " + colname.left(colname.indexOf("_")) + "(id), ";
        }
    }

    strColNames = strColNames.left(strColNames.lastIndexOf(','));
    strColumns = strColumns.left(strColumns.lastIndexOf(','));

    strQueryList.append("CREATE TABLE backup(" + strColumns + ");");
    strQueryList.append("INSERT INTO backup(" + strColNames + ") SELECT " + strColNames + " FROM " + tableName +';');
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

    tfp.writeToFile(saveToFilePath, saveToFileValues, WriteToFileParams::To_End_Of_File);

}

void DataProcessor::deleteColumn(QString tableName, int index)
{
    bool b;
    TextFileProcessor tfp;
    QSqlQuery query;
    QStringList strQueryList;
    QStringList colNames = getColNames(tableName);
    QStringList colTypes = getColTypes(tableName);
    QStringList params = getColParams(tableName), primaryKeys;
    QStringList dependencies = tfp.readFromFile(getDependenciesPath()), dependency;
    QString saveToFilePath = getTablePath(tableName);
    int colNum = getColNum(tableName);
    QString strColumns = "", strColNames = "", saveToFileValues;

    for (int i = 0; i < colNum; i++)
    {
        if (i != index)
        {
            if (params[i] == "")
                saveToFileValues += colNames[i] + " " + colTypes[i] + "\n";
            else
                saveToFileValues += colNames[i] + " " + colTypes[i] + " " + params[i] + "\n";

            if (params[i].contains("PK"))
            {
                primaryKeys.append(colNames[i]);
                params[i].remove("PK");
            }

            strColumns += colNames[i] + " " + colTypes[i] + " " + params[i] + ", ";

            strColNames += colNames[i] + ", ";

        }
    }

    if (!primaryKeys.isEmpty())
    {
        QString strPrimaryKeys = "";

        for (auto pk : primaryKeys)
        {
            strPrimaryKeys += pk + ", ";
        }

        strPrimaryKeys = strPrimaryKeys.left(strPrimaryKeys.lastIndexOf(','));
        strColumns += "PRIMARY KEY (" + strPrimaryKeys + "), ";
    }

    for (auto colname : colNames)
    {
        if (colname.contains("_fk"))
        {
            strColumns += "FOREIGN KEY (" + colname + ") REFERENCES " + colname.left(colname.indexOf("_")) + "(id), ";
        }
    }

    strColumns = strColumns.left(strColumns.lastIndexOf(','));
    strColNames = strColNames.left(strColNames.lastIndexOf((',')));

    strQueryList.append("CREATE TABLE backup(" + strColumns + ");");
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

    tfp.writeToFile(saveToFilePath, saveToFileValues, WriteToFileParams::Rewrite_File);

}

void DataProcessor::deleteTable(QString tableName)
{
    bool b;
    QSqlQuery query;
    QString strQuery = "DROP TABLE " + tableName, deleteFilePath = QDir::currentPath();
    
    b = query.exec(strQuery);
        qDebug() << query.at();
    if (!b)
        qDebug() << query.lastError().text();
    else
        qDebug() << "Table " + tableName + " was successfully deleted!";
    existingTableNames.removeAt(existingTableNames.indexOf(tableName));
    
    deleteFilePath = deleteFilePath.left(deleteFilePath.lastIndexOf('/'));
    deleteFilePath += TABLES_PATH; 
    deleteFilePath += "/" + tableName + ".txt";
    
    QFile file(deleteFilePath);
    
    if (file.exists())
        file.remove();
    else
        qDebug() << "The " << tableName << ".txt does not exist!";
    
    if (!existingTableNames.isEmpty())
        currentTable = existingTableNames[0];
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
    QString strQuery = "", strColNames = "", tablePath = getTablePath(tableName);
    QSqlQuery query;
    TextFileProcessor tfp;
    QStringList columns = tfp.readFromFile(tablePath);

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
        updateValues += colNames[i] + " = " + values[i] + ", ";
    }
    updateValues = updateValues.left(updateValues.lastIndexOf(','));
    QString strQuery = "UPDATE " + tableName + " SET " + updateValues + " WHERE id=" + strRecordId;
    b = query.exec(strQuery);
    if (!b)
        qDebug() << query.lastError();
    else
        qDebug () << "record " + strRecordId + " was successfully updated!";

}

void DataProcessor::initDatabase(QString defaultsPath)
{
    TextFileProcessor tfp;
    IOProcessor iop;
    QDir defaults(defaultsPath);
    QStringList filters, fileNames, columns, colNames, colTypes, params;
    QStringList temp;
    filters << "*.txt";
    fileNames = defaults.entryList(filters);

    //qDebug() << defaultTableNames;

    for (int i = 0; i < fileNames.size(); i++)
    {
        defaultTableNames.append(fileNames[i].left(fileNames[i].lastIndexOf('.')));
        if (existingTableNames.contains(defaultTableNames[i]))
        {
        }
        else
        {
            columns = tfp.readFromFile(defaultsPath + "/" + fileNames[i]);
            for (int i = 0; i < columns.size(); i++)
            {
                temp = columns[i].split(" ");
                colNames.append(temp[0]);
                colTypes.append(temp[1].remove("\n"));

                QString param;
                if (temp.size() > 2)
                {
                    for (int i = 0; i < temp.size() - 2; i++)
                    {
                        param += temp[i + 2] + " ";
                    }
                    param.remove(param.lastIndexOf(" "), 1);
                    params.append(param);
                }
                else params.append("");
            }
            addTable(defaultTableNames[i],colNames,colTypes, params);

            if (defaultTableNames[i].contains("auto"))
            {
                iop.tableAutoFill(defaultTableNames[i]);
            }

            colNames.clear();
            colTypes.clear();
            params.clear();
        }
        qDebug() << defaultTableNames[i];

    }

}

void DataProcessor::addParameter(QString tableName, QString colName, QString parameter)
{
    TextFileProcessor tfp;
    QString strColumns = "", saveToFileValues = "", strColNames = "", depPath = getDependenciesPath(),
                                                                      saveToFilePath = getTablePath(tableName);
    QStringList colNames = getColNames(tableName);
    QStringList colTypes = getColTypes(tableName);
    QStringList parameters = getColParams(tableName);
    QStringList dependencies = tfp.readFromFile(depPath);
    QStringList strQueryList;
    QSqlQuery query;
    bool b;

    int colNum = getColNum(tableName);

    for (int i = 0; i < colNum; i++)
    {
        if (colNames[i] == colName)
        {
            if (parameter == "PK")
                parameters[i] = parameter + " " + parameters[i] + "\n";
            else if (parameters[i] != "")
                parameters[i] += " " + parameter + "\n";
            else
                parameters[i] += parameter + "\n";
        }
    }

    QStringList primaryKeys;
    QString check;

    for (int i = 0; i < colNum; i++)
    {
        if (parameters[i] == "")
        {
            strColumns += colNames[i] + " " + colTypes[i] + ", ";
            colTypes[i] += "\n";
            saveToFileValues += colNames[i] + " " + colTypes[i];
        }
        else
        {
            saveToFileValues += colNames[i] + " " + colTypes[i] + " " + parameters[i];
            if (parameters[i].contains("PK"))
            {
                parameters[i].remove("PK");
                primaryKeys.append(colNames[i]);
            }
            strColumns += colNames[i] + " " + colTypes[i] + " " +  parameters[i] + ", ";
        }

         strColNames += colNames[i] + ", ";

    }

    if (!primaryKeys.isEmpty())
    {
        QString strPrimaryKeys = "";

        for (auto pk : primaryKeys)
        {
            strPrimaryKeys += pk + ", ";
        }

        strPrimaryKeys = strPrimaryKeys.left(strPrimaryKeys.lastIndexOf(','));
        strColumns += "PRIMARY KEY (" + strPrimaryKeys + "), ";
    }

    for (auto colname : colNames)
    {
        if (colname.contains("_fk"))
        {
            strColumns += "FOREIGN KEY (" + colname + ") REFERENCES " + colname.left(colname.indexOf("_")) + "(id), ";
        }
    }

     strColNames = strColNames.left(strColNames.lastIndexOf(','));
     strColumns = strColumns.left(strColumns.lastIndexOf(','));

     strQueryList.append("CREATE TABLE backup(" + strColumns + ");");
     strQueryList.append("INSERT INTO backup(" + strColNames + ") SELECT " + strColNames + " FROM " + tableName +';');
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

     tfp.writeToFile(saveToFilePath, saveToFileValues, WriteToFileParams::Rewrite_File);

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

QSqlQuery DataProcessor::selectAll(QString tableName, QString columnName)
{
    bool b;
    QSqlQuery query;
    QString strQuery = "SELECT " + columnName + " FROM " + tableName + ";";
    b = query.exec(strQuery);
  /*  if (!b)
        qDebug() << query.lastError();
    else
        qDebug() << "Data is selected";
  */
  return query;

}

QSqlQuery DataProcessor::selectExact(QString tableName, QStringList colNames, QStringList values, QStringList compSigns, QStringList colsToShowConditions)
{
    bool b;
    QSqlDatabase mdb = QSqlDatabase::database();
    QSqlQuery query;
    QSqlRecord rec = mdb.record(tableName);
    QString strQuery, strSelect, strColsToShow;

    for (int i = 0; i < values.size(); i++)
    {
        strSelect += colNames[i] + compSigns[i] + values[i] + " AND ";
    }

    strSelect = strSelect.left(strSelect.lastIndexOf('A'));
    if (colsToShowConditions.isEmpty())
    {
        if (strSelect != "")
            strQuery = "SELECT * FROM " + tableName + " WHERE " + strSelect;
        else
            strQuery = "SELECT * FROM " + tableName;

        b = query.exec(strQuery);
        if (!b)
            qDebug() << query.lastError();
        else
            qDebug() << "Data is selected";
    }
    else
    {
        for (int i = 0; i < colsToShowConditions.size(); i++)
        {
            if (colsToShowConditions[i] == "true")
            strColsToShow += rec.fieldName(i) + ", ";
        }
        strColsToShow = strColsToShow.left(strColsToShow.lastIndexOf(','));

        if (strSelect != "")
            strQuery = "SELECT " + strColsToShow + " FROM " + tableName + " WHERE " + strSelect;
        else
            strQuery = "SELECT " + strColsToShow + " FROM " + tableName;

        b = query.exec(strQuery);
        if (!b)
            qDebug() << query.lastError();
        else
            qDebug() << "Data is selected";
    }

    return query;
}

int DataProcessor::getColNum(QSqlQuery query)
{
    query.exec(query.lastQuery());
    int colCount = 0;
    QSqlDatabase mdb = QSqlDatabase::database();
    QSqlRecord rec = query.record();
    if (query.next())
    {
        for ( int i=0; i<rec.count(); i++)
        {
            colCount++;
        }
    }

    return colCount;
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
    query.exec(query.lastQuery());
    int rowCount = 0;
    while (query.next())
    {
        rowCount++;
    }
    return rowCount;
}

int DataProcessor::getServicesCount(QString tableName)
{
    QStringList colNames = getColNames(tableName);
    int count = 0;
    for (auto colName : colNames)
    {
        if (colName.contains("licznik_") && !colName.contains("licznik_a"))
            count++;
    }

    return count;
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

QStringList DataProcessor::getColNames(QSqlQuery query)
{
    query.exec(query.lastQuery());
    QSqlRecord rec = query.record();
    QStringList colnames;

    for (int i = 0; i < rec.count(); i++)
    {
        colnames.append(rec.fieldName(i));
    }
    //qDebug() << colnames;
    return colnames;
}

QStringList DataProcessor::getColTypes(QString tableName)
{
    QString filePath = getTablePath(tableName);
    QStringList types;
    TextFileProcessor tfp;

    QStringList colsList = tfp.readFromFile(filePath);
    QStringList colsSplitted;

    for (auto column: colsList)
    {
        colsSplitted = column.split(" ");
        types.append(colsSplitted[1].remove("\n"));
    }

    return types;
}

QStringList DataProcessor::getColParams(QString tableName)
{
    QString filePath = getTablePath(tableName);
    QStringList params;
    TextFileProcessor tfp;

    QStringList colsList = tfp.readFromFile(filePath);
    QStringList colsSplitted;

    for (auto column: colsList)
    {
        colsSplitted = column.split(" ");
        if (colsSplitted.size() > 2)
        {
            for (int i = 0; i < colsSplitted.size() - 2; i++)
            {
                params.append(colsSplitted[i + 2].remove("\n"));
             }
        }
        else
            params.append("");
    }

    return params;
}

QStringList DataProcessor::getTableNames()
{
    return existingTableNames;
}

QStringList DataProcessor::getQuerySelectResultRecords(QSqlQuery query)
{
    QStringList result;
    QString tempStr = "";
    query.exec(query.lastQuery());
    QStringList colNames = getColNames(query);
    int colNum = colNames.size();
    query.exec(query.lastQuery());
    while (query.next())
    {
        for (int i = 0; i < colNum; i++)
        {
            tempStr += query.value(i).toString() + " ";
        }
        tempStr = tempStr.left(tempStr.lastIndexOf(" "));
        result.append(tempStr);
        tempStr = "";
    }

    tempStr = "";
    for (int i = 0; i < colNum; i++ )
    {
        tempStr += colNames[i] + " ";
    }
    tempStr = tempStr.left(tempStr.lastIndexOf(" "));
    result.prepend(tempStr);

    return result;
}

QStringList DataProcessor::getDependenies(QString tableName)
{
    TextFileProcessor tfp;

    if (tableName == "")
        return tfp.readFromFile(getDependenciesPath());
    else
    {
        QStringList dependencies = tfp.readFromFile(getDependenciesPath());
        for (auto dep : dependencies)
        {
            QStringList depSplitted = dep.split(".");
            if (!depSplitted[0].contains(tableName))
            {
                dependencies.removeOne(dep);
            }
        }
        return dependencies;
    }
}

bool DataProcessor::hasDependency(QString tableName, QString colName)
{
    QString strColumn;

    QStringList cols = getDependenies(), colParts;

           for (auto column : cols)
        {
            if (column.contains(colName))
            {
                colParts = column.split(".");
                if (colParts[0].contains(tableName) && colParts[1].contains(colName) && colParts[2].contains("FK"))
                {
                    return true;
                }

            }
        }

           return false;
}

bool DataProcessor::isPrimaryKey(QString tableName, QString colName)
{
    QString tablePath = getTablePath(tableName);
    TextFileProcessor tfp;
    QStringList columns = tfp.readFromFile(tablePath);

    for (auto column : columns)
    {
        if (column.contains(colName))
        {
            QStringList colSplitted = column.split(" ");
            if (colSplitted.contains("PK"))
                return true;
        }
    }
    return false;
}

bool DataProcessor::isAutoFill(QString tableName, QString colName)
{
    QString tablePath = QDir::currentPath(), strColumn;
    TextFileProcessor tfp;
    tablePath = tablePath.left(tablePath.lastIndexOf('/'));
    tablePath += DEPENDENCIES_PATH;
    QStringList cols = tfp.readFromFile(tablePath), colParts;

           for (auto column : cols)
        {
            if (column.contains(colName))
            {
                colParts = column.split(".");
                if (colParts[0].contains(tableName) && colParts[1].contains(colName) && colParts[2].contains("FKauto"))
                {
                    return true;
                }

            }
        }

           return false;
}

QString DataProcessor::getValue(QString strQuery, int recordNum, int colNum)
{
    QSqlQuery query;
    bool b;
    b = query.exec(strQuery);
  /*  if (!b)
        qDebug() << query.lastError();
    else
        qDebug() << "getValue: selection successfull";
    */
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

QString DataProcessor::getValue(QString tableName, int recordNum, QString colName)
{
    QSqlQuery query;
    bool b;
    b = query.exec("SELECT " + colName + " FROM " + tableName + " WHERE id=" + QString::number(recordNum));
    /*  if (!b)
          qDebug() << query.lastError();
      else
          qDebug() << "getValue: selection successfull";
      */
    if (query.next())
    {
        QString value = query.value(0).toString();
        return value;
    }
    else
        return "";
}

QString DataProcessor::getValue(QSqlQuery query, int recordNum, int colNum)
{
    query.exec(query.lastQuery());
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

QString DataProcessor::getType(QString tableName, QString colName)
{
    QStringList colTypes = getColTypes(tableName), colNames = getColNames(tableName);

    for (int i = 0; i < colNames.size(); i++)
    {
           if (colNames[i] == colName)
               return colTypes[i];
    }
}

QString DataProcessor::getValueParams(QStringList typeNames, QStringList paramVals)
{
    QString tempstr = "", result = "";
     for (int i=0; i<typeNames.size(); i++)
     {
        if (typeNames[i].contains("int") || typeNames[i].contains("INTEGER"))
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

QString DataProcessor::getTablePath(QString tableName)
{
    QString tablePath = QDir::currentPath();
    tablePath = tablePath.left(tablePath.lastIndexOf('/'));
    tablePath += TABLES_PATH;
    tablePath += "/" + tableName + ".txt";
    return tablePath;
}

QString DataProcessor::getFKColumnName(QString tableName, QString colName)
{
    TextFileProcessor tfp;
    QString depPath = getDependenciesPath(), parentColName = "";
    QStringList dependencies = tfp.readFromFile(depPath);

    for (auto dep : dependencies)
    {
        QStringList depSplitted = dep.split(".");
        if (depSplitted[0].contains(tableName) && depSplitted[1].contains(colName))
            return depSplitted[3] + "_id_fk";
    }

    return "";
}

QString DataProcessor::getDependenciesPath()
{
    QString depPath = QDir::currentPath();
    depPath = depPath.left(depPath.lastIndexOf('/'));
    depPath += DEPENDENCIES_PATH;
    return depPath;
}

QString DataProcessor::findDependency(QString tableName, QString colName)
{
    QStringList dependencies = getDependenies(tableName);
    QString result;
    for (auto dep : dependencies)
    {
        if (dep.contains(tableName + "." + colName + "."))
            result = dep;
    }
    return result;
}

QString DataProcessor::attachStringList(QStringList stringList, QString separator)
{
    QString result;

    for (auto str : stringList)
        result += str + separator;

    if (separator.contains(" "))
        separator.remove(" ");

    result = result.left(result.lastIndexOf(separator));
   /*
    if (separator.contains(','))
        result = result.left(result.lastIndexOf(','));
    else if (separator.contains('.'))
        result = result.left(result.lastIndexOf('.'));
    */

    return result;
}

void DataProcessor::executeQuery(QString query)
{
    QSqlQuery q;
    bool b;
    b = q.exec(query);
    if (!b)
        qDebug() << q.lastError();
    else
        qDebug() << query << " executed successfully!";
}




