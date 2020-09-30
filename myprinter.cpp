#include "myprinter.h"

MyPrinter::MyPrinter()
{
    QSqlDatabase mdb = QSqlDatabase::database();
    if(mdb.open())
        qDebug() << "DB is connected";
    else
    {
        qDebug() << "connecting to database...";
        mdb = QSqlDatabase::addDatabase("QSQLITE");
        mdb.setDatabaseName("MainData.sqlite");
        if(mdb.open())
            qDebug() << "DB is connected";
        else
        {
            qDebug() << "Failed to connect to the database!";
         }
    }

}

QTextDocument *MyPrinter::createHtmlPrintDocument(QPrinter *printer, QStringList contents)
{
    QRect pageRect = printer->pageRect();
    QTextDocument *qTextDoc = new QTextDocument();
    QStringList separatedContentsRow = contents[0].split(" ");
    QString str = "";
    double colwidth = (pageRect.width()/separatedContentsRow.size())*100/pageRect.width();

    qTextDoc->setTextWidth(pageRect.width());
    qTextDoc->setDocumentMargin(0);

    QString sHtmlHeader = "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head><body>";
    QString sHtmlFooter = "</body></html>";

    QString sTable = ( "<h2 align=\"center\">Raport</h2>");

    sTable += "<table width=100% border=0 cellpadding=0 cellspacing=0><tr>";
    for (int i = 0; i < separatedContentsRow.size(); i++)
    {
        sTable += "<td align=\"center\" style=\"font-weight:600\" width=\"" + str.number(colwidth, 'f', 2) + "%\">" + separatedContentsRow[i] + "</td>";
        sTable += "<hr>";
    }
    sTable += "</tr>";


    for (int i = 1; i < contents.size(); i++)
    {
        separatedContentsRow = contents[i].split(" ");
        sTable += "<tr>";
        for (int j = 0; j < separatedContentsRow.size(); j++)
        {
            sTable += "<td align=\"center\" width=\"" + str.number(colwidth, 'f', 2) + "%\">" + separatedContentsRow[j] + "</td>";
        }
        sTable += "</tr>";
    }

    sTable += "</table><br>";

    qTextDoc->setHtml(sHtmlHeader + sTable + sHtmlFooter);

    return qTextDoc;

}

void MyPrinter::printPageHeaderFooter(QPainter *painter, QRect rect, int pageNum)
{
    //painter->drawLine(0, 0, rect.width(), 0);
    painter->drawText(0, -painter->fontMetrics().ascent() + 5, "Rej&Olej for DLA");

   //painter->drawLine(0, rect.height(), rect.width(), rect.height());
    painter->drawText(rect.width() - painter->fontMetrics().horizontalAdvance(QString::number(pageNum)),
                    rect.height() + painter->fontMetrics().ascent() + 5, QString::number(pageNum));
}

