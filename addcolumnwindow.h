#ifndef ADDCOLUMNWINDOW_H
#define ADDCOLUMNWINDOW_H

#include <QDialog>
#include "dataprocessor.h"

namespace Ui {
class AddColumnWindow;
}

class AddColumnWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddColumnWindow(QWidget *parent = nullptr);
    ~AddColumnWindow();
    QString getColName();
    QString getColType();
    QString getUniqueStatus();
    QString getPrimaryKeyStatus();
    QString getForeginKeyStatus();

    void init_FK_comboBoxes();

private slots:
    void on_checkBox_ForeginKey_stateChanged(int arg1);

    void on_comboBox_FK_Table_currentIndexChanged(const QString &arg1);

private:
    Ui::AddColumnWindow *ui;
};

#endif // ADDCOLUMNWINDOW_H
