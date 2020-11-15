#include "textfileprocessor.h"
#include <QtDebug>

TextFileProcessor::TextFileProcessor()
{

}

QStringList TextFileProcessor::readFromFile(QString fileName)
{
    QStringList strlist;
    QString str, strcheck;
    QFile fileIn(fileName);
    if (fileIn.exists())
    {
        if (fileIn.open(QIODevice::ReadOnly | QIODevice::Text))
        {
                while(!fileIn.atEnd())
            {
                strlist.append(fileIn.readLine());
            }
        }
    }
    else
    {
        qDebug() << "plik nie istnieje!";
    }
    fileIn.close();
    return strlist;
}

void TextFileProcessor::writeToFile(QString fileName, QString string, int param)
{
    QFile fileOut(fileName);
    switch(param)
    {
    case 0:
        if (fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
        {
             QTextStream saveStream(&fileOut);
             saveStream << string;
        }
    break;

    case 1:
        if (fileOut.exists())
        {
            QStringList strList = readFromFile(fileName);
            strList.append(string);
            if (fileOut.open(QIODevice::WriteOnly))
            {
                   QTextStream saveStream(&fileOut);
                   foreach(QString s, strList)
                   {
                       saveStream << s;
                   }
            }
        }
    break;

    }


    fileOut.close();
}


