#ifndef TEXTFILEPROCESSOR_H
#define TEXTFILEPROCESSOR_H
#include <QString>
#include <QStringList>
#include <QFile>

enum WriteToFileParams
{
    Rewrite_File,
    To_End_Of_File
};

class TextFileProcessor
{
public:
    TextFileProcessor();
    QStringList readFromFile(QString fileName);

    void writeToFile(QString fileName, QString string, int param);
};

#endif // TEXTFILEPROCESSOR_H
