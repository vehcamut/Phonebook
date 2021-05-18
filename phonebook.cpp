#include "phonebook.h"
#include "ui_phonebook.h"

Phonebook::Phonebook(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Phonebook)
{
    this->isChanged = false;
    int rowCount = 0;
    ui->setupUi(this);
    setWindowTitle("Телефонный справочник");
    dialogNewContact = new DialogNewContact();
    dialogEditContact = new DialogEditContact();
    dialogSave = new DialogSave();
    QObject::connect(dialogNewContact, SIGNAL(newContactEntered(std::vector<std::string>)),
                          this, SLOT(onnewContactEntered(std::vector<std::string>)));
    QObject::connect(dialogEditContact, SIGNAL(editContactEntered(std::vector<std::string>)),
                          this, SLOT(oneditContact(std::vector<std::string>)));
    QObject::connect(dialogEditContact, SIGNAL(deleteContactEntered(int)),
                          this, SLOT(ondeleteContact(int)));
    QObject::connect(dialogSave, SIGNAL(buttonPressed(bool)),
                          this, SLOT(onsave(bool)));
    ui->tbMain->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tbMain->setColumnCount(4);
    ui->tbMain->setColumnHidden(3, true);
    for (auto i : Controller.GetContacts())
    {

        ui->tbMain->insertRow(rowCount);
        Controller.Read(i);
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(QString::fromStdString(Controller.contact.GetName()));
        ui->tbMain->setItem(rowCount, 0, item);
        item = new QTableWidgetItem;
        item->setText(QString::fromStdString(Controller.contact.GetPhone()));
        ui->tbMain->setItem(rowCount, 1, item);
        item = new QTableWidgetItem;
        item->setText(QString::fromStdString(Controller.contact.GetInformation()));
        ui->tbMain->setItem(rowCount, 2, item);
        item = new QTableWidgetItem;
        item->setText(QString::number(i));
        ui->tbMain->setItem(rowCount, 3, item);
        //ui->leSearch->setText(ui->tbMain->item(rowCount, 3)->text());
        rowCount++;
    }
}

Phonebook::~Phonebook()
{
    delete ui;
}

void Phonebook::closeEvent(QCloseEvent *event)
{
    /*ui->leName->setText("");
    ui->lePhone->setText("");
    ui->teInformation->setText("");
    event->accept();*/
    //QRegExp rx("\\d\\s\\(\\d\\d\\d\\)\\s\\d\\d\\d-\\d\\d-\\d\\d");
    if (this->isChanged == false) {
        event->accept();
    } else {
        event->ignore();
        dialogSave->show();
    }
}

void Phonebook::on_actionNew_triggered()
{
    for (int i = 0; i < ui->tbMain->rowCount(); ++i)
            ui->tbMain->removeRow(i);
    ui->tbMain->setRowCount(0);
    Controller.NewXml();
    this->isChanged = true;
}


void Phonebook::on_actionSave_triggered()
{
    Controller.SaveXml();
    this->isChanged = false;
}

void Phonebook::on_btSearch_clicked()
{
    Controller.FindAll(ui->leSearch->text().toStdString());
    for (int i = 0; i < ui->tbMain->rowCount(); ++i)
            ui->tbMain->removeRow(i);
    ui->tbMain->setRowCount(0);

    int rowCount = 0;
    for (auto i : Controller.GetContacts())
    {

        ui->tbMain->insertRow(rowCount);
        Controller.Read(i);
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(QString::fromStdString(Controller.contact.GetName()));
        ui->tbMain->setItem(rowCount, 0, item);
        item = new QTableWidgetItem;
        item->setText(QString::fromStdString(Controller.contact.GetPhone()));
        ui->tbMain->setItem(rowCount, 1, item);
        item = new QTableWidgetItem;
        item->setText(QString::fromStdString(Controller.contact.GetInformation()));
        ui->tbMain->setItem(rowCount, 2, item);
        item = new QTableWidgetItem;
        item->setText(QString::number(i));
        ui->tbMain->setItem(rowCount, 3, item);
        rowCount++;
    }

}

void Phonebook::on_btNew_clicked()
{
   // IOXmlController fileController("base.contact");
    dialogNewContact->show();
    //dialogNewContact->
}

