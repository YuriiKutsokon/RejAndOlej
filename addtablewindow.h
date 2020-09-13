#ifndef ADDTABLEWINDOW_H
#define ADDTABLEWINDOW_H

#include <QDialog>

namespace Ui {
class AddTableWindow;
}

class AddTableWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddTableWindow(QWidget *parent = nullptr);
    ~AddTableWindow();
    QString newTAbleName;
    QString getNewTableName();
private:
    Ui::AddTableWindow *ui;


};

#endif // ADDTABLEWINDOW_H
