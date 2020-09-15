#include "addcolumnwindow.h"
#include "ui_addcolumnwindow.h"

AddColumnWindow::AddColumnWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddColumnWindow)
{
    ui->setupUi(this);
}

AddColumnWindow::~AddColumnWindow()
{
    delete ui;
}

QString AddColumnWindow::getColName()
{
        return ui->lineEdit_colName->text();
}

QString AddColumnWindow::getColType()
{
    return ui->comboBox_colType->currentText();
}

QString AddColumnWindow::getUniqueStatus()
{
    if (ui->checkBox_unique->isChecked())
        return "UNIQUE ";
    else
        return "";
}

QString AddColumnWindow::getPrimaryKeyStatus()
{
    if (ui->checkBox_primaryKey->isChecked())
        return "PRIMARY KEY ";
    else
        return "";
}

