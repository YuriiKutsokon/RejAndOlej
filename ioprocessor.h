#ifndef IOPROCESSOR_H
#define IOPROCESSOR_H

#include <QDebug>
#include "dataprocessor.h"



class IOProcessor
{
public:
    IOProcessor();

    bool checkInput(QString tableName, QMap<QString, QString> inputFields, int parentIndex);
};

#endif // IOPROCESSOR_H