void Phonebook::onnewContactEntered(const std::vector<std::string> &con)
{
    this->isChanged = true;
    Controller.contact.SetName(con[0]);
    Controller.contact.SetPhone(con[1]);
    Controller.contact.SetInformation(con[2]);
    //апапапы
    Controller.Add();

    ui->tbMain->insertRow(ui->tbMain->rowCount());
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText(QString::fromStdString(Controller.contact.GetName()));
    ui->tbMain->setItem(ui->tbMain->rowCount() - 1, 0, item);
    item = new QTableWidgetItem;
    item->setText(QString::fromStdString(Controller.contact.GetPhone()));
    ui->tbMain->setItem(ui->tbMain->rowCount() - 1, 1, item);
    item = new QTableWidgetItem;
    item->setText(QString::fromStdString(Controller.contact.GetInformation()));
    ui->tbMain->setItem(ui->tbMain->rowCount() - 1, 2, item);
    item = new QTableWidgetItem;
    item->setText(QString::number(Controller.contact.GetAmount()));
    ui->tbMain->setItem(ui->tbMain->rowCount() - 1, 3, item);
    //rowCount++;
    // Adding a new item to the list widget
    //ui->leSearch->setText(con);
}

void Phonebook::oneditContact(const std::vector<std::string> &con)
{
    this->isChanged = true;
    Controller.contact.SetName(con[0]);
    Controller.contact.SetPhone(con[1]);
    Controller.contact.SetInformation(con[2]);
    //int b = ;
    //int a = ui->tbMain->currentRow();
    Controller.Edit(stoi(con[3]));
    //a = a + 1 - 1;
    //ui->tbMain->insertRow(ui->tbMain->rowCount());
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText(QString::fromStdString(Controller.contact.GetName()));
    ui->tbMain->setItem(ui->tbMain->currentRow(), 0, item);
    item = new QTableWidgetItem;
    item->setText(QString::fromStdString(Controller.contact.GetPhone()));
    ui->tbMain->setItem(ui->tbMain->currentRow(), 1, item);
    item = new QTableWidgetItem;
    item->setText(QString::fromStdString(Controller.contact.GetInformation()));
    ui->tbMain->setItem(ui->tbMain->currentRow(), 2, item);
    /*item = new QTableWidgetItem;
    item->setText(QString::number(Controller.contact.GetAmount()));
    ui->tbMain->setItem(ui->tbMain->rowCount() - 1, 3, item);*/
}
void Phonebook::ondeleteContact(int id)
{
    this->isChanged = true;
    Controller.Delete(id);
    ui->tbMain->removeRow(ui->tbMain->currentRow());
    for (int i = 0; i < ui->tbMain->rowCount(); ++i)
    {
        if  ((ui->tbMain->item(i, 3)->text()).toInt() > id)
            ui->tbMain->item(i, 3)->setText(QString::number((ui->tbMain->item(i, 3)->text()).toInt() - 1));
    }
}
//void Phonebook::on_btEdit_clicked()
//{
    /*//QTableWidgetItem* item;
    std::vector<std::string> temp;
    int row = ui->tbMain->currentRow();
    for (int i = 0; i < 4; i++)
    {
        //item = ui->tbMain->takeItem(row, i);
        temp.push_back(ui->tbMain->item(row, i)->text().toStdString());
    }
    this->dialogEditContact->SetDate(temp);
    //item = ui->tbMain->takeItem(row, 3);
    //temp.push_back(item->text().toStdString());

    //if (!ui->tbMain.contains(model->data(ui->table->selectionModel()->selectedRows().at(i), 0).toString()))
    dialogEditContact->show();
    //(QString::fromStdString(date[0]))*/
//}


void Phonebook::on_tbMain_cellDoubleClicked(int row, int column)
{
    std::vector<std::string> temp;
    //int row = ui->tbMain->currentRow();
    for (int i = 0; i < 4; i++)
    {
        //item = ui->tbMain->takeItem(row, i);
        temp.push_back(ui->tbMain->item(row, i)->text().toStdString());
    }
    this->dialogEditContact->SetDate(temp);
    //item = ui->tbMain->takeItem(row, 3);
    //temp.push_back(item->text().toStdString());

    //if (!ui->tbMain.contains(model->data(ui->table->selectionModel()->selectedRows().at(i), 0).toString()))
    dialogEditContact->show();
}

void Phonebook::onsave(bool flag)
{
    if(flag)
    {
        Controller.SaveXml();
    }
    this->isChanged = false;
    this->close();
}

void Phonebook::on_actionExit_triggered()
{
    this->close();
}

