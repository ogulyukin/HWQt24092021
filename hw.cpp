#include "hw.h"
#include "ui_hw.h"


HW::HW(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HW)
{
    ui->setupUi(this);
    this->setWindowTitle("Home Work");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("base.sqlite");
    bool result = loaderWriter::readFromSqlite(&persons, &db);
    if(!result)
        QMessageBox::information(this, "Ошибка", "Не удалось загрузить содержимое базы!");
    ui->personDataLabel->setText(QString::number(persons.size()));
    ui->tabWidget->setCurrentWidget(ui->tab);
    QString css = QString("background-color : %1").arg(QColor(Qt::red).name());
    ui->pushButton->setStyleSheet(css);
    css = QString("background-color : %1").arg(QColor(Qt::darkGreen).name());
    ui->jsonBytton->setStyleSheet(css);
    css = QString("background-color : %1").arg(QColor(245,245,220).name());
    ui->groupBox->setStyleSheet(css);
    ui->groupBox_2->setStyleSheet(css);
    setModel();
}

HW::~HW()
{
    delete ui;
    persons.clear();
}


void HW::on_actionAbout_triggered()
{
    QMessageBox::about(this, "О Программе",
                       "Гулюкин О.В. ВПУ021  \nДан XML-файл. Необходимо из этого файла данные поместить в БД.\nБД разработать на основе xml-файла\n*Написать конвертер из XML в JSON");
}


void HW::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}


void HW::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open File",
                                                    QDir::currentPath(), tr("XML files (*.xml);;All files (*.*)"));
    ui->filenameLabel->setText(filename);
    bool result = loaderWriter::readXml(&persons, filename, &db);
    if (result)
    {
        QMessageBox::information(this, "Успех", "Загрузка завершена успешно");
    }else
    {
        QMessageBox::information(this, "Ошибка", "не удалось загрузить файл!");
    }
    ui->personDataLabel->setText(QString::number(persons.size()));
    setModel();
}


void HW::on_jsonBytton_clicked()
{
    QString outPath = QFileDialog::getExistingDirectory(this, "Open Directory", QDir::currentPath(),
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->jsonLabel->setText(outPath);
    bool result = loaderWriter::writeToJson(&persons, outPath);
    if (result)
    {
        QMessageBox::information(this, "Успех", "Запись завершена успешно\n" + outPath + QDir::separator() + "result.json");
    }else
    {
        QMessageBox::information(this, "Ошибка", "Не удалось записать файл!");
    }
}

void HW::setModel()
{
    model = new QStandardItemModel(this);
    model->setColumnCount(4);
    model->setRowCount(persons.size());
    for (int i = 0; i < persons.count(); i++)
    {
        QModelIndex index = model->index(i, 0, QModelIndex());
        model->setData(index, QVariant::fromValue(persons.at(i)->id));
        index = model->index(i, 1, QModelIndex());
        model->setData(index, QVariant::fromValue(persons.at(i)->name));
        index = model->index(i, 2, QModelIndex());
        model->setData(index, QVariant::fromValue(persons.at(i)->company));
        index = model->index(i, 3, QModelIndex());
        model->setData(index, QVariant::fromValue(persons.at(i)->age));
    }
    QStringList list;
    list << "id" << "Имя" << "Компания" << "Возраст";
    model->setHorizontalHeaderLabels(list);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

