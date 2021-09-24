#ifndef HW_H
#define HW_H

#include <QMainWindow>
#include "loaderWriter.h"
#include "person.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class HW; }
QT_END_NAMESPACE

class HW : public QMainWindow
{
    Q_OBJECT

public:
    HW(QWidget *parent = nullptr);
    ~HW();

private slots:
    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_pushButton_clicked();

    void on_jsonBytton_clicked();

private:
    Ui::HW *ui;
    QList<Person*> persons;
    QSqlDatabase db;
    QStandardItemModel* model;
    void setModel();
};
#endif // HW_H
