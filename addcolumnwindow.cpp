#include "addcolumnwindow.h"
#include "ui_addcolumnwindow.h"

AddColumnWindow::AddColumnWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddColumnWindow)
{
    ui->setupUi(this);
    init_FK_comboBoxes();
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

QString AddColumnWindow::getForeginKeyStatus()
{
    QString strFK_Condition;
    if (ui->checkBox_ForeginKey->isChecked())
    {
        if (!ui->checkBox_autofill->isChecked())
        {
            strFK_Condition = getColName() + ".FK." + ui->comboBox_FK_Table->currentText() + "." + ui->comboBox_FK_Column->currentText();
            return strFK_Condition;
        }
        else
        {
            strFK_Condition = getColName() + ".FKauto." + ui->comboBox_FK_Table->currentText() + "." + ui->comboBox_FK_Column->currentText();
            return strFK_Condition;
        }
    }
    else return "";
}

bool AddColumnWindow::isNextService()
{
    if (ui->checkBox_isNextService->isChecked())
        return true;
    else
        return false;
}

void AddColumnWindow::init_FK_comboBoxes()
{
    DataProcessor dp;
    ui->comboBox_FK_Table->addItems(dp.getTableNames());
}


void AddColumnWindow::on_checkBox_ForeginKey_stateChanged(int arg1)
{
    DataProcessor dp;
    if (arg1 == 2)
    {
        ui->comboBox_FK_Table->setEnabled(true);
        ui->comboBox_FK_Column->setEnabled(true);
        ui->checkBox_autofill->setEnabled(true);
        ui->comboBox_FK_Column->clear();
        ui->comboBox_FK_Column->addItems(dp.getColNames(ui->comboBox_FK_Table->currentText()));
    }
    else
    {
        ui->comboBox_FK_Table->setEnabled(false);
        ui->comboBox_FK_Column->setEnabled(false);
        ui->checkBox_autofill->setEnabled(false);
        ui->comboBox_FK_Column->clear();
    }
}



void AddColumnWindow::on_comboBox_FK_Table_currentIndexChanged(const QString &arg1)
{
    DataProcessor dp;
    ui->comboBox_FK_Column->clear();
    ui->comboBox_FK_Column->addItems(dp.getColNames(arg1));
}

void AddColumnWindow::on_checkBox_stateChanged(int arg1)
{

}

void AddColumnWindow::on_checkBox_isNextService_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        ui->comboBox_colType ->setEnabled(false);
        ui->lineEdit_colName->clear();
        ui->lineEdit_colName->setEnabled(false);
        ui->checkBox_ForeginKey->setEnabled(false);
    }
    else
    {
        ui->comboBox_colType ->setEnabled(true);
        ui->lineEdit_colName->setEnabled(true);
        ui->checkBox_ForeginKey->setEnabled(true);
    }
}
