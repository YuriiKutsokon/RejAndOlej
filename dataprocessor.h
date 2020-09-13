#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H
#include <QtSql>


class DataProcessor
{
public:
    DataProcessor();
    DataProcessor(QString);
    QString getCurrentTable();
    void addTable(QString);
    QSqlQuery selectAll(QString);
    int getColNum(QString);
    int getRowNum(QString);
    QStringList getColNames(QString);
    QString getValue(QString,int,int);

private:

    QSqlDatabase mdb;
    QString currentTable;
};

#endif // DATAPROCESSOR_H
