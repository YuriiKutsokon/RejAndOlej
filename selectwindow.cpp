#include "selectwindow.h"
#include "ui_selectwindow.h"

SelectWindow::SelectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectWindow)
{
    ui->setupUi(this);
    dataProcessor = new DataProcessor();
    currentTable = dataProcessor->getCurrentTable();
    init();
    installEventFilters(lineEdits);

}

SelectWindow::~SelectWindow()
{
    delete ui;
}

QPushButton *SelectWindow::getOkButton()
{
    return ui->pushButton_select;
}

QStringList SelectWindow::getSelectConditions()
{
    return selectConditions;
}

void SelectWindow::init()
{
    QStringList colNames = dataProcessor->getColNames(currentTable);

    QStringList items;
    items.append("=");
    items.append(">");
    items.append("<");
    int dy = 0;
    for (int i = 0; i < colNames.size(); i++)
    {
        QLabel *label = new QLabel(this);
        label->setGeometry(294,70+dy,151,24);
        label->setText(colNames[i]);
        label->setVisible(true);
        labels.append(label);

        QComboBox *comboBox = new QComboBox(this);
        comboBox->setGeometry(470,70+dy,75,24);
        comboBox->addItems(items);
        comboBox->setVisible(true);
        comboBoxes.append(comboBox);

        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setGeometry(550,70+dy,151,24);
        lineEdit->setVisible(true);
        lineEdits.append(lineEdit);
        dy += 30;

    }
}

void SelectWindow::insertToTextEditWidget(QStringList conditions)
{
    ui->textEdit->clear();
    for (int i = 0; i < conditions.size(); i++)
        ui->textEdit->append(conditions[i]);
}

void SelectWindow::installEventFilters(QWidgetList widgets)
{
    for(int i = 0; i < widgets.size(); i++)
        qobject_cast<QLineEdit*>(widgets[i])->installEventFilter(this);
}

bool SelectWindow::eventFilter(QObject *obj, QEvent *evt)
{
    QStringList colTypes = dataProcessor->getColTypes(currentTable);
    bool result = false;
    QString strCondition = "";
    if (evt->type() == QEvent::KeyRelease)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(evt);
        if (ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return)
        {

            for (int i = 0; i < lineEdits.size(); i++)
            {

               if (qobject_cast<QLineEdit*>(lineEdits[i])->isModified())
               {

                    if (colTypes[i] == "QString")
                        strCondition = qobject_cast<QLabel*>(labels[i])->text() +" " + qobject_cast<QComboBox*>(comboBoxes[i])->currentText()
                           + " "  + "'" + qobject_cast<QLineEdit*>(lineEdits[i])->text() + "'";
                    else
                        strCondition = qobject_cast<QLabel*>(labels[i])->text() + " " + qobject_cast<QComboBox*>(comboBoxes[i])->currentText()
                           + " "  + qobject_cast<QLineEdit*>(lineEdits[i])->text();

                    if (selectConditions.size() < lineEdits.size())
                        selectConditions.append(strCondition);
                    else
                        selectConditions.replace(i,strCondition);

                    qobject_cast<QLineEdit*>(lineEdits[i])->setModified(false);

                    insertToTextEditWidget(selectConditions);
                    //qDebug() << selectConditions;
               }

               result = true;
             }

        }
         else result = false;
    }
    return result;
}

void SelectWindow::on_pushButton_select_clicked()
{
    this->close();
}
