#ifndef SELECTWINDOW_H
#define SELECTWINDOW_H
#include "dataprocessor.h"
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QKeyEvent>

namespace Ui {
class SelectWindow;
}

class SelectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SelectWindow(QString tableName, QWidget *parent = nullptr);
    ~SelectWindow();

    QPushButton *getOkButton();
    QStringList getSelectConditions();
    QStringList getShowConditions();


private slots:
    void on_pushButton_select_clicked();

private:

    Ui::SelectWindow *ui;
    DataProcessor *dataProcessor;
    QString currentTable;
    QStringList selectConditions;
    QWidgetList labels;
    QWidgetList comboBoxes;
    QWidgetList lineEdits;
    QWidgetList checkBoxes;

    void init();
    void insertToTextEditWidget(QStringList conditions);
    void installEventFilters(QWidgetList widgets);
    bool eventFilter(QObject *obj, QEvent *evt) override;
};

#endif // SELECTWINDOW_H
