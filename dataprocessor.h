#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H
#include <QtSql>


class DataProcessor
{
public:
    DataProcessor();
    DataProcessor(QString);
    QSqlQuery selectAll(QString);
    QString getCurrentTable();
    QString getValue(QString,int,int);
    QString getValueParams(QStringList, QStringList);
    void addTable(QString);
    void addColumn(QString, QString, QString);
    void addColumn(QString, QStringList, QStringList, QString);
    void deleteColumn(QString, QString);
    int getColNum(QString);
    int getRowNum(QString);
    QStringList getColNames(QString);
    QStringList getColTypes(QString);




private:

    QSqlDatabase mdb;
    QString currentTable;
};

#endif // DATAPROCESSOR_H
