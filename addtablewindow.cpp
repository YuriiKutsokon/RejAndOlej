#include "addtablewindow.h"
#include "ui_addtablewindow.h"

AddTableWindow::AddTableWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTableWindow)
{
    ui->setupUi(this);
}

AddTableWindow::~AddTableWindow()
{
    delete ui;
}

QString AddTableWindow::getNewTableName()
{
    return ui->edit_newTableName->text();
}
