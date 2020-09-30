#ifndef MYPRINTER_H
#define MYPRINTER_H

#include <QDebug>
#include <QSqlDatabase>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QTextDocument>
#include "dataprocessor.h"


class MyPrinter
{
public:
    MyPrinter();
    QTextDocument *createHtmlPrintDocument(QPrinter *printer, QStringList contents);
    void printPageHeaderFooter( QPainter *painter, QRect rect, int pageNum);

private:
    //DataProcessor *dataProcessor;
};

#endif // MYPRINTER_H
