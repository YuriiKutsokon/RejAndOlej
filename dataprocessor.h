#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H
#include <QtSql>


class DataProcessor
{
public:
    DataProcessor();
    DataProcessor(QString);
    QSqlQuery selectAll(QString tableName);
    QSqlQuery selectExact(QString tableName, QStringList colNames, QStringList values, QStringList compSigns, QStringList colsToShow);
    QString getCurrentTable();
    QString getValue(QString tableName, int recordNum, int colNum);
    QString getValue(QSqlQuery query, int recordNum, int colNum);
    QString getValueParams(QStringList, QStringList);
    void setCurrentTable(QString tableName);
    void addTable(QString);
    void addColumn(QString, QString, QString);
    void addColumn(QString, QStringList, QStringList, QString);
    void deleteColumn(QString, QString);
    void deleteTable(QString tableName);
    void deleteRow(QString tableName, int id);
    void insertRecord(QString tableName, QString values, QStringList colNames);
    void updateRecord(QString tablename, QStringList colNames, QStringList values, int recordId);
    int getColNum(QSqlQuery query);
    int getColNum(QString tableName);
    int getRowNum(QSqlQuery query);
    QStringList getColNames(QString tableName);
    QStringList getColNames(QSqlQuery query);
    QStringList getColTypes(QString tableName);
    QStringList getTableNames();
    QStringList getQuerySelectResultRecords(QSqlQuery query);



private:

    //QSqlDatabase mdb;
    QString currentTable = "";
    QStringList tableNames;
};



#endif // DATAPROCESSOR_H
