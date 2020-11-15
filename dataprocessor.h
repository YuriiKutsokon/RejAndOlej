#define DEFAULTS_PATH "/RejAndOlej/textfiles/database/default_tables"
#define TABLES_PATH "/RejAndOlej/textfiles/database/columns"
#define DEPENDENCIES_PATH "/RejAndOlej/textfiles/database/columns/dependencies.txt"

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QtSql>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include "textfileprocessor.h"



class DataProcessor
{
public:
    DataProcessor();
    DataProcessor(QString tableName);
    QSqlQuery selectAll(QString tableName);
    QSqlQuery selectAll(QString tableName, QString columnName);

    QSqlQuery selectExact(QString tableName, QStringList colNames, QStringList values, QStringList compSigns, QStringList colsToShow);

    QString getCurrentTable();

    QString getValue(QString tableName, int recordNum, int colNum);
    QString getValue(QString tableName, int recordNum, QString colName);
    QString getValue(QSqlQuery query, int recordNum, int colNum);

    QString getValueParams(QStringList typeNames, QStringList paramVals);

    QString getTablePath(QString tableName);

    QString getParentColumnName(QString tableName, QString colName);

    QString getDependenciesPath();

    void setCurrentTable(QString tableName);

    void addTable(QString tableName);
    void addTable(QString tableName, QStringList colNames, QStringList colTypes, QStringList params);

    void addColumn(QString tableName, QString newColName, QString newColType);
    void addColumn(QString tableName, QStringList colNames, QStringList colTypes, QStringList params);

    void deleteColumn(QString tableName, int index);

    void deleteTable(QString tableName);

    void deleteRow(QString tableName, int id);

    void insertRecord(QString tableName, QString values, QStringList colNames);

    void updateRecord(QString tablename, QStringList colNames, QStringList values, int recordId);

    void initDatabase(QString defaultsPath);

    void addParameter(QString tableName, QString colName, QString parameter);

    int getColNum(QSqlQuery query);
    int getColNum(QString tableName);

    int getRowNum(QSqlQuery query);

    QStringList getColNames(QString tableName);
    QStringList getColNames(QSqlQuery query);

    QStringList getColTypes(QString tableName);

    QStringList getColParams(QString tableName);

    QStringList getTableNames();

    QStringList getQuerySelectResultRecords(QSqlQuery query);

    bool hasDependency(QString TableName,QString colName);

    bool isPrimaryKey(QString tableName, QString colName);

    bool isAutoFill(QString tableName, QString colName);



private:

    //QSqlDatabase mdb;
    QString currentTable = "";
    QString defaultTablesPath;
    QStringList existingTableNames;
    QStringList defaultTableNames;
};



#endif // DATAPROCESSOR_H
