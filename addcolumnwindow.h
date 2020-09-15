#ifndef ADDCOLUMNWINDOW_H
#define ADDCOLUMNWINDOW_H

#include <QDialog>

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

private:
    Ui::AddColumnWindow *ui;
};

#endif // ADDCOLUMNWINDOW_H
