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
            if (i.value() == query.value(i.key()))
                result = true;
            else
                result = false;
        }
    }


    return result;

}
