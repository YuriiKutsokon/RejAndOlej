#include "ioprocessor.h"

IOProcessor::IOProcessor()
{

}

bool IOProcessor::checkInput(QString tableName, QMap<QString,QString> inputFields, int parentIndex)
{
    bool b, result = false;
    DataProcessor *dp = new DataProcessor();
    TextFileProcessor tfp;
    QString dependeciesPath = dp->getDependenciesPath(),
            parentTableName, strQuery;
    QStringList dependencies = tfp.readFromFile(dependeciesPath),
                dependencySplitted;

    QMapIterator<QString, QString> i(inputFields);
    while (i.hasNext())
    {
        i.next();
        for (auto dep : dependencies)
        {
            if (dep.contains(tableName + "." + i.key() + "."))
            {
                dependencySplitted = dep.split(".");
                parentTableName = dependencySplitted[3];
            }
        }
    }

    QSqlQuery query;
    strQuery = "SELECT * FROM " + parentTableName + " WHERE id=" + QString::number(parentIndex);
    b = query.exec(strQuery);
    if (!b)
        qDebug() << query.lastError();
    else
        qDebug() << "data selection in InputCheck is successfull!";

    i.toFront();

    if (query.next())
    {
        while (i.hasNext())
        {
            i.next();
            QString ch1 = i.value(), ch2 = query.value(i.key()).toString();
            if (i.value() == query.value(i.key()).toString())
                result = true;
            else
                result = false;
        }
    }


    return result;

}

void IOProcessor::tableAutoFill(QString tableName)
{
      DataProcessor *dp = new DataProcessor();
      QStringList colNames = dp->getColNames(tableName),
              dependencies = dp->getDependenies(tableName);
      QString strQuery, strParentCols, strColsToCopy, parentTable;

      for(auto name : colNames)
      {
          if (dp->isAutoFill(tableName,name))
          {
              strColsToCopy += name + ", ";
              QStringList dependencySplitted = dp->findDependency(tableName, name).split(".");
              strParentCols += dependencySplitted[4].remove("\n") + ", ";
              parentTable = dependencySplitted[3];
          }
      }

      strColsToCopy = strColsToCopy.left(strColsToCopy.lastIndexOf(","));
      strParentCols = strParentCols.left(strParentCols.lastIndexOf(","));

      strQuery = "DELETE FROM " + tableName + ";";
      dp->executeQuery(strQuery);

      strQuery = "INSERT INTO " + tableName + " (" + strColsToCopy + ")" + " SELECT " + strParentCols + " FROM " + parentTable + ";";
      dp->executeQuery(strQuery);

}
